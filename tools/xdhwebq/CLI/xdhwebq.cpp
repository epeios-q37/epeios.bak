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
//			xdwmain::sRow Row;
			void reset( bso::sBool P = true )
			{
                State = s_Undefined;
                Token.reset( P );
//                Row = qNIL;
			}
			qCDTOR(rData);
			void Init(void)
			{
                State = s_Undefined;
                Token.Init();
//                Row = qNIL;
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
                qENUM(Task_) {
                    tNone,      // No task.
                    tAction,    // Handle action.
                    tClose,      // Close session.
                    t_amount,
                    t_Undefined
                };

                class rTask_ {
                private:
                    tht::rLocker Locker_;
                    eTask_ Task_;
                    str::wString Id_, Action_;
                    void Lock_(void)
                    {
                        Locker_.Lock();
                    }
                    void Unlock_(void)
                    {
                        Locker_.Unlock();
                    }
                    eTask_ Set_(
                        eTask_ Task,
                        const str::dString &Id,
                        const str::dString &Action)
                    {
                        eTask_ PreviousTask = t_Undefined;

                        if ( Task >= t_amount )
                            qRGnr();

                        Lock_();

                        Task_ = Task;

                        Id_ = Id;
                        Action_ = Action;

                        Unlock_();

                        return PreviousTask;
                    }
                    eTask_ Set_(eTask_ Task)
                    {
                        return Set_(Task, str::Empty, str::Empty);
                    }
                public:
                    void reset(bso::sBool P = true)
                    {
                        Task_ = t_Undefined;
                        tol::reset(P, Locker_, Id_, Action_);
                    }
                    qCDTOR(rTask_);
                    void Init(void)
                    {
                        Task_ = t_Undefined;
                        tol::Init(Locker_, Id_, Action_);
                    }
                    eTask_ SetNOP(void)
                    {
                        return Set_(tNone);
                    }
                    eTask_ SetClose(void)
                    {
                        return Set_(tClose);
                    }
                    eTask_ Set(
                        const str::dString &Id,
                        const str::dString &Action)
                    {
                        if ( Id.Amount() ==  0)
                            qRGnr();

                        if ( Action.Amount() == 0 )
                            qRGnr();

                        return Set_(tAction, Id, Action);
                    }
                    eTask_ Set(const str::dString &Id)
                    {
                        if ( Id.Amount() == 0 )
                            qRGnr();

                        return Set_(tAction, Id, str::Empty);
                    }
                    eTask_ Get(
                        qCBUFFERh &Id,
                        qCBUFFERh &Action )
                    {
                        eTask_ Task = t_Undefined;

                        Lock_();

                        Task = Task_;

                        if ( Task == tAction) {
                            Id_.Convert(Id);
                            Action_.Convert(Action);
                        }

                        Task_ = t_Undefined;
                        tol::Init(Id_, Action_);

                        Unlock_();

                        return Task;
                    }
                    bso::sBool IsPending(void) const
                    {
                        return Task_ != t_Undefined;    // Expected to be atomic.
                    }
                };

                qENUM(Actor_) {
                    aRegular,
                    aSpecial,
                    a_amount,
                    a_Undefined
                };

                class rCapsule_ {
                private:
                    eActor_ Actor_;
                    tht::rBlocker
                        Reading_,  // Blocks action reading; to wait until current action be completed.
                        Handling_;  // Blocks action handling; to wait until an action is available.
                    rTask_
                        Regular_,    // Regular task issued by client.
                        Special_;    // Special task directly issued by the front end.
                public:
                    void reset(bso::sBool P = true)
                    {
                        Actor_ = a_Undefined;
                        tol::reset(P, Reading_, Handling_, Regular_, Special_);
                     }
                    qCDTOR(rCapsule_);
                    void Init(void)
                    {
                        Actor_ = a_Undefined;
                        tol::Init(Reading_, Handling_, Regular_, Special_);
                     }
                    void SetRegularNOP(void)
                    {
                        if ( Regular_.SetNOP() != t_Undefined )
                            qRGnr();

                        Handling_.Unblock();
                        Reading_.Wait();
                    }
                    void SetRegular(
                        const str::dString &Id,
                        const str::dString &Action)
                    {
                        if ( Regular_.Set(Id, Action) != t_Undefined )
                            qRGnr();

                        Handling_.Unblock();
                        Reading_.Wait();
                    }
                    bso::sBool SetSpecial(const str::dString &Id)
                    {
                        if ( Special_.Set(Id) == t_Undefined ) {
                            Handling_.Unblock();
                            return true;
                        } else
                            return false;
                    }
                    eTask_ Get(
                        qCBUFFERh &Id,
                        qCBUFFERh &Action )
                    {
                        eTask_ Task = t_Undefined;

                        Handling_.Wait();

                        Task = Special_.Get(Id, Action);

                        if ( Task == t_Undefined ) {
                            Task = Regular_.Get(Id, Action);

                            if ( Task == t_Undefined )
                                qRGnr();
                            else
                                 Actor_ = aRegular;
                        } else
                            Actor_ = aSpecial;

                        return Task;
                    }
                    bso::sBool HasPending(void)
                    {
                        if ( Actor_ == aRegular)
                            Reading_.Unblock();

                        Actor_ = a_Undefined;

                        return Special_.IsPending() || Regular_.IsPending();
                    }
                };


                bso::sBool Handle_(
                    const str::dString &Digest,
                    str::dString &Id,
                    str::dString &Action)
                {
                    bso::sBool ActionInProgress = false;
                qRH;
                    xdhutl::wEventAbstract Abstract;
                qRB;
                    Abstract.Init();
                    if ( xdhutl::FetchEventAbstract(Digest, Id, Abstract) ) {
                        if ( xdhutl::IsPredefined( Abstract.Action() ) )
                            qRVct();
                        else if ( Abstract.Action() == xdhutl::a_User ) {
                            // 'Id' is already set.
                            Action = Abstract.UserAction;
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
                        str::wString Digest, Id, Action;
                    qRB
                        Flow.Init(*Shared.Driver);

                        Blocker.Release();

                        while(true) {
                            Digest.Init();
                            if ( !websck::GetMessage(Flow,Digest) )
                                break;
                            Flow.Dismiss();

                            tol::Init(Id, Action);

                            if ( Handle_(Digest, Id, Action) )
                                Capsule.SetRegular(Id, Action);
                            else
                                Capsule.SetRegularNOP();
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
                xdhcmn::cSession *Callback = NULL;
            qRB
                Callback = Agent.FetchSessionCallback();

                if ( Callback == NULL )
                    qRGnr();

                Session.Init(Callback, RawDriver, "", Data.Token);
//                Data.Row = xdwmain::Add(RawDriver, Data.Token);

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
                    switch( Capsule.Get(Id, Action) ) {
                    case tNone:
                        break;
                    case tAction:
                        Session.Launch(Id, Action);
                        break;
                    case tClose:
                        qRVct();
                        break;
                    default:
                        qRGnr();
                        break;
                    }

                    if ( !Capsule.HasPending() ) {
                        Flow.Write("StandBy", 7);
                        Flow.Commit();
                    }
                }
            qRR
            qRT
                if ( Callback != NULL )
                    Agent.DismissSessionCallback(Callback);
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

                rData *Data = (rData *)UP;

#if 0
                if ( Data->Row != qNIL )
                    xdwmain::Remove(Data->Row, Data->Token);
#endif
                delete Data;

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
