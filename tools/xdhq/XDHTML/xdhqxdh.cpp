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

#include "XDHqxdh.h"

#include "registry.h"

#include "prtcl.h"

#include "csdbnc.h"
#include "csdcmn.h"

#include "sclmisc.h"

namespace {
	csdbnc::rIOFlow Client_;
}

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::mode__ Mode )
{
qRH;
	qCBUFFERr Buffer;
	str::wString HostService;
qRB;
	HostService.Init();
	sclmisc::MGetValue( ::registry::parameter::HostService, HostService );

	Client_.Init( HostService.Convert( Buffer ), sck::NoTimeout, err::h_Default );

	csdcmn::SendProtocol( prtcl::ProtocolId, prtcl::ProtocolVersion, Client_ );

	Client_.Commit();
qRR;
qRT;
qRE;
}
 
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

	void SetLayout_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, XML, XSL;
	qRB;
		tol::Init( Id, XML, XSL );

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, XML );
		prtcl::Get( Flow, XSL );
		Flow.Dismiss();

		Proxy.SetLayout( Id, XML, XSL, NULL );
	qRR;
	qRT;
	qRE;
	}

	void SetCasting_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, XML, XSL;
	qRB;
		tol::Init( Id, XML, XSL );

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, XML );
		prtcl::Get( Flow, XSL );
		Flow.Dismiss();

		Proxy.SetCasting( Id, XML, XSL );
	qRR;
	qRT;
	qRE;
	}

	class rSession_
	: public xdhcmn::cSession,
	  public xdhdws::sProxy
	{
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override
		{
			prtcl::PutRequest( prtcl::rLaunch_1, Client_ );

			prtcl::Put( Id, Client_ );
			prtcl::Put( Action, Client_ );
			Client_.Commit();

			while( true  )
				switch ( prtcl::GetAnswer( Client_ ) ) {
				case prtcl::aOK_1:
					return true;
					break;
				case prtcl::aError_1:
					return false;
					break;
				case prtcl::aSetLayout_1:
					SetLayout_( Client_, *this );
					Client_.Commit();
					break;
				case prtcl::aSetCasting_1:
					SetCasting_( Client_, *this );
					Client_.Commit();
					break;
				default:
					qRGnr();
					break;
			}

			return false;	// To avoid a warning;
		}
	public:
		void reset( bso::sBool P = true )
		{
			xdhdws::sProxy::reset( P );
		}
		qCVDTOR( rSession_ )
		void Init( xdhcmn::cProxy *Callback )
		{
			xdhdws::sProxy::Init( Callback );
		}
	};
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcmn::cProxy *ProxyCallback )
{
	rSession_ *Session = new rSession_;

	if ( Session == NULL )
		qRGnr();

	Session->Init( ProxyCallback );

	if ( prtcl::GetAnswer( Client_ ) != prtcl::aSetLayout_1 )
		qRGnr();

	SetLayout_( Client_, *Session );

	if ( prtcl::GetAnswer( Client_ ) != prtcl::aOK_1 )
		qRGnr();

	Client_.Dismiss();

	return Session;
}

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

const char *sclmisc::SCLMISCTargetName = PROGRAM_NAME;
const char *sclmisc::SCLMISCProductName = SOFTWARE_NAME;
