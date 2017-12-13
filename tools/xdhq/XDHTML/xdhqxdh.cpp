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

	namespace {
		void GetContents_(
			const str::dStrings &Ids,
			xdhdws::sProxy &Proxy,
			str::dStrings &Contents )
		{
		qRH;
			str::wString Content;
			sdr::sRow Row = qNIL;
		qRB;
			Row = Ids.First();

			while ( Row != qNIL ) {
				Content.Init();
				Proxy.GetValue( Ids( Row ), Content );

				Contents.Append( Content );

				Row = Ids.Next( Row );
			}
		qRR;
		qRT;
		qRE;
		}
	}

	void GetContents_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Contents;
	qRB;
		Ids.Init();
		prtcl::Get( Flow, Ids );

		Contents.Init();
		GetContents_( Ids, Proxy, Contents );

		prtcl::Put( Contents, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetContents__(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Contents;
		str::wString MergedIds, MergedContents;
	qRB;
		tol::Init( Ids, Contents );
		prtcl::Get( Flow, Ids );
		prtcl::Get( Flow, Contents );

		tol::Init( MergedIds, MergedContents );

		xdhcmn::FlatMerge( Ids, MergedIds, true );
		xdhcmn::FlatMerge( Contents, MergedContents, true );

		Proxy.SetContents( MergedIds, MergedContents );
	qRR;
	qRT;
	qRE;
	}

	void SetWidgets_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
	qRB;
		Id.Init();
		prtcl::Get( Flow, Id );

		Proxy.SetWidgets( Id );
	qRR;
	qRT;
	qRE;
	}

	void SetCasts_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, MergedTags, MergedValues;
		str::wStrings Tags, Values;
	qRB;
		tol::Init( Id, Tags, Values );

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Tags );
		prtcl::Get( Flow, Values );

		tol::Init( MergedTags, MergedValues );
		xdhcmn::FlatMerge( Tags, MergedTags, false );
		xdhcmn::FlatMerge( Values, MergedValues, true );

		Proxy.SetCasts( Id, MergedTags, MergedValues );
	qRR;
	qRT;
	qRE;
	}

	void GetAttribute_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Value.Init();
		Proxy.GetAttribute( Id, Name, Value );

		prtcl::Put( Value, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetAttribute_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name, Value );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );
		prtcl::Get( Flow, Value );

		Proxy.SetAttribute( Id, Name, Value );
	qRR;
	qRT;
	qRE;
	}


	void GetProperty_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Value.Init();
		Proxy.GetProperty( Id, Name, Value );

		prtcl::Put( Value, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetProperty_(
		flw::sRFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name, Value;
	qRB;
		tol::Init( Id, Name, Value );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );
		prtcl::Get( Flow, Value );

		Proxy.SetProperty( Id, Name, Value );
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

# define H( name )\
	case prtcl::a##name##_1:\
		name##_( Client_, *this );\
		break

			while( Continue )
				switch ( prtcl::GetAnswer( Client_ ) ) {
				case prtcl::aOK_1:
					Return = true;
				case prtcl::aError_1:
					Continue = false;
					Client_.Dismiss();
					break;
				H( SetLayout );
				H( GetContents );
				H( SetContents_ );
				H( SetWidgets );
				H( SetCasts );
				H( GetAttribute );
				H( SetAttribute );
				H( GetProperty );
				H( SetProperty );
				default:
					qRGnr();
					break;
			}

#undef H

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
