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
            qCDEF( char *, FaaSProtocolLabel_, "0fac593d-d65f-4cc1-84f5-3159c23c616b" );
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
        namespace {
            namespace {
                namespace {
                    typedef xdhcuc::cGlobal cUpstream_;

                    // Must exist, but not used.
                    class sUpstream_
                    : public cUpstream_
                    {
                    protected:
                        virtual xdhcuc::sRow XDHCUCCreate(const str::dString &) override
                        {
                            qRGnr();

                            return qNIL;    // To avoid a warning.
                        }
                        virtual void XDHCUCRemove(xdhcuc::sRow Row) override
                        {
                            qRGnr();
                        }
                    } Upstream_;
                }

                void Initialize_(xdhups::rAgent &Agent)
                {
                qRH
                    str::wString Identification, ModuleFilename;
                    qCBUFFERh Buffer;
                qRB
                    Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
                    Identification.Append( cpe::GetDescription() );

                    ModuleFilename.Init();
                    sclm::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

                    Agent.Init(Upstream_,  xdhcdc::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ) );
                qRR
                qRT
                qRE
                }
            }
        }

        namespace {
            void Ignition_(
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
            qRR
            qRT
            qRE
            }
        }

        namespace {
            using namespace session;

            namespace {
                qCDEF( char *, MainProtocolLabel_, "8d2b7b52-6681-48d6-8974-6e0127a4ca7e" );
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
                sId Id = 0;
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

                while(true) {
                    if ( csdcmn::Get(Proxy, Id) == UndefinedId) {
                        if ( Search(csdcmn::Get(Proxy, Id), Ids) != qNIL )
                            sclc::ReportAndAbort("IdShouldNotExists", Id);

                        Proxy.Dismiss();

                        Row = Ids.Append(Id);

                        if ( ( Session = new rSession() ) == NULL )
                            qRAlc();

                        Session->Init(Id, Agent, ProxyDriver, Blocker);

                        if ( Sessions.Append(Session) != Row )
                            qRGnr();

                        csdcmn::Put(Id, Proxy);
                        csdcmn::SendProtocol(MainProtocolLabel_, MainProtocolVersion_, Proxy);

                        Proxy.Commit();
                    } else {
                        if ( ( Row = Search(Id, Ids) ) == qNIL )
                            qRGnr();

                        Session = Sessions(Row);

                        if ( !Session->Handshaked) {
                            Message.Init();

                            csdcmn::Get(Proxy, Message);

                            if ( Message.Amount())
                                sclc::ReportAndAbort(Message);

                            csdcmn::Get(Proxy, Message);    // Language; not handled yet.

                            Proxy.Dismiss();

                            csdcmn::Put(Id, Proxy);
                            csdcmn::Put("XDH", Proxy);
                            Proxy.Commit();

                            Session->Handshaked = true;

                            mtk::LaunchAndKill(Routine_, Session);
                        } else {
                            Proxy.Dismiss();
                            Session->Unblock();
                            Blocker.Wait();
                        }
                    }

                }
            qRR
            qRT
            qRE
            }
        }

        void Process_(fdr::rRWDriver &ProxyDriver)
        {
        qRH
            xdhups::rAgent Agent;
        qRB
            Initialize_(Agent);
            Ignition_(ProxyDriver, Agent);
            Handle_(ProxyDriver, Agent);
        qRR
        qRT
        qRE
        }
	}

	void Process_( void )
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
