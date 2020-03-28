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

#include "sclc.h"
#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "csdbnc.h"
#include "csdcmn.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "lstbch.h"
#include "xdhups.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( faasq, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
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
        void Initialize_(csdbnc::rRWFlow &Proxy)
        {
        qRH
            str::wString HostService;
            qCBUFFERr Buffer;
        qRB
            HostService.Init();
            sclmisc::MGetValue(registry::parameter::HostService, HostService);

            sclc::Display("TryToConnectTo", cio::COut, HostService);

            if (!Proxy.Init(HostService.Convert(Buffer), SCK__DEFAULT_TIMEOUT, err::h_Default))
                sclc::ReportAndAbort("UnableToConnectTo", HostService);

            sclc::Display("ConnectedTo", cio::COut, HostService);
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

        void HandShake_(flw::rRWFlow &Proxy)
        {
        qRH
            str::wString Message;
        qRB
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
                void Initialize_(xdhups::rAgent &Agent)
                {
                qRH
                    str::wString Identification, ModuleFilename;
                    qCBUFFERr Buffer;
                qRB
                    Identification.Init( NAME_LC " V" VERSION " Build " __DATE__ " " __TIME__ " - " );
                    Identification.Append( cpe::GetDescription() );

                    ModuleFilename.Init();
                    sclmisc::MGetValue( registry::parameter::ModuleFilename, ModuleFilename );

                    Agent.Init( xdhcmn::mMultiUser, ModuleFilename, dlbrry::n_Default, Identification.Convert( Buffer ) );
                qRR
                qRT
                qRE
                }
            }
        }

        namespace {
            void Ignition_(
                flw::rRWFlow &Proxy,
                xdhups::rAgent &Agent)
            {
            qRH
                str::wString Head, Token, Message, URL;
            qRB
               Head.Init();
               if ( !Agent.GetHead(str::wString(""),Head) )
                    qRGnr();

                csdcmn::Put("", Proxy);
                csdcmn::Put(Head, Proxy);
                csdcmn::Put("faas1.q37.info", Proxy);
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

                cio::COut << Head << txf::nl << URL << txf::nl << txf::commit;
            qRR
            qRT
            qRE
            }
        }

        namespace {
            namespace {
                typedef bso::sU8 sId_;
                qCDEF(sId_, UndefinedId_, 255);
                qROW(Row_);

                qCDEF( char *, MainProtocolLabel_, "8d2b7b52-6681-48d6-8974-6e0127a4ca7e" );
                qCDEF(bso::sU8, MainProtocolVersion_, 0);

                class sSession_
                {
                public:
                    xdhcmn::cSession *Session;
                    sId_ Id;
                    bso::sBool Handshaked;
                    void reset(bso::sBool P = true)
                    {
                        Id = UndefinedId_;
                        Session = NULL;
                        Handshaked = false;
                    }
                    sSession_(
                        sId_ Id,
                        xdhcmn::cSession *Session)
                    : sSession_()
                    {
                        Init(Id, Session);
                    }
                    qCDTOR(sSession_);
                    void Init(
                        sId_ Id,
                        xdhcmn::cSession *Session)
                    {
                        this->Id = Id;
                        this->Session = Session;
                        Handshaked = false;
                    }
                };

                typedef lstbch::qBUNCHd(sSession_,sRow_) dSessions_;
                qW(Sessions_);

                typedef lstbch::qBUNCHd(sId_, sRow_) dIds_;
                qW(Ids_);

                sRow_ Search_(
                    sId_ Id,
                    const dIds_ &Ids)
                {
                    sRow_ Row = Ids.First();

                    while ( ( Row != qNIL ) && ( Ids(Row) != Id) )
                        Row = Ids.Next(Row);

                    return Row;
                }
            }

            void Handle_(
                flw::rRWFlow &Proxy,
                xdhups::rAgent &Agent)
            {
            qRH
                wIds_ Ids;
                wSessions_ Sessions;
                bso::sU8 Id = 0;
                sRow_ Row = qNIL;
                str::wString Message;
            qRB
                tol::Init(Ids, Sessions);

                while(true) {
                    if ( csdcmn::Get(Proxy, Id) == UndefinedId_) {
                        if ( Search_(csdcmn::Get(Proxy, Id), Ids) != qNIL )
                            sclc::ReportAndAbort("IdSholdNotExists", Id);

                        Proxy.Dismiss();

                        Row = Ids.Append(Id);

                        if ( Sessions.Append(sSession_(Id,Agent.RetrieveSession())) != Row )
                            qRGnr();

                        csdcmn::SendProtocol(MainProtocolLabel_, MainProtocolVersion_, Proxy);

                        Proxy.Commit();
                    } else {
                        if ( ( Row = Search_(Id, Ids) ) == qNIL )
                            qRGnr();

                        if ( !Sessions(Row).Handshaked) {
                            Message.Init();

                            csdcmn::Get(Proxy, Message);

                            if ( Message.Amount())
                                sclc::ReportAndAbort(Message);

                            csdcmn::Get(Proxy, Message);    // Language; not ahndled yet.

                            Proxy.Dismiss();

                            csdcmn::Put(Id, Proxy);
                            csdcmn::Put("XDH", Proxy);
                            Proxy.Commit();
                        } else
                            qRLmt();
                    }

                }
            qRR
            qRT
            qRE
            }
        }

        void Process_(flw::rRWFlow &Proxy)
        {
        qRH
            xdhups::rAgent Agent;
        qRB
            Initialize_(Agent);
            Ignition_(Proxy, Agent);
            Handle_(Proxy, Agent);
        qRR
        qRT
        qRE
        }
	}

	void Process_( void )
	{
	qRH;
		csdbnc::rRWFlow Proxy;
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

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
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
