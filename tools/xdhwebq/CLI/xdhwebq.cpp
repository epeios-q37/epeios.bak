/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhwebq.h"

#include "registry.h"

#include "sclt.h"
#include "scle.h"

#include "xdwmain.h"

#include "csdbns.h"
#include "csdcmn.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "mtk.h"
#include "websck.h"
#include "xdhutl.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( xdhwebq, NAME_LC, NAME_MC );

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

namespace {
	namespace {
		typedef csdbns::cProcessing cProcessing_;

		qENUM( State_ ) {
            sProlog,
            sRegular,
            s_amount,
            s_Undefined
		};

		struct rData {
			eState_ State;
			str::wString Token;
			void reset( bso::sBool P = true )
			{
                State = s_Undefined;
                Token.reset( P );
			}
			qCDTOR(rData);
			void Init(void)
			{
                State = s_Undefined;
                Token.Init();
			}
		};

		namespace {
            void HandleProlog_(
                xdwmain::rAgent &Agent,
                fdr::rRWDriver &Driver,
                rData &Data )
            {
            qRH
                qCBUFFERh Buffer;
                flw::rDressedRWFlow<> Flow;
                str::wString Head;
            qRB
                Flow.Init(Driver);
                Head.Init();
                if ( !Agent.GetHead(Data.Token, Head) )
                    sclm::MGetValue(registry::definition::ErrorHead, Head);
                Flow.Write(Head.Convert(Buffer), Head.Amount());
                Flow.Commit();
            qRR
            qRT
            qRE
            }

            namespace {
                class rCapsule_ {
                private:
                    tht::rBlocker
                        Reading_,  // Blocks action reading; to wait until current action be completed.
                        Handling_;  // Blocks action handling; to wait until an action is available.
                    str::wString Id_, Action_;
                public:
                    void reset(bso::sBool P = true)
                    {
                        tol::reset(P, Reading_, Handling_, Id_, Action_);
                    }
                    qCDTOR(rCapsule_);
                    void Init(void)
                    {
                        Reading_.Init();
                        tol::Init(Handling_, Id_, Action_);
                    }
                    void WaitForActionCompletion(void)
                    {
                        Reading_.Wait();
                    }
                    void ReportActionCompleted(void)
                    {
                        Reading_.Unblock();
                    }
                    void Send(
                        const str::dString &Id,
                        const str::dString &Action)
                    {
                        Id_ = Id;
                        Action_ = Action;

                        Handling_.Unblock();
                    }
                    void Get(
                        qCBUFFERh &Id,
                        qCBUFFERh &Action )
                    {
                        Handling_.Wait();

                        Id_.Convert(Id);
                        Action_.Convert(Action);
                    }

                };

                bso::sBool Handle_(
                    const str::dString &Digest,
                    rCapsule_ &Capsule)
                {
                    bso::sBool ActionInProgress = false;
                qRH;
                    str::string Id;
                    xdhutl::event_abstract Abstract;
                qRB;
                    Id.Init();
                    Abstract.Init();
                    if ( xdhutl::FetchEventAbstract(Digest, Id, Abstract) ) {
                        if ( xdhutl::IsPredefined( Abstract.Action() ) )
                            qRVct();
                        else if ( Abstract.Action() == xdhutl::a_User ) {
                            Capsule.Send(Id, Abstract.UserAction);
                            ActionInProgress = true;
                        } else
                            qRGnr();
                    }
                qRR;
                qRT;
                qRE;
                    return ActionInProgress;
                }

                struct sShared_ {
                public:
                    rCapsule_ *Capsule;
                    fdr::rRDriver *Driver;
                    void reset(bso::sBool P = true)
                    {
                        tol::reset(P, Capsule, Driver);
                    }
                    qCDTOR(sShared_);
                    void Init(void)
                    {
                        Capsule = NULL;
                        Driver = NULL;
                    }
                };

                void Routine_(
                    void *UP,
                    mtk::gBlocker &Blocker)
                    {
                    qRH
                        sShared_ &Shared = *(sShared_ *)UP;
                        rCapsule_ &Capsule = *Shared.Capsule;
                        flw::rDressedRFlow<> Flow;
                        str::wString Digest;
                    qRB
                        Flow.Init(*Shared.Driver);

                        Blocker.Release();

                        while(true) {
                            Digest.Init();
                            if ( !websck::GetMessage(Flow,Digest) )
                                break;
                            Flow.Dismiss();
                            if ( Handle_(Digest, Capsule) )
                                Capsule.WaitForActionCompletion();
                    }

                    qRR
                    qRT
                    qRE
                    }
            }

            void HandleRegular_(
                xdwmain::rAgent &Agent,
                fdr::rRWDriver &RawDriver,
                rData &Data)
            {
            qRH
                websck::rDriver Driver;
                flw::rDressedWFlow<> Flow;
                xdwmain::rSession Session;
                str::wString Script;
                rCapsule_ Capsule;
                sShared_ Shared;
                qCBUFFERh Id, Action;
            qRB
                Session.Init(Agent, RawDriver, "", Data.Token);

                if ( Agent.IsValid(Data.Token) )
                    Session.Launch("","");
                else {
                    Script.Init();
                    sclm::MGetValue(registry::definition::ErrorScript, Script);
                    Session.Execute(Script);
                }


                Driver.Init(RawDriver, websck::mWithTerminator, fdr::ts_Default);
                Flow.Init(Driver);

                Capsule.Init();

                Shared.Init();
                Shared.Capsule = &Capsule;
                Shared.Driver = &Driver;

                mtk::Launch(Routine_, &Shared);

                while ( true ) {
                    Capsule.Get(Id, Action);
                    Session.Launch(Id, Action);
                    Flow.Write("StandBy", 7);
                    Flow.Commit();
                    Capsule.ReportActionCompleted();
                }
            qRR
            qRT
            qRE
            }
		}

		class sProcessing
		: public cProcessing_
		{
		private:
			qRMV( xdwmain::rAgent, A_, Agent_ );
		protected:
			void *CSDSCBPreProcess(
				fdr::rRWDriver *Driver,
				const ntvstr::char__ *Origin ) override
			{
                rData *Data = NULL;
			qRH
                websck::wHeader Header;
                websck::rRFlow Flow;
			qRB
                if ( ( Data = new rData ) == NULL )
                    qRAlc();

                Data->Init();

                Header.Init();

                if ( websck::Handshake(*Driver, Header) ) {
                    Flow.Init(*Driver, websck::mWithTerminator);
                    websck::GetMessage(Flow, Data->Token);
                    Data->State = sRegular;
                } else if ( Header.FirstLine == "XDH web prolog" ) {
                    if ( websck::GetValue(str::wString("Token"), Header, Data->Token ) )
                        Data->State = sProlog;
                } else {
                    Data->State = s_Undefined;
                }
			qRR
			qRT
			qRE
				return Data;
			}
			csdscb::action__ CSDSCBProcess(
				fdr::rRWDriver *Driver,
				void *UP ) override
			{

                if ( UP == NULL )
                    qRGnr();

                rData &Data = *(rData *)UP;


                switch ( Data.State ) {
                case sProlog:
                    HandleProlog_(A_(), *Driver, Data);
                    break;
                case sRegular:
                    HandleRegular_(A_(), *Driver, Data);
                    break;
                default:
                    qRGnr();
                    break;
                }

				return csdscb::aStop;
			}
			virtual bso::sBool CSDSCBPostProcess( void *UP ) override
			{
                if ( UP == NULL )
                    qRGnr();

                delete (rData *)UP;
				return true;
			}
		public:
			void reset( bso::bool__ P = true )
			{
				Agent_ = NULL;
			}
			E_CVDTOR( sProcessing );
			void Init( xdwmain::rAgent &Agent )
			{
				Agent_ = &Agent;
			}
		};

	}

	void Process_( void )
	{
	qRH
		xdwmain::rAgent Agent;
		str::wString Identification, ModuleFilename;
		TOL_CBUFFER___ IdentificationBuffer;
		csdbns::server___ Server;
		sProcessing Callback;
		qCBUFFERh Buffer;
	qRB
		Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		sclm::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		Agent.Init( xdhcmn::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ) );

		Callback.Init( Agent );

		Server.Init( sclm::MGetU16( registry::parameter::Service ), Callback );
		Server.Process( NULL );
	qRR
	qRT
	qRE
	}
}

const scli::sInfo &sclt::SCLTInfo( void )
{
	return xdhwebq::Info;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}
