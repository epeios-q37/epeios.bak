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

namespace {
	using rgstry::rEntry;

	namespace {
		namespace {
			rEntry XSLFiles_( "XSLFiles", sclrgstry::Definitions );
		}
		rEntry UntaggedXSLFile_( "XSLFile", XSLFiles_ );
	}
}

rgstry::rEntry registry::definition::XSLFile( RGSTRY_TAGGING_ATTRIBUTE( "target" ), UntaggedXSLFile_ );

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
# define FUNCTION_SPEC
#endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scllocale.cpp', d'o le 'namespace'.
namespace {

	typedef xdhcmn::cDownstream cDownstream_;

	class sDownstream
	: public cDownstream_
	{
	private:
		xdhcmn::eMode Mode_;
	protected:
		virtual void XDHCMNInitialize( const xdhcmn::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			Mode_ = Data.Mode();

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
		virtual xdhcmn::cSession *XDHCMNRetrieveCallback(
			const char *Language,
			xdhcmn::cProxy *ProxyCallback ) override
		{
			return SCLXDHTMLRetrieveCallback( Language, Mode_, ProxyCallback );
		}
		virtual void XDHCMNReleaseCallback( xdhcmn::cSession *Callback ) override
		{
			return SCLXDHTMLReleaseCallback( Callback );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Mode_ = xdhcmn::m_Undefined;
		}
		E_CVDTOR( sDownstream );
		void Init( void )
		{
			Mode_ = xdhcmn::m_Undefined;
		}
	};
}

static inline void DoNothing_( void )
{}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XDHCMN_RETRIEVE_FUNCTION_NAME, xdhcmn::retrieve );

xdhcmn::cDownstream *XDHCMNRetrieve( void )
{
	sDownstream *Callback = NULL;
qRFH
qRFB
	Callback = new sDownstream;

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

void sclxdhtml::sProxy::Alert_(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	const char *Language )
{
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	scllocale::GetTranslation( SCLXDHTML_NAME "_CloseText", Language, CloseText );

	Core_.Alert( XML, XSL, Title, CloseText );
qRR
qRT
qRE
}

void sclxdhtml::sProxy::Alert_(
	const ntvstr::string___ &Message,
	const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
	const char *CloseTextLanguage )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, MessageLanguage, XML, XSL );

	Alert_(XML, XSL, ntvstr::string___(), CloseTextLanguage );
qRR
qRT
qRE
}

void sclxdhtml::sProxy::AlertT(
	const ntvstr::string___ &RawMessage,
	const char *Language )
{
	Alert_( RawMessage, Language, Language );
}

void sclxdhtml::sProxy::AlertU(
	const ntvstr::string___ &Message,
	const char *Language )
{
	Alert_( Message, NULL, Language );
}

bso::bool__ sclxdhtml::sProxy::Confirm_(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	const char *Language )
{
	bso::bool__ Confirmation = false;
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	scllocale::GetTranslation( SCLXDHTML_NAME "_CloseText", Language, CloseText );

	Confirmation = Core_.Confirm( XML, XSL, Title, CloseText );
qRR
qRT
qRE
	return Confirmation;
}

bso::bool__ sclxdhtml::sProxy::Confirm_(
	const ntvstr::string___ &Message,
	const char *MessageLanguage,	// If != 'NULL', 'Message' is translates, otherwise it is displayed as is.
	const char *CloseTextLanguage )
{
	bso::bool__ Confirmation = false;
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	XSL.Init();

	SetXMLAndXSL_( Message, MessageLanguage, XML, XSL );

	Confirmation = Confirm_( XML, XSL, ntvstr::string___(), CloseTextLanguage );
qRR
qRT
qRE
	return Confirmation;
}


bso::bool__ sclxdhtml::sProxy::ConfirmT(
	const ntvstr::string___ &RawMessage,
	const char *Language )
{
	return Confirm_( RawMessage, Language, Language );
}

bso::bool__ sclxdhtml::sProxy::ConfirmU(
	const ntvstr::string___ &Message,
	const char *Language )
{
	return Confirm_( Message,  NULL, Language );
}

void sclxdhtml::HandleError(
	sProxy &Proxy,
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
			Proxy.AlertU( Message, Language );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		Proxy.AlertU( "???", Language );
		break;
	default:
		Proxy.AlertU( err::Message( ErrBuffer ), Language );
		break;
	}

	ERRRst();
qRR
qRT
qRE
}

void sclxdhtml::sProxy::SetLayout_(
	const xdhdws::nstring___ &Id,
	const rgstry::rEntry & Filename,
	const char *Target,
	const sclrgstry::registry_ &Registry,
	const str::dString &XML,
	bso::char__ Marker )
{
qRH
	str::wString XSL;
qRB
	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( Filename, Target ), Registry, XSL, Marker );

	Core_.SetLayout( Id, XML, XSL );
qRR
qRT
qRE
}

void sclxdhtml::sProxy::SetContents(
	const str::dStrings &Ids,
	const str::dStrings &Contents )
{
qRH;
	str::wString MergedIds, MergedContents;
qRB;
	if ( Ids.Amount() != Contents.Amount() )
		qRGnr();

	MergedIds.Init();
	xdhcmn::FlatMerge( Ids, MergedIds, true );	// Used as is in an JS script, hence last argument at 'true'.

	MergedContents.Init();
	xdhcmn::FlatMerge( Contents, MergedContents, true );	// Used as is in an JS script, hence last argument at 'true'.

	Core_.SetContents(  MergedIds, MergedContents );
qRR;
qRT;
qRE;
}

void sclxdhtml::sProxy::SetContent(
	const str::dString &Id,
	const str::dString &Content )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	tol::Init( Ids, Contents );

	Ids.Append( Id );
	Contents.Append( Content );

	SetContents( Ids, Contents );
qRR;
qRT;
qRE;
}

void sclxdhtml::sProxy::InsertCSSRule(
	const str::dString &Rule,
	xdhcmn::sIndex Index )
{
	return Core_.InsertCSSRule( Rule, Index );
}

xdhcmn::sIndex sclxdhtml::sProxy::AppendCSSRule( const str::dString &Rule )
{
	return Core_.AppendCSSRule( Rule );
}

void sclxdhtml::sProxy::RemoveCSSRule( xdhcmn::sIndex Index )
{
	Core_.RemoveCSSRule( Index );
}

namespace {
	void HandleClasses_(
		const str::dStrings &Ids,
		const str::dStrings &Classes,
		void (xdhdws::sProxy::* Method)(
			const xdhcmn::rNString &Ids,
			const xdhcmn::rNString &Classes ),
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString MergedIds, MergedClasses;
	qRB;
		MergedIds.Init();
		xdhcmn::FlatMerge( Ids, MergedIds, true );	// Passed as is to a JS script, hence 'true'.

		MergedClasses.Init();
		xdhcmn::FlatMerge( Classes, MergedClasses, true );	// Passed as is to a JS script, hence 'true'.

		(Proxy.*Method)( MergedIds, MergedClasses );
	qRR;
	qRT;
	qRE;
	}

	void HandleClass_(
		const str::dString &Id,
		const str::dString &Class,
		void (sProxy::*Method)(
			const str::dStrings &Ids,
			const str::dStrings &Classes ),
		sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Classes;
	qRB;
		Ids.Init();
		Ids.Append( Id );

		Classes.Init();
		Classes.Append( Class );

		(Proxy.*Method)( Ids, Classes );
	qRR;
	qRT;
	qRE;
	}
}

void sclxdhtml::sProxy::AddClasses(
	const str::dStrings &Ids,
	const str::dStrings& Classes )
{
	HandleClasses_( Ids, Classes, &xdhdws::sProxy::AddClasses, Core_ );
}

void sclxdhtml::sProxy::AddClass(
	const str::dString &Id,
	const str::dString &Class )
{
	HandleClass_( Id, Class, &sProxy::AddClasses, *this );
}

void sclxdhtml::sProxy::RemoveClasses(
	const str::dStrings &Ids,
	const str::dStrings& Classes )
{
	HandleClasses_( Ids, Classes, &xdhdws::sProxy::RemoveClasses, Core_ );
}

void sclxdhtml::sProxy::RemoveClass(
	const str::dString &Id,
	const str::dString &Class )
{
	HandleClass_( Id, Class, &sProxy::RemoveClasses, *this );
}

void sclxdhtml::sProxy::ToggleClasses(
	const str::dStrings &Ids,
	const str::dStrings& Classes )
{
	HandleClasses_( Ids, Classes, &xdhdws::sProxy::ToggleClasses, Core_ );
}

void sclxdhtml::sProxy::ToggleClass(
	const str::dString &Id,
	const str::dString &Class )
{
	HandleClass_( Id, Class, &sProxy::ToggleClasses, *this );
}

namespace {
	void HandleElements_(
		const str::dStrings &Ids,
		void (xdhdws::sProxy::* Method)( const xdhcmn::rNString &Ids ),
		xdhdws::sProxy &Proxy )
	{
	qRH;
		str::wString MergedIds;
	qRB;
		MergedIds.Init();
		xdhcmn::FlatMerge( Ids, MergedIds, true );	// Passed as is to a JS script, hence 'true'.

		(Proxy.*Method)( MergedIds );
	qRR;
	qRT;
	qRE;
	}

	void HandleElement_(
		const str::dString &Id,
		void (sProxy::*Method)( const str::dStrings &Ids ),
		sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids;
	qRB;
		Ids.Init();
		Ids.Append( Id );

		(Proxy.*Method)( Ids );
	qRR;
	qRT;
	qRE;
	}
}

void sclxdhtml::sProxy::EnableElements( const str::dStrings &Ids )
{
	HandleElements_( Ids, &xdhdws::sProxy::EnableElements, Core_ );
}

void sclxdhtml::sProxy::EnableElement(	const str::dString &Id )
{
	HandleElement_( Id, &sProxy::EnableElements, *this );
}

void sclxdhtml::sProxy::DisableElements( const str::dStrings &Ids )
{
	HandleElements_( Ids, &xdhdws::sProxy::EnableElements, Core_ );
}

void sclxdhtml::sProxy::DisableElement( const str::dString &Id )
{
	HandleElement_( Id, &sProxy::DisableElements, *this );
}

qCDEF( char *, sclxdhtml::RootTagId_, "XDHRoot" );

void sclxdhtml::prolog::GetLayout(
	sclfrntnd::rFrontend &Frontend,
	xml::writer_ &Writer)
{
	sclfrntnd::GetProjectsFeatures( Frontend.Language(), Writer );
}

static sclmisc::project_type__ GetProjectType_( sProxy &Proxy )
{
	sclmisc::project_type__ ProjectType = sclmisc::pt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	ProjectType = sclmisc::GetProjectType( Proxy.GetValue( prolog::ProjectTypeId, Value ) );
qRR
qRT
qRE
	return ProjectType;
}

void sclxdhtml::prolog::DisplaySelectedProjectFilename(
	sProxy &Proxy,
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
		Proxy.SetValue( RemoteProjectId, FileName );
qRR
qRT
qRE
}

sclmisc::project_type__ sclxdhtml::prolog::GetProjectFeatures(
	sProxy &Proxy,
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
		Feature.Append( Proxy.GetValue( RemoteProjectId, Buffer ) );
		break;
	case sclmisc::ptEmbedded:
		qRVct();	// Not implemented yet.
		break;
	case sclmisc::ptPredefined:
		Feature.Append( Proxy.GetValue( PredefinedProjectId, Buffer ) );
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

void sclxdhtml::prolog::LoadProject( sProxy &Proxy )
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
		sProxy &Proxy,
		str::dString &Type )
	{
		return Proxy.GetValue( login::BackendTypeId, Type );
	}
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
	sProxy &Proxy,
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

		Parameters.Append( Proxy.GetValue( BackendId, Buffer ) );

		if ( NormalizeStraightBackendFeature )
			straight_::Normalize( Parameters );
	}

	sclfrntnd::SetBackendFeatures( Type, Parameters, Features );
qRR
qRT
qRE
}

void sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename(
	sProxy &Proxy,
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
		Proxy.SetValue( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}

