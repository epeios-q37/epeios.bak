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

#define XULFTK_COMPILATION_

#include "xulftk.h"

#include "xulfrg.h"

#include "fnm.h"

using namespace xulftk;

#define SETUP_LABEL		FRDKRN_SETUP_LABEL

#define SETUPS_LABEL	SETUP_LABEL "s"

static void GetAuthorText_(
	const char *Name,
	const char *Contact,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( XULFTK_NAME "_AuthorText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( Contact );

	Locale.GetTranslation( Meaning, Language, Text );
qRR
qRT
qRE
}

static void GetAffiliationText_(
	const char *Tag,
	const char *Name,
	const char *URL,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();
	Meaning.SetValue( Tag );
	Meaning.AddTag( Name );
	Meaning.AddTag( URL );

	Locale.GetTranslation( Meaning, Language, Text );
qRR
qRT
qRE
}

const str::string_ &xulftk::BuildAboutText(
	const char *LauncherIdentification,
	const char *BuildInformations,
	const char *AppName,
	const char *Version,
	const char *AuthorName,
	const char *AuthorContact,
	const char *Copyright,
	const char *SoftwareDetails,
	const char *SoftwareURL,
	xulftk::trunk___ &Trunk,
	str::string_ &Text )
{
qRH
	str::string AuthorText, ProjectAffiliationText, SoftwareAffiliationText, ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
qRB
	Text.Append( AppName );
	Text.Append( ' ' );
	Text.Append( Version );
	Text.Append( " Build " __DATE__ " " __TIME__ );
	Text.Append( " (" );
	Text.Append( BuildInformations );
	Text.Append( ")\n\t" );

	AuthorText.Init();
	GetAuthorText_( AuthorName, AuthorContact, Trunk.Kernel().Locale(), Trunk.Kernel().Language(), AuthorText );
	Text.Append( AuthorText );

	Text.Append( "\nCopyright " );
	Text.Append( Copyright );

	Text.Append( "\n\n\t\t" );
	SoftwareAffiliationText.Init();
	GetAffiliationText_( XULFTK_NAME "_AffiliatedSoftwareText", SoftwareDetails, SoftwareURL, Trunk.Kernel().Locale(), Trunk.Kernel().Language(), SoftwareAffiliationText );
	Text.Append( SoftwareAffiliationText );
	Text.Append( '.' );

	Text.Append( "\n\n(" );
	Text.Append( LauncherIdentification );
	Text.Append( ")" );


	if ( Trunk.Kernel().IsConnected() ) {
		Text.Append( '\n' );
		Text.Append( '\n' );

		ProtocolVersion.Init();
		BackendLabel.Init();
		APIVersion.Init();
		BackendInformations.Init();
		BackendCopyright.Init();
		SoftwareInformations.Init();

		Trunk.Kernel().AboutBackend( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

		Text.Append( "Backend : " );
		Text.Append( BackendInformations );
		Text.Append( " (" );
		Text.Append( SoftwareInformations );
		Text.Append( ')' );
	}
qRR
qRT
qRE
	return Text;
}


void xulftk::reporting_functions__::FRDKRNReport( const str::string_ &Message )
{
	if ( _Trunk == NULL )
		qRFwk();

	_Trunk->UI().LogAndPrompt( Message );

	qRAbort();
}

void xulftk::reporting_functions__::FRDKRNNotify( const str::string_ &Message )
{
	if ( _Trunk == NULL )
		qRFwk();

	_Trunk->UI().Alert( Message );

	qRAbort();
}

void xulftk::reporting_functions__::Init( trunk___ &Trunk )
{
	_Trunk = &Trunk;

	_reporting_functions__::Init( Trunk.Kernel() );
}

void xulftk::trunk___::Handle_( frdkrn::status__ Status )
{
qRH
	str::string Translation;
qRB
	Translation.Init();

	Kernel().GetErrorMeaningTranslation( Translation );

	switch( Status ) {
	case frdkrn::sOK:
		break;
	case frdkrn::sWarning:
		_UI->LogAndPrompt( Translation );
		break;
	case frdkrn::sError:
		_UI->Alert( Translation );
		qRAbort();
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
}

enum annex_type__
{
	atEmbedded,
	atFile,
	atVolatile,
	at_amount,
	at_Undefined
};


static annex_type__ GetAnnexType_( const rgstry::multi_level_registry_ &Registry )
{
	annex_type__ Type = at_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();

	if ( Registry.GetValue( xulfrg::AnnexTargetType, Value ) ) {
		if ( Value == "Embedded" )
			Type = atEmbedded;
		else if ( Value == "File" )
			Type = atFile;
		else if ( Value == "Volatile " )
			Type = atVolatile;
	}
qRR
qRT
qRE
	return Type;
}

static bso::bool__ IsValid_(
	const str::string_ &Value,
	bso::bool__ AlpahaNumericOnly )	// A 'true' si la valeur doit tre alpha-numrique, comme un libell d'attribut, par example.
{
	sdr::row__ Row = Value.First();

	if ( Value.Amount() == 0 )
		return false;

	if ( !AlpahaNumericOnly )
		return true;

	while ( Row != qNIL ) {
		if ( !isalnum( Value( Row ) ) && ( Value( Row ) != '_' ) )
			return false;

		Row = Value.Next( Row );
	}

	return true;
}


static const char *GetAnnexTarget_( 
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	TOL_CBUFFER___ &Buffer )
{
	const char *Result = NULL;
qRH
	str::string Target, Translation;
qRB
	Target.Init();

	Registry.GetValue( xulfrg::AnnexTarget, Target );

	if ( !IsValid_( Target, true ) ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTarget", Language, Translation ) );

		qRReturn;
	}

	Result = Target.Convert( Buffer );
qRR
qRT
qRE
	return Result;
}

enum dir__
{
	dIn,
	dOut,
	d_amount,
	d_Undefined,
};


template <typename bag, typename flow> static flow *GetEmbeddedFlow_(
	dir__ Direction,
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
qRH
	TOL_CBUFFER___ Buffer;
	const char *Target = NULL;
qRB
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, Buffer ) ) == NULL )
		qRReturn;

	Bag.Embedded.Init();
	switch ( Direction ) {
	case dIn:
		nsxpcm::GetAttribute( nsxpcm::GetElement( UI.Main().Window() ), Target, Bag.Embedded );
		break;
	case dOut:
		break;
	default:
		qRFwk();
		break;
	}

	Bag.EmbeddedFlow.Init( Bag.Embedded );
	Flow = &Bag.EmbeddedFlow;
qRR
qRT
qRE
	return Flow;
}

template <typename bag, typename flow> static flow *GetFileFlow_(
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
qRH
	TOL_CBUFFER___ Buffer;
	const char *Target = NULL;
qRB
	if ( ( Target = GetAnnexTarget_( UI, Registry, Locale, Language, Buffer ) ) == NULL )
		qRReturn;

	if ( Bag.FileFlow.Init( Target, err::hUserDefined ) != tol::rSuccess )
		Flow = &Bag.VoidFlow;
	else
		Flow = &Bag.FileFlow;
qRR
qRT
qRE
	return Flow;
}

template <typename bag, typename flow> static flow  *GetVolatileFlow_( bag &Bag )
{
	return &Bag.VoidFlow;
}

template <typename bag, typename flow> static flow  *GetFlow_(
	dir__ Direction,
	bag &Bag,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	flow *Flow = NULL;
qRH
	str::string Translation;
qRB
	switch ( GetAnnexType_( Registry )  ) {
	case atEmbedded:
		Flow = GetEmbeddedFlow_<bag, flow>( Direction, Bag, UI, Registry, Locale, Language );
		break;
	case atFile:
		Flow = GetFileFlow_<bag, flow>( Bag, UI, Registry, Locale, Language );
		break;
	case atVolatile:
		Flow = GetVolatileFlow_<bag, flow>( Bag );
		break;
	case at_Undefined:
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_MissingOrBadAnnexTargetType", Language, Translation ) );

		qRReturn;
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Flow;
}


static void Write_(
	const rgstry::registry_ &Set,
	rgstry::row__ Root,
	flw::oflow__ &Flow )
{
qRH
	txf::text_oflow__ TFlow;
qRB
	TFlow.Init( Flow );
	Set.Dump( Root, true, xml::oCompact, xml::e_None, TFlow );
qRR
qRT
qRE
}

static rgstry::row__ ReadSetups_(
	flw::iflow__ &Flow,
	rgstry::registry_ &Setups )
{
	rgstry::row__ Root = qNIL;
qRH
	xtf::extended_text_iflow__ XFlow;
qRB
	XFlow.Init( Flow, utf::f_Default );

	if ( rgstry::FillRegistry( XFlow, xpp::criterions___(), SETUPS_LABEL, Setups, Root ) != rgstry::sOK )
		Root = qNIL;
qRR
qRT
qRE
	return Root;
}

static const str::string_ &BuildSetupPath_(
	const str::string_ &Id,
	str::string_ &Path )
{
	Path.Append( SETUP_LABEL "[id=\"" );
	Path.Append( Id );
	Path.Append( "\"]" );

	return Path;
}

static bso::bool__ GetSetup_(
	const str::string_ &Id,
	const rgstry::registry_ &Setups,
	rgstry::row__ Root,
	str::string_ &Setup )
{
	bso::bool__ Success = false;
qRH
	str::string Path;
qRB
	Path.Init();

	BuildSetupPath_( Id, Path );

	Success = Setups.GetValue( Path, Root, Setup );
qRR
qRT
qRE
	return Success;
}

static void SetSetup_(
	const str::string_ &Id,
	const str::string_ &Setup,
	rgstry::registry_ &Setups,
	rgstry::row__ Root )
{
qRH
	str::string Path;
qRB
	Path.Init();

	BuildSetupPath_( Id, Path );

	Setups.SetValue( Path, Setup, Root );
qRR
qRT
qRE
}

struct ibag___ {
	str::string Embedded;
	flf::file_iflow___ FileFlow;
	flx::E_STRING_IFLOW__  EmbeddedFlow;
	flw::iflow__ &VoidFlow;
	ibag___ ( void )
	: VoidFlow( flx::VoidIFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static rgstry::row__ RetrieveSetups_(
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	rgstry::registry_ &Setups )
{
	rgstry::row__ Root = qNIL;
qRH
	flw::iflow__ *Flow = NULL;
	ibag___ Bag;
	str::string Translation;
qRB
	Bag.Init();
	
	if ( ( Flow = GetFlow_<ibag___,flw::iflow__>( dIn, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToRetrieve" SETUPS_LABEL, Language, Translation ) );
		qRReturn;
	}

	if ( Flow->EndOfFlow() ) {
		Root = Setups.CreateRegistry( str::string( SETUPS_LABEL ) );
		qRReturn;
	}

	if ( ( Root = ReadSetups_( *Flow, Setups ) ) == qNIL ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToFind" SETUPS_LABEL, Language, Translation) );
		qRReturn;
	}
qRR
qRT
qRE
	return Root;
}

void xulftk::trunk___::_DefineSession( const str::string_ &ProjectId )
{
qRH
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string Location;
qRB
	Kernel().Registry().SetValue( frdrgy::ProjectId, ProjectId );

	Kernel().Registry().GetValue( frdrgy::Backend, Location );

	switch ( Type = frdkrn::GetBackendExtendedType( Kernel().Registry() ) ) {
	case frdkrn::bxtNone:
		break;
	case frdkrn::bxtPredefined:
		qRFwk();
		break;
	case frdkrn::bxtDaemon:
		UI().SessionForm().Widgets.txbDaemonBackend.SetValue( Location );
		break;
	case frdkrn::bxtEmbedded:
		UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( Location );
		break;
	case frdkrn::bxt_Undefined:
		Type = frdkrn::bxtNone;
		break;
	default:
		qRFwk();
		break;
	}

	UI().SessionForm().SetBackendType( Type );
qRR
qRT
qRE
}


#if 0 // Old
void xulftk::trunk___::_DefineSession(
	const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel prremplir le 'SessionForm'.
	const xpp::criterions___ &Criterions )
{
qRH
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string Location;
	str::string ProjectId;
	bso::integer_buffer__ Buffer;
qRB
	ProjectId.Init();

	if ( ProjectFileName.Amount() != 0 ) {
		Handle_( Kernel().LoadProject( ProjectFileName, _TargetName, Criterions, ProjectId ) );
		Location.Init();
		Kernel().Registry().GetValue( frdrgy::Backend, Location );
	} else {
		Kernel().CreateNewProject();
		ProjectId.Init( bso::Convert( tol::EpochTime( true ), Buffer ) );
	}

	Kernel().Registry().SetValue( frdrgy::ProjectId, ProjectId );

	switch ( Type = frdkrn::GetBackendExtendedType( Kernel().Registry() ) ) {
	case frdkrn::bxtNone:
		break;
	case frdkrn::bxtPredefined:
		qRFwk();
		break;
	case frdkrn::bxtDaemon:
		UI().SessionForm().Widgets.txbDaemonBackend.SetValue( Location );
		break;
	case frdkrn::bxtEmbedded:
		UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( Location );
		break;
	case frdkrn::bxt_Undefined:
		Type = frdkrn::bxtNone;
		break;
	default:
		qRFwk();
		break;
	}

	UI().SessionForm().SetBackendType( Type );
qRR
qRT
qRE
}
#endif

static const str::string_ &GetProjectId_(
	trunk___ &Trunk,
	str::string_ &ProjectId )
{
qRH
	str::string Translation;
qRB
	if ( !Trunk.Kernel().Registry().GetValue( frdrgy::ProjectId, ProjectId ) || (  ProjectId.Amount() == 0 ) ) {
		Translation.Init();
		Trunk.UI().LogAndPrompt( Trunk.Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoProjectId", Trunk.Kernel().Language(), Translation) );

		qRAbort();
	}
qRR
qRT
qRE
	return ProjectId;
}


void xulftk::trunk___::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
qRH
	xtf::extended_text_iflow__ XFlow;
	rgstry::registry Setups;
	rgstry::row__ Root = qNIL;
	str::string Setup;
	flx::E_STRING_IFLOW__ Flow;
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
	str::string ProjectId;
	str::string Translation;
qRB
	Value.Init();
	UI().SessionForm().Widgets.mnlBackendType.GetValue( Value );

	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtNone:
		Value.Init();
		break;
	case frdkrn::bxtPredefined:
		Value.Init();
		UI().SessionForm().Widgets.mnlPredefinedBackend.GetValue( Value );
		break;
	case frdkrn::bxtDaemon:
		Value.Init();
		UI().SessionForm().Widgets.txbDaemonBackend.GetValue( Value );
		break;
	case frdkrn::bxtEmbedded:
		Value.Init();
		UI().SessionForm().Widgets.txbEmbeddedBackend.GetValue( Value );
		break;
	default:
		Translation.Init();
		UI().LogAndPrompt( Kernel().Locale().GetTranslation( XULFTK_NAME "_BadOrNoBackendType", Kernel().Language(), Translation ) );
		qRAbort();
		break;
	}

	Kernel().Registry().SetValue( frdrgy::Backend, Value );
	frdkrn::SetBackendExtendedType( Kernel().Registry(), Type );

	Handle_( Kernel().Launch( CompatibilityInformations ) );

	Setups.Init();

	if ( ( Root = RetrieveSetups_( UI(), Kernel().Registry(), Kernel().Locale(), Kernel().Language(), Setups ) ) == qNIL )
		qRAbort();

	ProjectId.Init();
	GetProjectId_( *this, ProjectId );

	Setup.Init();
	GetSetup_( ProjectId, Setups, Root, Setup );

	Session().OpenSession( Kernel().Language() );

	if ( Setup.Amount() != 0 ) {
		Flow.Init( Setup );

		XFlow.Init( Flow, utf::f_Default );

		Handle_( Session().FillSetupRegistry( XFlow, xpp::criterions___() ) );
	}
qRR
	Session().CloseSession();
	Kernel().CloseProject();
qRT
qRE
}

bso::bool__ xulftk::trunk___::_DefendSession( void )
{
	bso::bool__ Confirmed = false;
qRH
	str::string Translation;
qRB
	Translation.Init();
	Translation.Init( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingAsking", Kernel().Language(), Translation) );
	Translation.Append( ' ' );
	Translation.Append( Kernel().Locale().GetTranslation( XULFTK_NAME "_ProjectClosingWarning", Kernel().Language(), Translation ) );

	Confirmed = UI().Confirm( Translation );
qRR
qRT
qRE
	return Confirmed;
}

struct obag___ {
	str::string Embedded;
	flf::file_oflow___ FileFlow;
	flx::E_STRING_OFLOW___  EmbeddedFlow;
	flw::oflow__ &VoidFlow;
	obag___ ( void )
	: VoidFlow( flx::VoidOFlow )
	{}
	void Init( void )
	{
		// Standardisation.
	}
};

static bso::bool__ StoreSetups_(
	const rgstry::registry_ &Setups,
	rgstry::row__ Root,
	xulfui::ui___ &UI,
	const rgstry::multi_level_registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language )
{
	bso::bool__ Success = false;
qRH
	flw::oflow__ *Flow = NULL;
	obag___ Bag;
	str::string Translation;
	TOL_CBUFFER___ Buffer;
qRB
	Bag.Init();
	
	if ( ( Flow = GetFlow_<obag___,flw::oflow__>( dOut, Bag, UI, Registry, Locale, Language ) ) == NULL ) {
		Translation.Init();
		UI.LogAndPrompt( Locale.GetTranslation( XULFTK_NAME "_UnableToStore" SETUPS_LABEL, Language, Translation ) );

		qRReturn;
	}

	Write_( Setups, Root, *Flow );

	Bag.EmbeddedFlow.reset();

	UI.SaveAnnex( GetAnnexTarget_( UI, Registry, Locale, Language, Buffer), Bag.Embedded );

	Success = true;
qRR
qRT
qRE
	return Success;
}

static void GetSetup_(
	const frdssn::session___ &Session,
	str::string_ &Setup )
{
qRH
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
qRB
	Flow.Init( Setup );
	TFlow.Init( Flow );
	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Session.DumpSetupRegistry( Writer );
qRR
qRT
qRE
}


void xulftk::trunk___::_DropSession( void )
{
qRH
	str::string Setup;
	rgstry::registry Setups;
	rgstry::row__ Root = qNIL;
	str::string ProjectId;
qRB

	if ( !Kernel().IsProjectInProgress() )
		qRReturn;

	Setups.Init();

	if ( ( Root = RetrieveSetups_( UI(), Registry(), Kernel().Locale(), Kernel().Language(), Setups ) ) != qNIL ) {
		Setup.Init();
		GetSetup_( Session(), Setup );

		ProjectId.Init();
		GetProjectId_( *this, ProjectId );

		SetSetup_( ProjectId, Setup, Setups, Root );

		StoreSetups_( Setups, Root, UI(), Registry(), Kernel().Locale(), Kernel().Language() );
	}

	Session().CloseSession();

	Handle_( Kernel().CloseProject() );
qRR
qRT
qRE
}

void xulftk::trunk___::FinalizeLaunching( void )
{
qRH
	str::string ProjectFileName;
qRB
	ProjectFileName.Init();

	if ( Kernel().Registry().GetValue( frdrgy::DefaultProject, ProjectFileName ) )
		DefineSession( ProjectFileName );
	else
		GoToHome();
qRR
qRT
qRE
}


const str::string_ &xulftk::trunk___::BuildXSLFileName(
	const char *XSLFileNameAffix,
	str::string_ &FileName )
{
qRH
	fnm::name___ NBuffer;
	TOL_CBUFFER___ CBuffer;
qRB
	NBuffer.Init();
	FileName.Append( fnm::BuildFileName( DefaultXSLRootPath(), XSLFileNameAffix, ".xsl", NBuffer ).UTF8( CBuffer ) );
qRR
qRT
qRE
	return FileName;
}

const str::string_ &xulftk::trunk___::BuildXSLDigestFileName(
	const char *XSLFileNameAffix,
	str::string_ &FileName )
{
qRH
	TOL_CBUFFER___ STRBuffer;
	str::string Buffer;
qRB
	Buffer.Init( XSLFileNameAffix );
	Buffer.Append( "Avaibilities" );

	BuildXSLFileName( Buffer.Convert( STRBuffer ), FileName );
qRR
qRT
qRE
	return FileName;
}
