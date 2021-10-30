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

#include "xdwsessn.h"

#include "csdbns.h"
#include "csdcmn.h"
#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "flw.h"
#include "mtk.h"
#include "websck.h"

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

namespace faas_ {
	using namespace xdhcmn::faas;
}

namespace {
	namespace {
		typedef csdbns::cProcessing cProcessing_;

		qENUM( State_ )
		{
			sProlog,
			sRegular,
			s_amount,
			s_Undefined
		};

		struct rData {
			eState_ State;
			str::wString Token, UserId;
			void reset( bso::sBool P = true )
			{
				State = s_Undefined;
				tol::reset(P, Token, UserId);
			}
			qCDTOR(rData);
			void Init(void)
			{
				State = s_Undefined;
				tol::Init(Token, UserId);
			}
		};

		namespace {
			void HandleProlog_(
				xdhups::rAgent &Agent,
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
		}

		namespace {
			// Special scripts, which are not executed by the client,
			// but intercepted to launch a special action.
			namespace ss_ {
				namespace standby {
					qCDEF(char *, Label, "%StandBy");
					qCDEF(bso::sSize, Size, 8);
				}
			}

			namespace {
			  namespace {
			    bso::sBool SendConnectEvent_(flw::rRWFlow &Flow)
          {
          qRH;
            str::wString Script;
            qCBUFFERh Buffer;
          qRB;
            Script.Init("launchEvent('');");  // Launch an event with an empty digest.

            Flow.Write(Script.Convert(Buffer), Script.Amount());

            Flow.Commit();
            Flow.Dismiss();
          qRR;
          qRT;
          qRE;

          return true;
          }
			  }

				void HandleRegular_(
					xdhups::rAgent &Agent,
					fdr::rRWDriver &Driver,
					xdhcdc::cSingle &Callback,
					const str::dString &Token,
					const str::dString &UserId)
				{
				qRH
					websck::rFlow Flow;
					xdwsessn::rSession Session;
					str::wString Digest, Script;
					qCBUFFERh Buffer;
				qRB
					Flow.Init(Driver, websck::mWithTerminator);

					if ( Session.Init(Callback, Driver, "", Token, UserId) ==  qNIL ) {
						Script.Init();
						sclm::MGetValue(registry::definition::ErrorScript, Script);
						Session.Execute(Script);
					// } else if ( Session.Handle(NULL) ) { // Reponses of the connect event were mixed the ones from upcoming events.
					 } else if ( SendConnectEvent_(Flow) ) {  // To queue upcoming events.
						while ( true ) {
							Digest.Init();
							if ( !websck::GetMessage(Flow, Digest) )
								break;
							Flow.Dismiss();
							if ( !Session.Handle(Digest.Convert(Buffer)) )
								break;
							Flow.Write(ss_::standby::Label, ss_::standby::Size);
							Flow.Commit();
						}
					}
				qRR
				qRT
				qRE
				}
			}

			void HandleRegular_(
				xdhups::rAgent &Agent,
				fdr::rRWDriver &Driver,
				rData &Data)
			{
			qRH
				xdhcdc::cSingle *Callback = NULL;
			qRB
				Callback = Agent.FetchCallback();

				if ( Callback == NULL )
					qRGnr();

				HandleRegular_(Agent, Driver, *Callback, Data.Token, Data.UserId);
			qRR
			qRT
				if ( Callback != NULL )
					Agent.DismissCallback(Callback);
			qRE
			}
		}

		class sProcessing
		: public cProcessing_
		{
		private:
			qRMV( xdhups::rAgent, A_, Agent_ );
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
					Flow.Init(*Driver, websck::mWithTerminator);	// Second time: uses the WebSocket initiated below.
					websck::GetMessage(Flow, Data->Token);
					websck::GetMessage(Flow, Data->UserId);
					Data->State = sRegular;
				} else if ( Header.FirstLine == "XDH web prolog" ) {
					if ( websck::GetValue(str::wString("Token"), Header, Data->Token) )	// First time: launch PHP program which initiates the WebSocket
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
			void Init(xdhups::rAgent &Agent)
			{
				Agent_ = &Agent;
			}
		};

	}

	namespace {
		namespace {
			typedef xdhcuc::cGlobal cUpstream_;

			class sUpstream_
			: public cUpstream_
			{
			protected:
				virtual faas_::sRow XDHCUCCreate(const str::dString &Token) override
				{
					return xdhbrd::Create(Token);
				}
				virtual void XDHCUCRemove(faas_::sRow Row) override
				{
					return xdhbrd::Remove(Row);
				}
				virtual void XDHCUCBroadcast(
					const str::dString &Script,
					faas_::sRow Row) override
				{
					xdhbrd::Broadcast(Script, Row);
				}
			public:
				void reset(bso::sBool = true)
				{}
				qCVDTOR(sUpstream_)
				void Init(void)
				{}
			};
		}
	}

	void Process_( void )
	{
	qRH
		xdhups::rAgent Agent;
		str::wString Identification, ModuleFilename;
		TOL_CBUFFER___ IdentificationBuffer;
		csdbns::server___ Server;
		sProcessing Callback;
		sUpstream_ Upstream_;
		qCBUFFERh Buffer;
	qRB
		Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
		Identification.Append( cpe::GetDescription() );

		ModuleFilename.Init();
		sclm::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

		Upstream_.Init();

		Agent.Init(Upstream_, xdhcdc::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ));

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
