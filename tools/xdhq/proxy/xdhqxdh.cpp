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

#include "dmopool.h"
#include "registry.h"

#include "prtcl.h"

#include "csdmnc.h"
#include "csdcmn.h"
#include "csdbns.h"

#include "sclmisc.h"

SCLI_DEF( xdhqxdh, PROGRAM_NAME, SOFTWARE_NAME );

namespace {
	csdmnc::rCore Core_;
}

namespace {

	stsfsm::wAutomat CommandAutomat_;

	qENUM( Command_ )
	{
		cStandBy_1,	// Send as command to report that there is no more command to handle.
		cExecute_1,
		cAlert_1,
		cConfirm_1,
		cSetLayout_1,
		cGetContents_1,
		cSetContents_1,
		cDressWidgets_1,
		cAddClasses_1,
		cRemoveClasses_1,
		cToggleClasses_1,
		cEnableElements_1,
		cDisableElements_1,
		cSetAttribute_1,
		cGetAttribute_1,
		cRemoveAttribute_1,
		cSetProperty_1,
		cGetProperty_1,
		cFocus_1,
		c_amount,
		c_Undefined
	};

	#define C( name ) case c##name : return #name ; break

	const char *GetLabel_( eCommand_ Command )
	{
		switch ( Command ) {
			case cStandBy_1:
				return prtcl::StandBy;
				break;
			C( Execute_1 );
			C( Alert_1 );
			C( Confirm_1 );
			C( SetLayout_1 );
			C( GetContents_1 );
			C( SetContents_1 );
			C( DressWidgets_1 );
			C( AddClasses_1 );
			C( RemoveClasses_1 );
			C( ToggleClasses_1 );
			C( EnableElements_1 );
			C( DisableElements_1 );
			C( SetAttribute_1 );
			C( GetAttribute_1 );
			C( RemoveAttribute_1 );
			C( SetProperty_1 );
			C( GetProperty_1 );
			C( Focus_1 );
		default:
			qRFwk();
			break;
		}

		return NULL; // To avoid a 'warning'.
	}

#undef C

	void FillCommandAutomat_( void )
	{
		CommandAutomat_.Init();
		stsfsm::Fill<eCommand_>( CommandAutomat_, c_amount, GetLabel_ );
	}

	eCommand_ GetCommand_( flw::iflow__ &Flow )
	{
		return stsfsm::GetId( Flow, CommandAutomat_, c_Undefined, c_amount );
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
		Core_.Init( HostService.Convert( Buffer ), 0, sck::NoTimeout );

	dmopool::Initialize();
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

	void Execute_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Script;
		qCBUFFERr Buffer;
	qRB;
		Script.Init();
		prtcl::Get( Flow, Script );

		Proxy.Execute( Script, Buffer );

		Flow.Dismiss();

		prtcl::Put( (char *)Buffer(), Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Alert_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		qCBUFFERr Buffer;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Script.Init( "window.alert(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\");'';");

		Proxy.Execute( Script, Buffer );
	qRR;
	qRT;
	qRE;
	}

	void Confirm_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Message, Script;
		qCBUFFERr Buffer;
	qRB;
		Message.Init();
		prtcl::Get( Flow, Message );

		Script.Init( "if ( window.confirm(\"");
		xdhcmn::Escape( Message, Script, '"' );
		Script.Append( "\") ) 'true'; else 'false';");

		Proxy.Execute( Script, Buffer );

		Flow.Dismiss();

		prtcl::Put( (char *)Buffer(), Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetLayout_(
		flw::sRWFlow &Flow,
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

		Flow.Dismiss();

		prtcl::Put( Contents, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void SetContents_(
		flw::sRWFlow &Flow,
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

	void DressWidgets_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
	qRB;
		Id.Init();
		prtcl::Get( Flow, Id );

		Proxy.DressWidgets( Id );
	qRR;
	qRT;
	qRE;
	}

	namespace {
		void HandleClasses_(
			flw::sRWFlow &Flow,
			void (xdhdws::sProxy::* Method)(
				const xdhdws::rNString &Ids,
				const xdhdws::rNString &Classes),
			xdhdws::sProxy &Proxy )
		{
		qRH;
			str::wStrings Ids, Classes;
			str::wString MergedIds, MergedClasses;
		qRB;
			tol::Init( Ids, Classes );

			prtcl::Get( Flow, Ids );
			prtcl::Get( Flow, Classes );

			tol::Init( MergedIds, MergedClasses );
			xdhcmn::FlatMerge( Ids, MergedIds, true );
			xdhcmn::FlatMerge( Classes, MergedClasses, true );

			(Proxy.*Method)( MergedIds, MergedClasses );
		qRR;
		qRT;
		qRE;
		}
	}

	void AddClasses_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::AddClasses, Proxy );
	}

	void RemoveClasses_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::RemoveClasses, Proxy );
	}

	void ToggleClasses_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleClasses_( Flow, &xdhdws::sProxy::ToggleClasses, Proxy );
	}

	namespace {
		void HandleElements_(
			flw::sRWFlow &Flow,
			void (xdhdws::sProxy::* Method)( const xdhdws::rNString &Ids ),
			xdhdws::sProxy &Proxy )
		{
		qRH;
			str::wStrings Ids;
			str::wString MergedIds;
		qRB;
			tol::Init( Ids );

			prtcl::Get( Flow, Ids );

			tol::Init( MergedIds );
			xdhcmn::FlatMerge( Ids, MergedIds, true );

			(Proxy.*Method)( MergedIds );
		qRR;
		qRT;
		qRE;
		}
	}

	void EnableElements_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleElements_( Flow, &xdhdws::sProxy::EnableElements, Proxy );
	}

	void DisableElements_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
		HandleElements_( Flow, &xdhdws::sProxy::DisableElements, Proxy );
	}

	void SetAttribute_(
		flw::sRWFlow &Flow,
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

		Flow.Dismiss();

		prtcl::Put( Value, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void RemoveAttribute_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id, Name;
	qRB;
		tol::Init( Id, Name );
		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Name );

		Proxy.RemoveAttribute( Id, Name );
	qRR;
	qRT;
	qRE;
	}

	void SetProperty_(
		flw::sRWFlow &Flow,
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

		Flow.Dismiss();

		prtcl::Put( Value, Flow );
		Flow.Commit();
	qRR;
	qRT;
	qRE;
	}

	void Focus_(
		flw::sRWFlow &Flow,
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString Id;
	qRB;
		tol::Init( Id );
		prtcl::Get( Flow, Id );

		Proxy.Focus( Id );
	qRR;
	qRT;
	qRE;
	}

	qENUM( Mode_ ) {
		mDemo,	// Demo mode, direct connexion (not muxed) through the easy to install component.
		mProd,	// Production mode, muxed connexion through the native component.
		m_amount,
		m_Undefined
	};

	class rSession_
	: public xdhcmn::cSession,
	  public xdhdws::sProxy
	{
	private:
		eMode_ Mode_;
		sck::rRWDriver DemoDriver_;
		csdmnc::rRWDriver ProdDriver_;
		fdr::rRWDriver &D_( void )
		{
			switch ( Mode_ ) {
			case mDemo:
				return DemoDriver_;
				break;
			case mProd:
				return ProdDriver_;
				break;
			default:
				qRGnr();
				break;
			}

			return *(fdr::rRWDriver *)NULL;	// To avoid a warning.
		}
	protected:
		virtual bso::bool__ XDHCMNLaunch(
			const char *Id,
			const char *Action ) override
		{
			bso::sBool Return = false;
		qRH;
			bso::sBool Continue = true;
			flw::sDressedRWFlow<> Flow;
		qRB;
			Flow.Init( D_() );

			prtcl::Put( Id, Flow );
			prtcl::Put( Action, Flow );
			Flow.Commit();

# define H( name )\
	case c##name##_1:\
		::name##_( Flow, *this );\
		break

			while( Continue )
				switch ( GetCommand_( Flow ) ) {
				case cStandBy_1:
					Return = true;
					Continue = false;
					Flow.Dismiss();
					break;
				H( Execute );
				H( Alert );
				H( Confirm );
				H( SetLayout );
				H( GetContents );
				H( SetContents );
				H( DressWidgets );
				H( AddClasses );
				H( RemoveClasses );
				H( ToggleClasses );
				H( EnableElements );
				H( DisableElements );
				H( SetAttribute );
				H( GetAttribute );
				H( RemoveAttribute );
				H( SetProperty );
				H( GetProperty );
				H( Focus );
				default:
					qRGnr();
					break;
			}

#undef H

		qRR;
		qRT;
		qRE;
			return Return;
	}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, DemoDriver_, ProdDriver_ );
			Mode_ = m_Undefined;
			xdhdws::sProxy::reset( P );
		}
		qCVDTOR( rSession_ )
		void Init(
			xdhcmn::cProxy *Callback,
			const char *Language,
			const str::dString &Token )	// If empty, PROD session, else token used for the DEMO session.
		{
		qRH;
			flw::sDressedRWFlow<> Flow;
			csdcmn::sVersion Version = csdcmn::UndefinedVersion;
		qRB;
			tol::reset( DemoDriver_, ProdDriver_ );
			Mode_ = m_Undefined;

			if ( Token.Amount() == 0 ) {
				ProdDriver_.Init( Core_, fdr::ts_Default );
				Mode_ = mProd;
			} else {
				DemoDriver_.Init( dmopool::GetConnection( Token ), true, fdr::ts_Default );
				Mode_ = mDemo;
			}

			Flow.Init( D_() );

			prtcl::Put( Language, Flow );

			Flow.Commit();

			if ( (Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow )) != prtcl::ProtocolVersion )
				qRGnr();

			xdhdws::sProxy::Init( Callback );
		qRR;
		qRT;
		qRE;
		}
		operator fdr::rRWDriver &( void )
		{
			return D_();
		}
		fdr::rRWDriver &Driver( void )
		{
			return D_();
		}
	};
}

xdhcmn::cSession *sclxdhtml::SCLXDHTMLRetrieveCallback(
	const char *Language,
	xdhcmn::eMode Mode,
	const str::dString &Token,
	xdhcmn::cProxy *ProxyCallback )
{
	rSession_ *Session = new rSession_;
	bso::sBool Continue = true;

	if ( Session == NULL )
		qRGnr();

	Session->Init( ProxyCallback, Language, Token );

	// WARNING ! In 'MultiUser' mode, 'ProxyCallback' is not correctly set yet!
/*	if ( Mode == xdhcmn::mMonoUser ) {
		Session->Launch( "", "" );
	}
*/
	return Session;
}

const scli::sInfo &sclxdhtml::SCLXDHTMLInfo( void )
{
	return xdhqxdh::Info;
}

void sclxdhtml::SCLXDHTMLReleaseCallback( xdhcmn::cSession *Callback )
{
	if ( Callback == NULL )
		qRGnr();

	delete Callback;
}

qGCTOR( xdhqxdh )
{
	FillCommandAutomat_();
}
