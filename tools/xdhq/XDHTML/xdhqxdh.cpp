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

}

void sclxdhtml::SCLXDHTMLInitialization( xdhcmn::eMode Mode )
{
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

		Proxy.SetLayout( Id, XML, XSL );
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

		Proxy.SetCasting( Id, XML, XSL );
	qRR;
	qRT;
	qRE;
	}

	void GetContent_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Content;
	qRB;
		Id.Init();
		prtcl::Get( Flow, Id );

		Content.Init();
		Proxy.GetValue( Id, Content );

		prtcl::Put( Content, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetContent_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Content;
		str::wStrings Ids, Contents;
	qRB;
		tol::Init( Ids, Contents );
		prtcl::Get( Flow, Ids );
		prtcl::Get( Flow, Contents );

		tol::Init( Id, Content );

		xdhcmn::FlatMerge( Ids, Id, true );
		xdhcmn::FlatMerge( Contents, Content, true );

		Proxy.SetContents( Id, Content );
	qRR;
	qRT;
	qRE;
	}

	class rSession_
	: public xdhcmn::cSession,
	  public xdhdws::sProxy
	{
	private:
		csdbnc::rIOFlow Client_;
		bso::sBool NotFirstCall_;
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override
		{
			bso::sBool Return = false;
			bso::sBool Continue = true;

			if ( !NotFirstCall_ ) {
				if ( prtcl::GetAnswer( Client_ ) != prtcl::aOK_1 )
					qRGnr();

				Client_.Dismiss();

				NotFirstCall_ = true;
			}

			prtcl::PutRequest( prtcl::rLaunch_1, Client_ );

			prtcl::Put( Id, Client_ );
			prtcl::Put( Action, Client_ );
			Client_.Commit();

			while( Continue )
				switch ( prtcl::GetAnswer( Client_ ) ) {
				case prtcl::aOK_1:
					Return = true;
				case prtcl::aError_1:
					Continue = false;
					Client_.Dismiss();
					break;
				case prtcl::aSetLayout_1:
					SetLayout_( Client_, *this );
					break;
				case prtcl::aSetCasting_1:
					SetCasting_( Client_, *this );
					break;
				case prtcl::aGetContent_1:
					GetContent_( Client_, *this );
					break;
				case prtcl::aSetContent_1:
					SetContent_( Client_, *this );
					break;
				default:
					qRGnr();
					break;
			}

			return Return;
		}
	public:
		void reset( bso::sBool P = true )
		{
			xdhdws::sProxy::reset( P );
			NotFirstCall_ = false;
		}
		qCVDTOR( rSession_ )
		void Init(
			xdhcmn::cProxy *Callback,
			const char *Language )
		{
		qRH;
			qCBUFFERr Buffer;
			str::wString HostService;
		qRB;
			HostService.Init();
			sclmisc::MGetValue( ::registry::parameter::HostService, HostService );

			Client_.Init( HostService.Convert( Buffer ), sck::NoTimeout, err::h_Default );

			csdcmn::SendProtocol( prtcl::ProtocolId, prtcl::ProtocolVersion, Client_ );

			prtcl::Put( Language, Client_ );

			Client_.Commit();

			xdhdws::sProxy::Init( Callback );

			NotFirstCall_ = false;
		qRR;
		qRT;
		qRE;
		}
		operator flw::sIOFlow &( void )
		{
			return Client_;
		}
		flw::sIOFlow &Client( void )
		{
			return Client_;
		}
	};
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcmn::eMode Mode,
	xdhcmn::cProxy *ProxyCallback )
{
	rSession_ *Session = new rSession_;
	bso::sBool Continue = true;

	if ( Session == NULL )
		qRGnr();

	Session->Init( ProxyCallback, Language );

	// WARNING ! In 'MultiUser' mode, 'ProxyCallback' is not correctly set yet!
	if ( Mode == xdhcmn::mMonoUser ) {
		Session->Launch( "", "" );
	}

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
