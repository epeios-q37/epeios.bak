/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define SCLXDHTML_COMPILATION_

#include "sclxdhtml.h"

#include "cdgurl.h"
#include "xdhcmn.h"

using namespace sclxdhtml;

namespace {
	using rgstry::rEntry;

	namespace {
		rEntry XMLFiles_( "XMLFiles", sclrgstry::Definitions );
		rEntry UntaggedXSLFile_( "XSLFile", XMLFiles_ );
	}
}

rgstry::rEntry registry::definition::XMLFilesHandling( "@Handling", XMLFiles_ );
rgstry::rEntry registry::definition::XSLFile( RGSTRY_TAGGING_ATTRIBUTE( "target" ), UntaggedXSLFile_ );
rgstry::rEntry registry::definition::HeadFile( "HeadFile", XMLFiles_ );

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
	protected:
		virtual void XDHCMNInitialize( const xdhcmn::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				qRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.SCLRack(), Data.Localization(), SCLXDHTMLInfo() );

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
		virtual xdhcmn::cSession *XDHCMNRetrieveSession(void) override
		{
			return SCLXDHTMLRetrieveSession();
		}
		virtual void XDHCMNReleaseSession( xdhcmn::cSession *Session ) override
		{
			return SCLXDHTMLReleaseSession( Session );
		}
		const scli::sInfo &XDHCMNGetInfo( void ) override
		{
			return SCLXDHTMLInfo();
		}
		bso::sBool XDHCMNGetHead(
			const str::dString &Token,
			str::dString &Head,
			qRPN ) override
		{
            if ( Token.Amount())
                qRFwk();

			sclmisc::LoadXMLAndTranslateTags( registry::definition::HeadFile, sclrgstry::GetCommonRegistry(), Head, 1, DefaultMarker);

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{}
		E_CVDTOR( sDownstream );
		void Init( void )
		{}
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
	void PutInXML_(
		const str::string_  &Message,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_TOFLOW___ STOFlow;
		xml::rWriter Writer;
	qRB
		STOFlow.Init( XML );
		Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

		Writer.PutValue( Message, "Content" );
	qRR
	qRT
	qRE
	}

	void TranslateAndPutInXML_(
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

		PutInXML_( Message, XML );
	qRR
	qRT
	qRE
	}

	inline void EncodeXML_(
		const str::string_ &Plain,
		str::dString &Encoded )
	{
		Encoded.Append("data:text/xml;charset=utf-8,");

		cdgurl::Encode( Plain, Encoded );
	}

	const char *AlertBaseXSL_ = "\
<?xml version = \"1.0\" encoding=\"utf-8\"?>\
<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
	<xsl:output method=\"html\" encoding=\"utf-8\"/>\
	<xsl:template match=\"/\">\
		<xsl:value-of select=\"Content\"/>\
	</xsl:template>\
</xsl:stylesheet>";

	inline void SetXMLAndXSL_(
		const str::dString &Message,
		const char *Language,
		str::string_ &XML,
		str::string_ &XSL )
	{
		if ( Language == NULL )
			PutInXML_( Message, XML );
		else
			TranslateAndPutInXML_( Message, Language, XML );

		EncodeXML_( str::wString( AlertBaseXSL_ ), XSL );
	}
}

void sclxdhtml::sProxy::Fill_(
    str::dStrings &Values,
    const str::dStrings &SplittedValues)
{
qRH
    str::wString MergedValues;
qRB
    MergedValues.Init();

    xdhcmn::FlatMerge(SplittedValues, MergedValues, true);

    Values.Append(MergedValues);
qRR
qRT
qRE
}


void sclxdhtml::sProxy::Alert_(
	const str::dString &XML,
	const str::dString &XSL,
	const str::dString &Title,
	const char *Language )
{
qRH
	str::string CloseText;
qRB
	CloseText.Init();
//	scllocale::GetTranslation( SCLXDHTML_NAME "_CloseText", Language, CloseText );

//	Process_("PrettyAlert_1", NULL, XML, XSL, Title, CloseText );
    Process_("Alert_1", &CloseText, XML);
qRR
qRT
qRE
}

void sclxdhtml::sProxy::Alert_(
	const str::dString &RawMessage,
	const str::dString &RawTitle,
	const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
	const char *CloseTextLanguage )
{
qRH
	str::string XML, XSL;
	str::wString Title;
qRB
	tol::Init(XML, XSL);
	SetXMLAndXSL_( RawMessage, MessageLanguage, XML, XSL );

	Title.Init();

    if ( ( MessageLanguage != NULL ) && RawTitle.Amount() )
        scllocale::GetTranslation(RawTitle, MessageLanguage, Title);
    else
        Title = RawTitle;

    Alert_(XML, XSL, Title, CloseTextLanguage );
qRR
qRT
qRE
}

void sclxdhtml::sProxy::AlertB( const str::dString & RawMessage )
{
qRH;
	str::wString Script;
qRB;
	Script.Init( "alert('" );

	xdhcmn::Escape( RawMessage, Script, '\'' );

	Script.Append( "');" );

	Execute( Script );
qRR;
qRT;
qRE;
}

void sclxdhtml::sProxy::Alert(
	const str::dString &XML,
	const str::dString &XSL,
	const str::dString &Title,
	const char *Language )
{
qRH;
	str::wString EncodedXSL;
qRB;
	tol::Init( EncodedXSL);
	EncodeXML_( XSL, EncodedXSL );

	Alert_( XML, EncodedXSL, Title, Language );
qRR;
qRT;
qRE;
}

void sclxdhtml::sProxy::AlertT(
	const str::dString &RawMessage,
	const str::dString &RawTitle,
	const char *Language )
{
	Alert_( RawMessage, RawTitle, Language, Language );
}

void sclxdhtml::sProxy::AlertU(
	const str::dString &Message,
	const str::dString &Title,
	const char *Language )
{
	Alert_( Message, Title, NULL, Language );
}

#if 0
bso::bool__ sclxdhtml::sProxy::Confirm_(
	const str::dString &XML,
	const str::dString &XSL,
	const str::dString &Title,
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
#endif

bso::bool__ sclxdhtml::sProxy::Confirm_(
	const str::dString &Message,
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

	Confirmation = Confirm_( XML, XSL, str::wString(), CloseTextLanguage );
qRR
qRT
qRE
	return Confirmation;
}


bso::bool__ sclxdhtml::sProxy::ConfirmT(
	const str::dString &RawMessage,
	const char *Language )
{
	return Confirm_( RawMessage, Language, Language );
}

bso::bool__ sclxdhtml::sProxy::ConfirmU(
	const str::dString &Message,
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
			Proxy.AlertU( Message, str::wString(), Language );
		}
		break;
	case err::t_Free:
		Proxy.AlertB( str::wString("???") );
		break;
	default:
		Proxy.AlertB( str::wString(err::Message( ErrBuffer )) );
		break;
	}

	ERRRst();
qRR
qRT
qRE
}

namespace {
    eXSLFileHandling GetXMLFileHandlingFromRegistry_(const sclrgstry::dRegistry &Registry)
    {
        eXSLFileHandling Result = xfh_Undefined;
    qRH;
        str::wString Handling;
    qRB;
        Handling.Init();

        if (!sclrgstry::OGetValue(Registry, registry::definition::XMLFilesHandling, Handling))
            Handling.Append("Content");	// Default value.

        if (Handling == "Content")
            Result = xfhContent;
        else if (Handling == "Name")
            Result = xfhName;
        else
            sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort(registry::definition::XMLFilesHandling);
    qRR;
    qRT;
    qRE;
        return Result;
    }

    void HandleXSL_(
        const rgstry::rEntry &XSLFilename,
        const char *Target,
        eXSLFileHandling Handling,
        const sclrgstry::registry_ &Registry,
        bso::sChar Marker,
        str::dString &XSL )
    {
    qRH;
        str::wString RawXSL;
    qRB;
        if ( Handling == xfhRegistry )
            Handling = GetXMLFileHandlingFromRegistry_( Registry );

        switch ( Handling ) {
        case xfhContent:
            // The content of the XSL file is transmitted (global XDHTML behavior).
            RawXSL.Init();
            sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( XSLFilename, Target ), Registry, RawXSL, Marker );
            XSL.Append( "data:text/xml;charset=utf-8," );
            cdgurl::Encode( RawXSL, XSL );
            break;
        case xfhName:
            // The _name_ of the XSL file is transmitted (Atlas toolkit behavior).
            // NOTA: the Atlas toolkit does NOT use this function. It uses a underlying function which have this behavior.
            XSL.Append( Target );
            XSL.Append( ".xsl" );
            break;
        default:
            qRFwk();
            break;
        }
    qRR;
    qRT;
    qRE;
    }
}

void sclxdhtml::sProxy::HandleLayout_(
    const char *Variant,
	const str::dString &Id,
	const rgstry::rEntry &XSLFilename,
	const char *Target,
	const sclrgstry::registry_ &Registry,
	const str::dString &XML,
	bso::char__ Marker)
{
qRH;
    str::wString XSL;
qRB;
    XSL.Init();

    HandleXSL_(XSLFilename, Target, XSLFileHandling_, Registry, Marker, XSL);

    Process_("HandleLayout_1", NULL, Variant, Id, XML, XSL);
qRR;
qRT;
qRE;
}

void sclxdhtml::sProxy::GetContents(
	const str::dStrings &Ids,
	str::dStrings &Contents)
{
qRH;
	str::wString MergedContents;
qRB;
    MergedContents.Init();

    Process_("GetContents_1", &MergedContents, Ids);

    xdhcmn::FlatSplit(MergedContents, Contents);
qRR;
qRT;
qRE;
}

const str::dString &sclxdhtml::sProxy::GetContent(
	const str::dString &Id,
	str::dString &Content)
{
qRH;
	str::wStrings Contents;
qRB;
    Contents.Init();

    GetContents(str::wStrings(Id), Contents);

    if ( Contents.Amount() != 1 )
        qRFwk();

    Content = Contents(Contents.First());
qRR;
qRT;
qRE;
    return Content;
}

void sclxdhtml::sProxy::SetTimeout(
	const str::dString &Delay,
	const str::dString &Action )
{
    qRLmt();
//	Core_.SetTimeout( Delay, Action );
}

void sclxdhtml::sProxy::EnableElements( const str::dStrings &Ids )
{
    qRLmt();
//	HandleElements_( Ids, &xdhdws::sProxy::EnableElements, Core_ );
}

void sclxdhtml::sProxy::EnableElement(	const str::dString &Id )
{
    qRLmt();
//	HandleElement_( Id, &sProxy::EnableElements, *this );
}

void sclxdhtml::sProxy::DisableElements( const str::dStrings &Ids )
{
    qRLmt();
//	HandleElements_( Ids, &xdhdws::sProxy::DisableElements, Core_ );
}

void sclxdhtml::sProxy::DisableElement( const str::dString &Id )
{
    qRLmt();
//	HandleElement_( Id, &sProxy::DisableElements, *this );
}

void sclxdhtml::prolog::GetLayout(
	sclfrntnd::rFrontend &Frontend,
	xml::rWriter &Writer)
{
	sclfrntnd::GetProjectsFeatures( Frontend.Language(), Writer );
}

static sclmisc::eProjectType GetProjectType_( sProxy &Proxy )
{
	sclmisc::eProjectType ProjectType = sclmisc::pt_Undefined;
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

void sclxdhtml::prolog::HandleProjectTypeSwitching( sProxy & Proxy )
{
	switch ( GetProjectType_( Proxy ) ) {
	case sclmisc::ptNew:
		Proxy.AddClass( prolog::RemoteProjectFormId, "hide" );
		Proxy.AddClass( prolog::PredefinedProjectFormId, "hide" );
		Proxy.AddClass( prolog::BorderId, "fieldset-vanish" );
		break;
	case sclmisc::ptPredefined:
		Proxy.AddClass( prolog::RemoteProjectFormId, "hide" );
		Proxy.RemoveClass( prolog::PredefinedProjectFormId, "hide" );
		Proxy.RemoveClass( prolog::BorderId, "fieldset-vanish" );
		break;
	case sclmisc::ptRemote:
		Proxy.RemoveClass( prolog::RemoteProjectFormId, "hide" );
		Proxy.AddClass( prolog::PredefinedProjectFormId, "hide" );
		Proxy.RemoveClass( prolog::BorderId, "fieldset-vanish" );
		break;
	default:
		qRFwk();
		break;
	}
}

void sclxdhtml::prolog::DisplaySelectedProjectFilename(
	sProxy &Proxy,
	const str::dString &Id )
{
qRH
	str::wString Buffer;
	str::wString FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	tol::Init(Args, Buffer);
	xdhcmn::Split( Proxy.GetContent( Id, Buffer ), Args );

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

sclmisc::eProjectType sclxdhtml::prolog::GetProjectFeatures(
	sProxy &Proxy,
	str::string_ &Feature )
{
	sclmisc::eProjectType Type = sclmisc::pt_Undefined;
qRH
	str::wString Buffer;
qRB
    Buffer.Init();

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

void sclxdhtml::prolog::LoadProject( sProxy &Proxy )
{
qRH
	str::string ProjectFeature;
qRB
	ProjectFeature.Init();
	sclfrntnd::LoadProject( prolog::GetProjectFeatures( Proxy, ProjectFeature ), ProjectFeature, Proxy.Info() );
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

#undef C

sclfrntnd::eLogin sclxdhtml::login::GetLayout(
	sclfrntnd::rFrontend &Frontend,
	xml::rWriter &Writer)
{
	sclfrntnd::GetBackendsFeatures( Frontend.Language(), Writer );

	return sclfrntnd::GetLoginFeatures( Writer );
}

namespace {
	const str::dString &GetType_(
		sProxy &Proxy,
		str::dString &Type )
	{
		return Proxy.GetContent( login::BackendTypeId, Type );
	}

	qENUM( BackendType_ )
	{
		// Below both types are special backend types.
		btNone,			// No backend.
		btPredefined,	// Predefined backend.
		// Below backend types are plugins.
		btEmbedded,
		btStraight,
		btProxy,
		bt_amount,
		bt_Undefined
	};

	stsfsm::wAutomat TypeAutomat_;

#define C( name )	case bt##name : return #name; break

	const char *GetLabel_( eBackendType_ Type )
	{
		switch ( Type ) {
		case btNone:
			return sclfrntnd::NoneBackendType;
			break;
		case btPredefined:
			return sclfrntnd::PredefinedBackendType;
			break;
		C( Embedded );
		C( Straight );
		C( Proxy );
		default:
			qRFwk();
			break;
		}

		return NULL;	// To avoid a warning.
	}

#undef C

	void FillTypeAutomat_( void )
	{
		TypeAutomat_.Init();
		stsfsm::Fill<eBackendType_>( TypeAutomat_, bt_amount, GetLabel_ );
	}

	eBackendType_ GetType_( const str::dString &Pattern )
	{
		return stsfsm::GetId( Pattern, TypeAutomat_, bt_Undefined, bt_amount );
	}

	eBackendType_ GetType_( sProxy &Proxy )
	{
		eBackendType_ Type = bt_Undefined;
	qRH;
		str::wString Pattern;
	qRB;
		Pattern.Init();

		Type = GetType_( GetType_( Proxy, Pattern ) );
	qRR;
	qRT;
	qRE;
		return Type;
	}

#define A( name )	Ids.Append(login::name##BackendId)

	void SetIds_( str::dStrings &Ids )
	{
		A( Predefined );
		A( Remote );
		A( Proxyfied );
		A( Embedded );
	}

#undef A

	void SetClasses_( str::dStrings &Classes )
	{
		Classes.Append("hide");
		Classes.Append("hide");
		Classes.Append("hide");
		Classes.Append("hide");
	}

	void HideAll_( sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Classes;
	qRB;
		tol::Init( Ids, Classes );

		SetIds_( Ids );
		SetClasses_( Classes );

		Proxy.AddClasses( Ids, Classes );
	qRR;
	qRT;
	qRE;
	}
}

#define S( name )	Proxy.RemoveClass( login::name##BackendId, "hide" )

void sclxdhtml::login::HandleBackendTypeSwitching( sProxy & Proxy )
{
	HideAll_( Proxy );

	switch ( GetType_( Proxy ) ) {
	case btNone:
		// Nothing to do ; all forms remain hidden.
		break;
	case btPredefined:
		S( Predefined );
		break;
	case btEmbedded:
		S( Embedded );
		break;
	case btStraight:
		S( Remote );
		break;
	case btProxy:
		S( Proxyfied );
		break;
	default:
		qRFwk();
		break;
	}
}

#undef S

namespace {
	void NormalizeParameters_(
		str::dString &Parameters,
		const str::dString DefaultPort )
	{
		if ( Parameters.Search( ':' ) == qNIL ) {
			if ( DefaultPort.Amount() ) {
				Parameters.Append( ':' );
				Parameters.Append( DefaultPort );
			}
		}
	}
}

namespace straight_ {
	namespace {
		rgstry::rEntry DefaultPort_( "@DefaultStraightPort", sclfrntnd::BackendParametersRegistryEntry );
	}

	void Normalize( str::dString &Parameters )
	{
	qRH
		str::wString Port;
	qRB
		Port.Init();
		sclmisc::OGetValue( DefaultPort_, Port );

		NormalizeParameters_( Parameters, Port );
	qRR
	qRT
	qRE
	}
}

namespace proxy_ {
	namespace {
		rgstry::rEntry DefaultPort_( "@DefaultProxyPort", sclfrntnd::BackendParametersRegistryEntry );
	}

	void Normalize( str::dString &Parameters )
	{
	qRH
		str::wString Port;
	qRB
		Port.Init();
		sclmisc::OGetValue( DefaultPort_, Port );

		NormalizeParameters_( Parameters, Port );
	qRR
	qRT
	qRE
	}
}

void sclxdhtml::login::GetBackendFeatures(
	sProxy &Proxy,
	sclfrntnd::rFeatures &Features )
{
qRH;
	eBackendType_ Type = bt_Undefined;
	str::wString Buffer;
	str::string Parameters;
qRB;
    tol::Init(Parameters, Buffer);

	switch ( Type = GetType_( Proxy ) ) {
	case btNone:
		break;
	case btPredefined:
		Parameters.Append( Proxy.GetContent( PredefinedBackendId, Buffer ) );
		break;
	case btEmbedded:
		Parameters.Append( Proxy.GetContent( EmbeddedBackendId, Buffer ) );
		break;
	case btStraight:
		Parameters.Append( Proxy.GetContent( RemoteBackendId, Buffer ) );
		straight_::Normalize( Parameters );
		break;
	case btProxy:
		Parameters.Append( Proxy.GetContent( ProxyfiedBackendId, Buffer ) );
		proxy_::Normalize( Parameters );
		break;
	default:
		qRFwk();
		break;
	}

	sclfrntnd::SetBackendFeatures( str::wString( GetLabel_( Type ) ), Parameters, Features );
qRR;
qRT;
qRE;
}

void sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename(
	sProxy &Proxy,
	const str::dString &Id )
{
qRH
	str::wString Buffer;
	str::string FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	tol::Init(Args, Buffer);
	xdhcmn::Split( str::string( Proxy.GetContent( Id, Buffer ) ), Args );

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

qGCTOR( sclxdhtml )
{
	FillTypeAutomat_();
}
