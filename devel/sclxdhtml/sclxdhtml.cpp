/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define SCLXDHTML__COMPILATION

#include "sclxdhtml.h"

#include "sclfrntnd.h"

using namespace sclxdhtml;

static bso::bool__ IsInitialized_ = false;

static const char *Launcher_ = NULL;

const sclrgstry::registry_ &sclxdhtml::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}

const char *sclxdhtml::GetLauncher( void )
{
	if ( Launcher_ == NULL )
		qRFwk();

	return Launcher_;
}

#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scllocale.cpp', d'o le 'namespace'.
namespace {

	typedef xdhcbk::downstream_callback__ _downstream_callback__;

	class donwstream_callback_implementation__
	: public _downstream_callback__
	{
	protected:
		virtual void XDHCBKInitialize( const xdhcbk::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.qRRor(), Data.SCLError(), Data.CIO(), Data.Localization() );
				SCLXDHTMLInitialization( Data.Mode() );
			}
		}
		virtual void XDHCBKBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcbk::session_callback__ *XDHCBKNew(
			const char *Language,
			xdhcbk::proxy_callback__ *ProxyCallback ) override
		{
			return SCLXDHTMLNew( Language, ProxyCallback );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			downstream_callback__::reset( P );
		}
		E_CVDTOR( donwstream_callback_implementation__ );
		void Init( void )
		{
			downstream_callback__::Init();
		}
	};
}

static inline void DoNothing_( void )
{}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XDHCBK_RETRIEVE_FUNCTION_NAME, xdhcbk::retrieve );

xdhcbk::downstream_callback__ *XDHCBKRetrieve( void )
{
	donwstream_callback_implementation__ *Callback = NULL;
qRFH
qRFB
	Callback = new donwstream_callback_implementation__;

	if ( Callback == NULL )
		qRAlc();

	Callback->Init();
qRFR
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
qRFT
qRFE(DoNothing_())
	return Callback;
}

namespace {
	void SetXML_(
		const ntvstr::string___ &Message,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_TOFLOW___ STOFlow;
		xml::writer Writer;
		str::string Buffer;
	qRB
		STOFlow.Init( XML );
		Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

		Buffer.Init();
		Writer.PutValue( Message.UTF8( Buffer ), "Content" );
	qRR
	qRT
	qRE
	}

	inline void SetXSL_( str::string_ &XSL )
	{
		XSL.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<xsl:stylesheet version=\"1.0\"\
			                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
				<xsl:output method=\"text\"\
					        encoding=\"utf-8\"/>\
				<xsl:template match=\"/\">\
					<xsl:value-of select=\"Content\"/>\
				</xsl:template>\
			</xsl:stylesheet>\
		");
	}

	inline void SetXMLAndXSL_(
		const ntvstr::string___ &Message,
		str::string_ &XML,
		str::string_ &XSL )
	{
		SetXML_( Message, XML );
		SetXSL_( XSL );
	}

}

void sclxdhtml::Alert(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	proxy__ &Proxy,
	const char *Language )
{
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	scllocale::GetTranslation( SCLXDHTML_NAME "_CloseText", Language, CloseText );

	Proxy.Alert( XML, XSL, Title, CloseText );
qRR
qRT
qRE
}

void sclxdhtml::Alert(
	const ntvstr::string___ &Message,
	const ntvstr::string___ &CloseText,
	proxy__ &Proxy )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, XML, XSL );

	Proxy.Alert(XML, XSL, ntvstr::string___(), CloseText );
qRR
qRT
qRE
}

void sclxdhtml::Alert(
	const ntvstr::string___ &Message,
	proxy__ &Proxy,
	const char *Language )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, XML, XSL );

	Alert( XML, XSL, ntvstr::string___(), Proxy, Language );
qRR
qRT
qRE
}


bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	proxy__ &Proxy,
	const char *Language )
{
	bso::bool__ Confirmation = false;
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	scllocale::GetTranslation( SCLXDHTML_NAME "_CloseText", Language, CloseText );

	Confirmation = Proxy.Confirm( XML, XSL, Title, CloseText );
qRR
qRT
qRE
	return Confirmation;
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &Message,
	const ntvstr::string___ &CloseText,
	proxy__ &Proxy )
{
	bso::bool__ Confirmation = false;
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, XML, XSL );

	Confirmation = Proxy.Confirm( XML, XSL, ntvstr::string___(), CloseText );
qRR
qRT
qRE
	return Confirmation;
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &Message,
	proxy__ &Proxy,
	const char *Language )
{
	bso::bool__ Confirmation = false;
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, XML, XSL );

	Confirmation = Confirm( XML, XSL, ntvstr::string___(), Proxy, Language );
qRR
qRT
qRE
	return Confirmation;
}



void sclxdhtml::HandleError(
	proxy__ &Proxy,
	const char *Language )
{
qRH
	str::string Message;
	err::buffer__ ErrBuffer;
qRB
	switch ( ERRType ) {
	case err::t_Abort:
		Message.Init();
		if ( sclerror::GetPendingErrorTranslation( Language, Message, err::hUserDefined ) ) {
			sclerror::ResetPendingError();
			Alert( Message, Proxy, Language );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		Alert( "???", Proxy, Language );
		break;
	default:
		Alert( err::Message( ErrBuffer ), Proxy, Language );
		break;
	}

	ERRRst();
qRR
qRT
qRE
}


#if 0
bso::bool__ sclxhtml::session___::XDHCBKXDHCBKLaunch(
	const char *Id,
	const char *Action )
{
	bso::bool__ Success = false;
qRH
	str::string Message;
	err::buffer__ ErrBuffer;
qRB
		if ( _OnBeforeAction( Id, Action ) )
			if ( !strcmp( Action, xdhcbk::CloseActionLabel ) )
				Success = _OnClose();	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
			else
				Success = _Handler.Launch( Id, Action );
qRR
#if 0
	switch ( ERRType ) {
	case err::t_Abort:
		Message.Init();
		if ( sclerror::GetPendingErrorTranslation( _L(), Message, err::hUserDefined ) ) {
			sclerror::ResetPendingError();
			_A().RawAlert( Message );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		_A().RawAlert( "???" );
		break;
	default:
		_A().RawAlert( err::Message( ErrBuffer ) );
		break;
	}

	ERRRst();
# endif
qRT
qRE
	return Success;
}
#endif

void sclxdhtml::LoadProject( proxy__ &Proxy )
{
qRH
	str::string ProjectFeature;
qRB
	ProjectFeature.Init();
	sclfrntnd::LoadProject( xdhdws::prolog::GetProjectFeatures( Proxy, ProjectFeature ), ProjectFeature );
qRR
qRT
qRE
}

void sclxdhtml::LaunchProject(
	 proxy__ &Proxy,
	sclfrntnd::kernel___ &Kernel )
{
qRH
	str::string BackendFeature;
qRB
	BackendFeature.Init();
	sclfrntnd::LaunchProject( Kernel, xdhdws::login::GetBackendFeatures( Proxy, BackendFeature ), BackendFeature );
qRR
qRT
qRE
}

