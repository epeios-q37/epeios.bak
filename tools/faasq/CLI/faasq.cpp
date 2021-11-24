/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

    'FaaSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FaaSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "faasq.h"

#include "registry.h"
#include "session.h"

#include "sclc.h"
#include "sclt.h"
#include "scle.h"

#include "err.h"
#include "cio.h"
#include "csdbnc.h"
#include "csdcmn.h"
#include "epsmsc.h"
#include "mtk.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "lstbch.h"
#include "xdhups.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( faasq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return faasq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}

	namespace {
		void Initialize_(csdbnc::rRWDriver &Proxy)
		{
		qRH
			str::wString Host, Service;
			qCBUFFERh HostBuffer, ServiceBuffer;
		qRB
			tol::Init(Host, Service);
			sclm::MGetValue(registry::parameter::proxy::Host, Host);
			sclm::MGetValue(registry::parameter::proxy::Service, Service);

			sclc::Display("ConnectingTo", cio::COut, Host, Service);
			cio::COut.Commit();

			if (!Proxy.Init(Host.Convert(HostBuffer), Service.Convert(ServiceBuffer), SCK__DEFAULT_TIMEOUT, qRPU))
				sclc::ReportAndAbort("UnableToConnectTo", Host, Service);

			sclc::Display("ConnectedTo", cio::COut, Host, Service);
			cio::COut.Commit();
		qRR
		qRT
		qRE
		}
	}

	namespace {
		namespace {
			qCDEF( char *, FaaSProtocolLabel_, "9efcf0d1-92a4-4e88-86bf-38ce18ca2894" );
			qCDEF(bso::sU8, FaaSProtocolVersion_, 0);
		}

		void HandShake_(fdr::rRWDriver &ProxyDriver)
		{
		qRH
			flw::rDressedRWFlow<> Proxy;
			str::wString Message;
		qRB
			Proxy.Init(ProxyDriver);

			csdcmn::SendProtocol(FaaSProtocolLabel_, FaaSProtocolVersion_, Proxy);
			Proxy.Commit();

			Message.Init();
			csdcmn::Get(Proxy, Message);

			if ( Message.Amount() )
				sclc::ReportAndAbort(Message);

			Message.Init();
			csdcmn::Get(Proxy, Message);

			cio::COut << Message << txf::nl << txf::commit;
		qRR
		qRT
		qRE
		}
	}

	namespace {
		namespace faas_ {
			using namespace xdhcmn::faas;
		}

		namespace {
			namespace {
				void Initialize_(
						xdhcuc::cGlobal &Upstream,
						xdhups::rAgent &Agent)
				{
				qRH
					str::wString Identification, ModuleFilename;
					qCBUFFERh Buffer;
				qRB
					Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
					Identification.Append( cpe::GetDescription() );

					ModuleFilename.Init();
					sclm::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

					Agent.Init(Upstream, xdhcdc::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ) );
				qRR
				qRT
				qRE
				}
			}
		}

		namespace {
			void _Ignition_(
				fdr::rRWDriver &ProxyDriver,
				xdhups::rAgent &Agent)
			{
			qRH
				flw::rDressedRWFlow<> Proxy;
				str::wString Head, Host, Token, Message, URL;
			qRB
				Proxy.Init(ProxyDriver);

				Head.Init();
				if ( !Agent.GetHead(str::wString(""),Head) )
					qRGnr();

				csdcmn::Put("", Proxy);
				csdcmn::Put(Head, Proxy);
				Host.Init();
				csdcmn::Put(registry::GetWebHost(Host), Proxy);
				csdcmn::Put("XDH", Proxy);
				Proxy.Commit();

				Token.Init();

				csdcmn::Get(Proxy, Token);

				if ( !Token.Amount() ) {
					Message.Init();
					csdcmn::Get(Proxy, Message);
					sclc::ReportAndAbort(Message);
				}

				URL.Init();
				csdcmn::Get(Proxy, URL);

				cio::COut << URL << txf::nl << txf::commit;

				tol::Launch(URL);
			qRR
			qRT
			qRE
			}
		}

		namespace {
			using namespace session;

			namespace {
				qCDEF( char *, _MainProtocolLabel_, "bf077e9f-baca-48a1-bd3f-bf5181a78666" );
				qCDEF(bso::sU8, MainProtocolVersion_, 0);
			}

			namespace {
				void Routine_(
					void *UP,
					mtk::gBlocker &Blocker)
				{
					rSession &Session = *(rSession *)UP;

					Blocker.Release();

					Session.Launch();
				}
			}

			void Handle_(
				fdr::rRWDriver &ProxyDriver,
				xdhups::rAgent &Agent)
			{
			qRH
				wIds Ids;
				wSessions Sessions;
				faas_::sId Id = faas_::UndefinedId;
				sRow Row = qNIL;
				str::wString Message, EId, Action;
				qCBUFFERh EIdBuffer, ActionBuffer;
				rSession *Session = NULL;
				flw::rDressedRWFlow<> Proxy;
				tht::rBlocker Blocker;
			qRB
				tol::Init(Ids, Sessions);
				Proxy.Init(ProxyDriver);

				Blocker.Init();

				while ( true ) {
					switch ( csdcmn::Get(Proxy, Id) ) {
					case faas_::UndefinedId:
						qRFwk();
						break;
					case faas_::upstream::CreationId:
						if ( Search(csdcmn::Get(Proxy, Id), Ids) != qNIL )
							sclc::ReportAndAbort("IdShouldNotExists", Id);

						Proxy.Dismiss();

						Row = Ids.Append(Id);

						if ( ( Session = new rSession() ) == NULL )
							qRAlc();

						Session->Init(Id, Agent, ProxyDriver, Blocker);

						if ( Sessions.Append(Session) != Row )
							qRGnr();

						Session = NULL;

						csdcmn::Put(Id, Proxy);
						csdcmn::SendProtocol(MainProtocolLabel_, MainProtocolVersion_, Proxy);

						Proxy.Commit();
						break;
					case faas_::upstream::ClosingId:
						if ( ( Row = Search(csdcmn::Get(Proxy, Id), Ids) ) == qNIL )
							qRGnr();

						Proxy.Dismiss();

						Session = Sessions(Row);

						if ( Session == NULL )
							qRGnr();

						Session->Quit = true;
						Session->Unblock();
						Blocker.Wait();

						delete Session;

						Session = NULL;

						Sessions.Remove(Row);
						Ids.Remove(Row);

						break;
					default:
						if ( ( Row = Search(Id, Ids) ) == qNIL )
							qRGnr();

						Session = Sessions(Row);

						if ( Session == NULL )
							qRGnr();

						if ( !Session->Handshaked) {
							Message.Init();

							csdcmn::Get(Proxy, Message);

							if ( Message.Amount())
								sclc::ReportAndAbort(Message);

							csdcmn::Get(Proxy, Message);    // Language; not handled yet.

							Proxy.Dismiss();

							Session->Handshaked = true;

							mtk::LaunchAndKill(Routine_, Session);
						} else {
							Proxy.Dismiss();
							Session->Unblock();
							Blocker.Wait();
						}
						break;
					}
				}
			qRR
			qRT
			qRE
			}
		}

		namespace {
			typedef xdhcuc::cGlobal cUpstream_;

			class sUpstream_
			: public cUpstream_
			{
			private:
				qRMV(fdr::rWDriver, P_, Proxy_);
			protected:
				virtual faas_::sRow XDHCUCCreate(const str::dString &) override
				{
					qRGnr();

					return qNIL;    // To avoid a warning.
				}
				virtual void XDHCUCBroadcast(
					const str::dString &Script,
					const faas_::sRow TRow) override
					{
					qRH
						flw::rDressedWFlow<> Proxy;
					qRB
						if ( TRow != qNIL )
							qRGnr();    // See below.

						Proxy.Init(P_());

						csdcmn::Put(faas_::downstream::BroadcastScriptId, Proxy);
						csdcmn::Put(Script, Proxy);

					//    csdcmn::Put(TRow, Proxy);    // 'TRow', when relevant (FaaS mode), is only handled by the 'xdhqxdh' tool.

						Proxy.Commit();
					qRR
					qRT
					qRE
				}
				virtual void XDHCUCRemove(faas_::sRow) override
				{
					qRGnr();
				}
			public:
				void reset(bso::sBool = true)
				{
					Proxy_ = NULL;
				}
				qCVDTOR(sUpstream_);
				void Init(fdr::rWDriver &ProxyDriver)
				{
					Proxy_ = &ProxyDriver;
				}
			};
		}


		void Process_(fdr::rRWDriver &ProxyDriver)
		{
		qRH
			xdhups::rAgent Agent;
			sUpstream_ Upstream;
		qRB
			Upstream.Init(ProxyDriver);
			Initialize_(Upstream, Agent);

			Ignition_(ProxyDriver, Agent);
			Handle_(ProxyDriver, Agent);
		qRR
		qRT
		qRE
		}
	}

	void _Process_( void )
	{
	qRH;
		csdbnc::rRWDriver Proxy;
	qRB;
		Initialize_(Proxy);
		HandShake_(Proxy);
		Process_(Proxy);
	qRR;
	qRT;
	qRE;
	}
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH;
qRB;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Process );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
