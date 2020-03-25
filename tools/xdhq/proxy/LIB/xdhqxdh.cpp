/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdhqxdh.h"

#include "registry.h"
#include "session.h"

#include "prtcl.h"

#include "logq.h"
#include "idsq.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclmisc.h"
#include "sclxdhtml.h"

SCLI_DEF( xdhqxdh, PROGRAM_NAME, SOFTWARE_NAME );

namespace {
	// As we do not need a backend, most of below items are only placeholders.
	class sDummy_
	{
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( sDummy_ );
		void Init( void )
		{
			// Standardization.
		}
	};


	class rFrontend_
	: public sclfrntnd::rFrontend,
	  public fblfrd::cFrontend
	{
	protected:
		virtual void FBLFRDOnConnection( void ) override {}
		virtual void FBLFRDOnDisconnection( void ) override {}
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( rFrontend_ );
		void Init( void )
		{
			// Standardization.
		}
	};

	qENUM( Dummy )
	{
		dAmount,
		d_Undefined
	};

	namespace {
		void Execute_(
            const str::dString &Script,
            xdhdws::sProxy &Proxy,
            str::dString &ReturnValue )
        {
        qRH;
            str::wStrings Values;
        qRB;
            Values.Init();
            Values.Append(Script);

            Proxy.Process( "Execute_1", Values, ReturnValue );
        qRR;
        qRT;
        qRE;
        }
    }

	void Execute_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Script;
		str::wStrings Values;
		str::wString Result;
	qRB;
		Script.Init();
		prtcl::Get( Flow, Script );

		Flow.Dismiss();

		Values.Init();
		Values.Append(Script);

        Result.Init();
        Execute_(Script, Proxy, Result );

		prtcl::Put( Result, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Alert_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		str::wString ReturnValue;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "window.alert(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\");'';");

        ReturnValue.Init();
		Execute_( Script, Proxy, ReturnValue );

		if (ReturnValue.Amount())
            qRGnr();

/*
	Despite the fact that this primitive does not need a return value,
	an empty string is returned in order for the back-end to wait
	that this primitive is achieved before launching another primitive.
	Without this, all other threads of the same back-end will be blocked
	until this primitive will return.
*/

		prtcl::Put( "", Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Confirm_(
		flw::rRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		str::wString ReturnValue;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Flow.Dismiss();

		Script.Init( "if ( window.confirm(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\") ) 'true'; else 'false';");

        ReturnValue.Init();
		Execute_( Script, Proxy, ReturnValue );

		prtcl::Put( ReturnValue, Flow );

		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}
}

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::eMode Mode )
{
qRH;
	qCBUFFERr Buffer;
	str::wString HostService;
qRB;
	HostService.Init();

	if ( sclmisc::OGetValue( ::registry::parameter::HostService, HostService ) )
		session::Core.Init( HostService.Convert( Buffer ), 0, sck::NoTimeout );

	faaspool::Initialize();
	session::LogDriver.Init( cio::COut );
qRR;
qRT;
qRE;
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveSession(void)
{
	session::rSession *Session = NULL;
qRH;
qRB;
	if ( ( Session = new session::rSession ) == NULL )
		qRAlc();

	if ( !Session->Init() ) {
		delete Session;
		Session = NULL;
	}

	// WARNING ! In 'MultiUser' mode, 'UpstreamCallback' is not correctly set yet!
/*	if ( Mode == xdhcmn::mMonoUser ) {
		Session->Launch( "", "" );
	}
*/
qRR;
	if ( Session != NULL )
		delete Session;

	Session = NULL;
qRT;
qRE;
	return Session;
}

const scli::sInfo &sclxdhtml::SCLXDHTMLInfo( void )
{
	return xdhqxdh::Info;
}

void sclxdhtml::SCLXDHTMLReleaseSession( xdhcmn::cSession *Session )
{
	if ( Session == NULL )
		qRGnr();

	delete Session;
}

qGCTOR( xdhqxdh )
{
}
