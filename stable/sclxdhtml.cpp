/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

using namespace sclxdhtml;

namespace registry_ {
	using rgstry::rEntry;

	namespace definition_ {
		namespace {
			namespace {
				rEntry XSLFilesSet_( "XSLFilesSet", sclrgstry::Definitions );
			}

			rEntry FreeXSLFiles_( "XSLFiles", XSLFilesSet_ );
		}
		rEntry TaggedXSLFiles( RGSTRY_TAGGING_ATTRIBUTE( "target" ), FreeXSLFiles_ );
	}
}

rgstry::rEntry registry::definition::XSLLayoutFile( "Layout", registry_::definition_::TaggedXSLFiles );
rgstry::rEntry registry::definition::XSLCastingFile( "Casting", registry_::definition_::TaggedXSLFiles );

namespace {
	E_CDEF(char *, StraightBackendType_, "Straight" );
	E_CDEF(char *, EmbeddedBackendType_, "Embedded" );
}

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

	typedef xdhcmn::downstream_callback__ _downstream_callback__;

	class donwstream_callback_implementation__
	: public _downstream_callback__
	{
	protected:
		virtual void XDHCMNInitialize( const xdhcmn::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.SCLRack(), Data.Localization() );

				SCLXDHTMLInitialization( Data.Mode() );
			}
		}
		virtual void XDHCMNBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcmn::session_callback__ *XDHCMNRetrieveCallback(
			const char *Language,
			xdhcmn::proxy_callback__ *ProxyCallback ) override
		{
			return SCLXDHTMLRetrieveCallback( Language, ProxyCallback );
		}
		virtual void XDHCMNReleaseCallback( xdhcmn::session_callback__ *Callback ) override
		{
			return SCLXDHTMLReleaseCallback( Callback );
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

DEF( XDHCMN_RETRIEVE_FUNCTION_NAME, xdhcmn::retrieve );

xdhcmn::downstream_callback__ *XDHCMNRetrieve( void )
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
		const str::string_  &Message,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_TOFLOW___ STOFlow;
		xml::writer Writer;
	qRB
		STOFlow.Init( XML );
		Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

		Writer.PutValue( Message, "Content" );
	qRR
	qRT
	qRE
	}

	void SetXML_(
		const str::string_  &RawMessage,
		const char *Language,
		str::string_ &XML )
	{
	qRH
		str::string Message;
		TOL_CBUFFER___ Buffer;
	qRB
		Message.Init();
		scllocale::GetTranslation( RawMessage.Convert( Buffer ), Language, Message );

		SetXML_( Message, XML );
	qRR
	qRT
	qRE
	}

	inline void SetXSL_( str::string_ &XSL )
	{
		XSL.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<xsl:stylesheet version=\"1.0\"\
			                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
				<xsl:output method=\"html\"\
					        encoding=\"utf-8\"/>\
				<xsl:template match=\"/\">\
					<xsl:value-of select=\"Content\"/>\
				</xsl:template>\
			</xsl:stylesheet>\
		");
	}

	inline void SetXMLAndXSL_(
		const ntvstr::string___ &Message,
		const char *Language,
		str::string_ &XML,
		str::string_ &XSL )
	{
	qRH
		str::string Buffer;
	qRB
		Buffer.Init();

		if ( Language == NULL )
			SetXML_( Message.UTF8( Buffer ), XML );
		else
			SetXML_( Message.UTF8( Buffer ), Language, XML );

		SetXSL_( XSL );
	qRR
	qRT
	qRE
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

namespace{
	void Alert_(
		const ntvstr::string___ &Message,
		const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
		proxy__ &Proxy,
		const char *CloseTextLanguage )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		XSL.Init();

		SetXMLAndXSL_( Message, MessageLanguage, XML, XSL );

		Alert(XML, XSL, ntvstr::string___(), Proxy, CloseTextLanguage );
	qRR
	qRT
	qRE
	}
}

void sclxdhtml::Alert(
	const ntvstr::string___ &RawMessage,
	const char *Language,
	proxy__ &Proxy )
{
	Alert_( RawMessage, Language, Proxy, Language );
}

void sclxdhtml::Alert(
	const ntvstr::string___ &Message,
	proxy__ &Proxy,
	const char *Language )
{
	Alert_( Message, NULL, Proxy, Language );
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

namespace {
	bso::bool__ Confirm_(
		const ntvstr::string___ &Message,
		const char *MessageLanguage,	// If != 'NULL', 'Message' is translates, otherwise it is displayed as is.
		proxy__ &Proxy,
		const char *CloseTextLanguage )
	{
		bso::bool__ Confirmation = false;
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		XSL.Init();

		SetXMLAndXSL_( Message, MessageLanguage, XML, XSL );

		Confirmation = Confirm( XML, XSL, ntvstr::string___(), Proxy, CloseTextLanguage );
	qRR
	qRT
	qRE
		return Confirmation;
	}
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &RawMessage,
	const char *Language,
	proxy__ &Proxy )
{
	return Confirm_( RawMessage,  Language, Proxy, Language );
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &Message,
	proxy__ &Proxy,
	const char *Language )
{
	return Confirm_( Message,  NULL, Proxy, Language );
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

void sclxdhtml::SetElement_(
	const xdhdws::nstring___ &Id,
	sProxyDocumentFunction Function,
	const rgstry::rEntry & Filename,
	const char *Target,
	const sclrgstry::registry_ &Registry,
	xdhdws::rGenericRack &Rack,
	xdhdws::proxy__ &Proxy, bso::char__ Marker)
{
qRH
	str::wString XSL;
qRB
	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( Filename, Target ), Registry, XSL, Marker );

	Function( Proxy, Id, Rack.Target(), XSL);
qRR
qRT
qRE
}

qCDEF( char *, sclxdhtml::RootTagName_, "Root" );


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
			if ( !strcmp( Action, xdhcmn::CloseActionLabel ) )
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
/*
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

*/
void sclxdhtml::prolog::GetLayout(
	sclfrntnd::rFrontend &Frontend,
	xml::writer_ &Writer)
{
	sclfrntnd::GetProjectsFeatures( Frontend.Language(), Writer );
}

static sclmisc::project_type__ GetProjectType_( proxy__ &Proxy )
{
	sclmisc::project_type__ ProjectType = sclmisc::pt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	ProjectType = sclmisc::GetProjectType( Proxy.GetContent( prolog::ProjectTypeId, Value ) );
qRR
qRT
qRE
	return ProjectType;
}

void sclxdhtml::prolog::GetCasting(
	proxy__ &Proxy,
	xml::writer_ &Writer)
{
	Writer.PushTag( "ProjectType ");

	Writer.PutValue( sclmisc::GetLabel( GetProjectType_( Proxy ) ) );

	Writer.PopTag();
}

void sclxdhtml::prolog::DisplaySelectedProjectFilename(
	proxy__ &Proxy,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	Args.Init();
	xdhcmn::Split( str::string( Proxy.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetContent( RemoteProjectId, FileName );
qRR
qRT
qRE
}

sclmisc::project_type__ sclxdhtml::prolog::GetProjectFeatures(
	proxy__ &Proxy,
	str::string_ &Feature )
{
	sclmisc::project_type__ Type = sclmisc::pt_Undefined;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	switch ( Type = GetProjectType_( Proxy ) ) {
	case sclmisc::ptNew:
		break;
	case sclmisc::ptRemote:
		Feature.Append( Proxy.GetContent( RemoteProjectId, Buffer ) );
		break;
	case sclmisc::ptEmbedded:
		qRVct();	// Not implemented yet.
		break;
	case sclmisc::ptPredefined:
		Feature.Append( Proxy.GetContent( PredefinedProjectId, Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Type;
}

void sclxdhtml::prolog::LoadProject( proxy__ &Proxy )
{
qRH
	str::string ProjectFeature;
qRB
	ProjectFeature.Init();
	sclfrntnd::LoadProject( prolog::GetProjectFeatures( Proxy, ProjectFeature ), ProjectFeature );
qRR
qRT
qRE
}

#define C( name ) case bv##name : return #name

const char *sclxdhtml::login::GetLabel( eBackendVisibility Visibility )
{
	switch ( Visibility ) {
	C( Show );
	C( Hide );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}


sclfrntnd::eLogin sclxdhtml::login::GetLayout(
	sclfrntnd::rFrontend &Frontend,
	xml::writer_ &Writer)
{
	sclfrntnd::GetBackendsFeatures( Frontend.Language(), Writer );

	return sclfrntnd::GetLoginFeatures( Writer );
}

namespace {
	const str::dString &GetBackendType_(
		proxy__ &Proxy,
		str::dString &Type )
	{
		return Proxy.GetContent( login::BackendTypeId, Type );
	}
}

void sclxdhtml::login::GetCasting(
	proxy__ &Proxy,
	eBackendVisibility Visibility,
	xml::writer_ &Writer )
{
qRH
	str::wString Type;
qRB
	Writer.PushTag( "Backend" );

	Type.Init();
	Writer.PutAttribute( "Type", GetBackendType_( Proxy, Type ) );

	Writer.PutAttribute( "Visibility", GetLabel( Visibility ) );

	Writer.PopTag();
qRR
qRT
qRE
}

namespace straight_ {
	namespace {
		rgstry::rEntry DefaultPort_("@DefaultStraightPort", sclfrntnd::BackendParametersRegistryEntry );
	}

	void Normalize( str::dString &Parameters )
	{
	qRH
		str::wString Port;
	qRB
		if ( Parameters.Search( ':' ) == qNIL ) {
			Port.Init();
			if ( sclmisc::OGetValue( DefaultPort_, Port ) ) {
				Parameters.Append( ':' );
				Parameters.Append( Port );
			}
		}
	qRR
	qRT
	qRE
	}
}

void sclxdhtml::login::GetBackendFeatures(
	proxy__ &Proxy,
	sclfrntnd::rFeatures &Features )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string Type, Parameters;
	const char *BackendId = NULL;
	bso::sBool NormalizeStraightBackendFeature = false;
qRB
	Parameters.Init();

	Type.Init();
	Type = GetBackendType_( Proxy, Type );

	if ( Type != sclfrntnd::NoneBackendType ) {
		if ( Type == sclfrntnd::PredefinedBackendType )
			BackendId = PredefinedBackendId;
		else if ( Type == EmbeddedBackendType_ )
			BackendId = EmbeddedBackendId;
		else if ( Type == StraightBackendType_ ) {
			NormalizeStraightBackendFeature = true;
			BackendId = StraightBackendId;
		} else
			qRGnr();

		Parameters.Append( Proxy.GetContent( BackendId, Buffer ) );

		if ( NormalizeStraightBackendFeature )
			straight_::Normalize( Parameters );
	}

	sclfrntnd::SetBackendFeatures( Type, Parameters, Features );
qRR
qRT
qRE
}

void sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename(
	proxy__ &Proxy,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	Args.Init();
	xdhcmn::Split( str::string( Proxy.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetContent( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}

