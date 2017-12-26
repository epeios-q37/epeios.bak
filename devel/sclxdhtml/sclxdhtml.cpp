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

void sclxdhtml::Alert(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	sProxy &Proxy,
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
		sProxy &Proxy,
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
	sProxy &Proxy )
{
	Alert_( RawMessage, Language, Proxy, Language );
}

void sclxdhtml::Alert(
	const ntvstr::string___ &Message,
	sProxy &Proxy,
	const char *Language )
{
	Alert_( Message, NULL, Proxy, Language );
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &XML,
	const ntvstr::string___ &XSL,
	const ntvstr::string___ &Title,
	sProxy &Proxy,
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
		sProxy &Proxy,
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
	sProxy &Proxy )
{
	return Confirm_( RawMessage,  Language, Proxy, Language );
}

bso::bool__ sclxdhtml::Confirm(
	const ntvstr::string___ &Message,
	sProxy &Proxy,
	const char *Language )
{
	return Confirm_( Message,  NULL, Proxy, Language );
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
	fSet Set,
	const rgstry::rEntry & Filename,
	const char *Target,
	const sclrgstry::registry_ &Registry,
	const str::dString &XML,
	xdhdws::sProxy &Proxy,
	bso::char__ Marker )
{
qRH
	str::wString XSL;
qRB
	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( Filename, Target ), Registry, XSL, Marker );

	Set( Proxy, Id, XML, XSL );
qRR
qRT
qRE
}

void sclxdhtml::SetContents_(
	const str::dStrings &Ids,
	const str::dStrings &Contents,
	xdhdws::sProxy & Proxy )
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

	SetContents_( Proxy, MergedIds, MergedContents );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetContent_(
	const str::dString &Id,
	const str::dString &Content,
	xdhdws::sProxy &Proxy )
{
qRH;
	str::wStrings Ids, Contents;
qRB;
	tol::Init( Ids, Contents );

	Ids.Append( Id );
	Contents.Append( Content );

	SetContents_( Ids, Contents, Proxy );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetCastsByIds_(
	const str::dStrings &Ids,
	const str::dStrings &Values, 
	xdhdws::sProxy &Proxy )
{
qRH;
	str::wString MergedIds, MergedValues;
qRB;
	MergedIds.Init();
	xdhcmn::FlatMerge( Ids, MergedIds, true );

	MergedValues.Init();
	xdhcmn::FlatMerge( Values, MergedValues, true );

	Proxy.SetCastsByIds( MergedIds, MergedValues );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetCastsByTags_(
	const xdhdws::nstring___ &Id,
	const dCasts &Casts,
	xdhdws::sProxy &Proxy )
{
qRH;
	str::wStrings Tags, Values;
	sdr::sRow Row = qNIL;
	str::wString MergedTags, MergedValues;
qRB;
	tol::Init( Tags, Values );

	Row = Casts.First();

	while ( Row != qNIL ) {
		Tags.Append( Casts( Row ).Tag );
		Values.Append( Casts( Row ).Value );

		Row = Casts.Next( Row );
	}

	MergedTags.Init();
	xdhcmn::FlatMerge( Tags, MergedTags, false );

	MergedValues.Init();
	xdhcmn::FlatMerge( Values, MergedValues, false );

	Proxy.SetCastsByTags( Id, MergedTags, MergedValues );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetCastById_(
	const xdhdws::nstring___ &RawId,
	const xdhdws::nstring___ &RawValue,
	xdhdws::sProxy &Proxy )
{
qRH;
	str::wString Id, Value;
	str::wStrings Ids, Values;
qRB;
	Id.Init();
	RawId.UTF8( Id );
	Ids.Init();
	Ids.Append( Id );

	Value.Init();
	RawValue.UTF8( Value );
	Values.Init();
	Values.Append( Value );

	SetCastsByIds_( Ids, Values, Proxy );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetCastByTag_(
	const xdhdws::nstring___ &Id,
	const dCast &Cast,
	xdhdws::sProxy &Proxy )
{
qRH;
	wCasts Casts;
qRB;
	Casts.Init();
	Casts.Append( Cast );

	SetCastsByTags_( Id, Casts, Proxy );
qRR;
qRT;
qRE;
}

void sclxdhtml::SetCastByTag_(
	const xdhdws::nstring___ &Id,
	const str::dString &Tag,
	const str::dString &Value,
	xdhdws::sProxy &Proxy )
{
qRH;
	wCast Cast;
qRB;
	Cast.Init( Tag, Value );
	SetCastByTag_( Id, Cast, Proxy );
qRR;
qRT;
qRE;
}

qCDEF( char *, sclxdhtml::RootTagId_, "Root" );

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

#define CAST( name )\
	Cast.Init( str::wString( #name "Cast" ), name );\
	Casts.Append( Cast )

void sclxdhtml::prolog::GetCasts(
	sProxy &Proxy,
	dCasts &Casts )
{
qRH;
	str::wString NewProject, PredefinedProjects, UserProject;
	wCast Cast;
qRB;
	tol::Init( NewProject, PredefinedProjects, UserProject );

	switch ( GetProjectType_( Proxy ) ) {
	case sclmisc::ptNew:
		NewProject = "Vanished";
		PredefinedProjects = "Hidden";
		UserProject = "Hidden";
		break;
	case sclmisc::ptPredefined:
		NewProject = "Plain";
		PredefinedProjects = "Plain";
		UserProject = "Hidden";
		break;
	case sclmisc::ptRemote:
		NewProject = "Plain";
		PredefinedProjects = "Hidden";
		UserProject = "Plain";
		break;
	default:
		qRFwk();
		break;
	}

	CAST( NewProject );
	CAST( PredefinedProjects );
	CAST( UserProject );
qRR;
qRT;
qRE;
}

#undef CAST

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

#define CAST( name )\
	Cast.Init( str::wString( #name "BackendCast" ), name );\
	Casts.Append( Cast )

void sclxdhtml::login::GetCasts(
	sProxy &Proxy,
	eBackendVisibility Visibility,
	dCasts &Casts )
{
qRH;
	str::wString Type;
	str::wString None, Straight, Embedded, Predefined, Visible;
	wCast Cast;
qRB;
	tol::Init( Type, None, Straight, Embedded, Predefined, Visible );

	GetBackendType_( Proxy, Type );

	if ( Type == "None" ) {
		None = "Vanished";
		Straight = "Hidden";
		Embedded = "Hidden";
		Predefined = "Hidden";
	} else if ( Type == "Straight" ) {
		None = "Plain";
		Straight = "Plain";
		Embedded = "Hidden";
		Predefined = "Hidden";
	} else if ( Type == "Embedded" ) {
		None = "Plain";
		Straight = "Hidden";
		Embedded = "Plain";
		Predefined = "Hidden";
	} else if ( Type == "Predefined" ) {
		None = "Plain";
		Straight = "Hidden";
		Embedded = "Hidden";
		Predefined = "Plain";
	} else
		qRFwk();

	if ( Visibility == bvShow )
		Visible = "Plain";
	else
		Visible = "Hidden";

	CAST( None );
	CAST( Straight );
	CAST( Embedded );
	CAST( Predefined );
	CAST( Visible );
qRR;
qRT;
qRE;
}

#undef CAST

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

