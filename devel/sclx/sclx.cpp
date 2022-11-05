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

#define SCLX_COMPILATION_

#include "sclx.h"

# include "cdgurl.h"

using namespace sclx;

bso::sBool (* sclx::SCLXGetHead)(str::dString &Head) = NULL;

namespace {
	using rgstry::rEntry;

	namespace {
		rEntry XMLFiles_( "XMLFiles", sclr::Definitions );
		rEntry UntaggedXSLFile_( "XSLFile", XMLFiles_ );
	}

	namespace {
	  namespace {
	    rEntry XDHTML_("XDHTML", sclr::Definitions);
	  }

	  rEntry XDHTMLScriptsVersion_("@ScriptsVersion", XDHTML_);
	}
}

rgstry::rEntry registry::definition::HeadFile( "HeadFile", sclr::Definitions );
rgstry::rEntry registry::definition::XMLFilesHandling( "@Handling", XMLFiles_ );
rgstry::rEntry registry::definition::XSLFile( RGSTRY_TAGGING_ATTRIBUTE( "target" ), UntaggedXSLFile_ );

static const char *Launcher_ = NULL;

const sclr::registry_ &sclx::GetRegistry( void )
{
	return sclr::GetCommonRegistry();
}

const char *sclx::GetLauncher( void )
{
	if ( Launcher_ == NULL )
		qRFwk();

	return Launcher_;
}

namespace {
	xdhcuc::cGlobal *Upstream_ = NULL;

	xdhcuc::cGlobal &GetUpstream_(void)
	{
		if ( Upstream_ == NULL )
			qRFwk();

		return *Upstream_;
	}
}

void sclx::Broadcast(
	const str::dString &Action,
	const str::dString &Id)
{
  GetUpstream_().Broadcast(Action, Id, qNIL);
}


#ifdef CPE_S_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
# define FUNCTION_SPEC
#endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scll.cpp', d'o le 'namespace'.
namespace {
	typedef xdhcdc::cGlobal cDownstream_;

	class sDownstream
	: public cDownstream_
	{
	protected:
		virtual xdhcmn::sScriptsVersion XDHCDCInitialize(
			const xdhcdc::sData &Data,
			xdhcuc::cGlobal &Upstream) override
		{
			if ( Upstream_ != NULL )
				qRFwk();

			Upstream_ = &Upstream;

			if ( Launcher_ != NULL )
				qRFwk();

			Launcher_ = Data.LauncherIdentification();
			sclm::Initialize( Data.SCLRack(), Data.Localization(), SCLXInfo() );

			SCLXInitialization(Data.Mode());

			return sclm::MGetU8(XDHTMLScriptsVersion_);
		}
		virtual void XDHCDCBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclm::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcdc::cSingle *XDHCDCFetchCallback(void) override
		{
			return SCLXFetchCallback();
		}
		virtual void XDHCDCDismissCallback(xdhcdc::cSingle *Callback) override
		{
			return SCLXDismissCallback(Callback);
		}
		const scli::sInfo &XDHCDCGetInfo( void ) override
		{
			return SCLXInfo();
		}
		bso::sBool XDHCDCGetHead(
			const str::dString &Token,
			str::dString &Head,
			qRPN ) override
		{
			bso::sBool Success = true;
		qRFH;
		qRFB;
			if ( Token.Amount())
				qRFwk();

      if ( SCLXGetHead != NULL )
        Success = SCLXGetHead(Head);
      else
        sclm::LoadXMLAndTranslateTags( registry::definition::HeadFile, sclr::GetCommonRegistry(), Head, 0, DefaultMarker);
		qRFR;
			Success = false;
		qRFT;
		qRFE(sclm::ErrorDefaultHandling());
			return Success;
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

DEF( XDHCDC_RETRIEVE_FUNCTION_NAME, xdhcdc::retrieve );

xdhcdc::cGlobal *XDHCDCRetrieve( void )
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
		scll::GetTranslation( RawMessage.Convert( Buffer ), Language, Message );

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

void sclx::sProxy::Fill_(
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


void sclx::sProxy::Alert_(
	const str::dString &XML,
	const str::dString &XSL,
	const str::dString &Title,
	const char *Language )
{
qRH
	str::string CloseText;
qRB
	CloseText.Init();
	scll::GetTranslation( SCLX_NAME "_CloseText", Language, CloseText );

	qRVct();  // 'PrettyAlert_1' n'existe pas, mais l'erreur n'est pas affichée correctement ; à investiguer.
	Process_("PrettyAlert_1", NULL, XML, XSL, Title, CloseText );
qRR
qRT
qRE
}

void sclx::sProxy::Alert_(
	const str::dString &RawMessage,
	const str::dString &RawTitle,
	const char *MessageLanguage,	// If != 'NULL', 'Message' is translated, otherwise it is displayed as is.
	const char *CloseTextLanguage ) // Apart, because 'RawMessage' and 'RawTitle' can be already translated.
{
qRH
	str::string XML, XSL;
	str::wString Title;
qRB
	tol::Init(XML, XSL);
	SetXMLAndXSL_( RawMessage, MessageLanguage, XML, XSL );

	Title.Init();

	if ( ( MessageLanguage != NULL ) && RawTitle.Amount() )
		scll::GetTranslation(RawTitle, MessageLanguage, Title);
	else
		Title = RawTitle;

	Alert_(XML, XSL, Title, CloseTextLanguage );
qRR
qRT
qRE
}

void sclx::sProxy::AlertB( const str::dString & RawMessage )
{
qRH;
	str::wString Dummy;
qRB;
	Dummy.Init();

	Process_("Alert_1", &Dummy, RawMessage);    // Get an unused return value to wait the dismissing of the dialog box.
qRR;
qRT;
qRE;
}

void sclx::sProxy::Alert(
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

void sclx::sProxy::AlertT(
	const str::dString &RawMessage,
	const str::dString &RawTitle,
	const char *Language )
{
	Alert_( RawMessage, RawTitle, Language, Language );
}

void sclx::sProxy::AlertU(
	const str::dString &Message,
	const str::dString &Title,
	const char *Language )
{
	Alert_( Message, Title, NULL, Language );
}

#if 0
bso::bool__ sclx::sProxy::Confirm_(
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
	scll::GetTranslation( SCLX_NAME "_CloseText", Language, CloseText );

	Confirmation = Core_.Confirm( XML, XSL, Title, CloseText );
qRR
qRT
qRE
	return Confirmation;
}
#endif

bso::bool__ sclx::sProxy::Confirm_(
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


bso::bool__ sclx::sProxy::ConfirmT(
	const str::dString &RawMessage,
	const char *Language )
{
	return Confirm_( RawMessage, Language, Language );
}

bso::bool__ sclx::sProxy::ConfirmU(
	const str::dString &Message,
	const char *Language )
{
	return Confirm_( Message,  NULL, Language );
}

void sclx::HandleError(
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
		if ( scle::GetPendingErrorTranslation( Language, Message, err::hUserDefined ) ) {
			scle::ResetPendingError();
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
	eXSLFileHandling GetXMLFileHandlingFromRegistry_(const sclr::dRegistry &Registry)
	{
		eXSLFileHandling Result = xfh_Undefined;
	qRH;
		str::wString Handling;
	qRB;
		Handling.Init();

		if (!sclr::OGetValue(Registry, registry::definition::XMLFilesHandling, Handling))
			Handling.Append("Content");	// Default value.

		if (Handling == "Content")
			Result = xfhContent;
		else if (Handling == "Name")
			Result = xfhName;
		else
			sclr::ReportBadOrNoValueForEntryErrorAndAbort(registry::definition::XMLFilesHandling);
	qRR;
	qRT;
	qRE;
		return Result;
	}

	void HandleXSL_(
		const rgstry::rEntry &XSLFilename,
		const char *Target,
		eXSLFileHandling Handling,
		const sclr::registry_ &Registry,
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
			sclx::LoadXSLAndTranslateTags( rgstry::tentry___( XSLFilename, Target ), Registry, RawXSL, Marker );
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

void sclx::sProxy::HandleLayout_(
	const char *Variant,
	const str::dString &Id,
	const rgstry::rEntry &XSLFilename,
	const char *Target,
	const sclr::registry_ &Registry,
	const str::dString &XML,
	bso::char__ Marker)
{
qRH;
	str::wString XSL, Dummy;
qRB;
  tol::Init(XSL, Dummy);
	XSL.Init();

	HandleXSL_(XSLFilename, Target, XSLFileHandling_, Registry, Marker, XSL);

	Process_("HandleLayout_1", &Dummy, Variant, Id, XML, XSL);
qRR;
qRT;
qRE;
}

void sclx::sProxy::GetValues(
	const str::dStrings &Ids,
	str::dStrings &Values)
{
qRH;
	str::wString MergedValues;
qRB;
	MergedValues.Init();

	Process_("GetValues_1", &MergedValues, Ids);

	xdhcmn::FlatSplit(MergedValues, Values);
qRR;
qRT;
qRE;
}

void sclx::sProxy::GetMarks(
	const str::dStrings &Ids,
	str::dStrings &Marks)
{
qRH;
	str::wString MergedMarks;
qRB;
	MergedMarks.Init();

	Process_("GetMarks_1", &MergedMarks, Ids);

	xdhcmn::FlatSplit(MergedMarks, Marks);
qRR;
qRT;
qRE;
}

void sclx::sProxy::EnableElements( const str::dStrings &Ids )
{
	qRLmt();
//	HandleElements_( Ids, &xdhdws::sProxy::EnableElements, Core_ );
}

void sclx::sProxy::EnableElement(	const str::dString &Id )
{
	qRLmt();
//	HandleElement_( Id, &sProxy::EnableElements, *this );
}

void sclx::sProxy::DisableElements( const str::dStrings &Ids )
{
	qRLmt();
//	HandleElements_( Ids, &xdhdws::sProxy::DisableElements, Core_ );
}

void sclx::sProxy::DisableElement( const str::dString &Id )
{
	qRLmt();
//	HandleElement_( Id, &sProxy::DisableElements, *this );
}

