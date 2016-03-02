/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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

#define SCLMISC__COMPILATION

#include "sclmisc.h"

#include "str.h"
#include "fnm.h"
#include "dir.h"
#include "lcl.h"

#include "scllocale.h"
#include "sclrgstry.h"
#include "sclerror.h"

using namespace sclmisc;

sclerror::error___ *sclerror::SCLERRORError = NULL;

bso::bool__ sclmisc::IsInitialized( void )
{
	return sclerror::SCLERRORError != NULL;
}

static TOL_CBUFFER___ BaseLanguage_;	// De base, le language d'admnistration (lu  partir du fichier de configuration), pouvant devenir language utiliateur selon le contexte.

#define DEFAULT_BASE_LANGUAGE	"en"

#define CONFIGURATION_DEFAULT_FILENAME_SUFFIX ".xcfg"

#define LOCALE_DEFAULT_FILENAME_SUFFIX ".xlcl"

const char *sclmisc::GetBaseLanguage( void )
{
	if ( BaseLanguage_ == NULL )
		return DEFAULT_BASE_LANGUAGE;

	return BaseLanguage_;
}

void sclmisc::RefreshBaseLanguage( void )
{
qRH
	str::string Language;
qRB
Language.Init();

	if ( sclrgstry::BGetValue( sclrgstry::GetCommonRegistry(), sclrgstry::parameter::Language, Language ) )
		Language.Convert( BaseLanguage_ );
qRR
qRT
qRE
}

void sclmisc::ErrFinal( void )
{
	if ( ERRType != err::t_Abort ) {
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		qRH
		qRB
			if ( cio::IsInitialized() ) {
				if ( cio::Target() == cio::tConsole ) {
					cio::COut << txf::commit;
					cio::CErr << txf::nl << txf::tab;
				}

				cio::CErr << "{ " << Message << " }";

				if ( cio::Target() == cio::tConsole )
					cio::CErr << txf::nl;

				cio::CErr << txf::commit;
			} else
				qRFwk();
		qRR
		qRT
		qRE
	} else
		ERRRst();
}



void sclmisc::ReportAndAbort( const lcl::meaning_ &Meaning )
{
	sclerror::SetMeaning( Meaning );
	qRAbort();
}

void sclmisc::ReportAndAbort( const char *Text )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();

	Meaning.SetValue( Text );

	ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void sclmisc::ReportAndAbort(
	const char *Text,
	const str::string_ &Tag )
{
qRH
	lcl::meaning Meaning;
qRB
	Meaning.Init();

	Meaning.SetValue( Text );
	Meaning.AddTag( Tag );

	ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void sclmisc::ReportParsingErrorAndAbort(
	const char *ErrorLabel,
	const rgstry::context___ &Context )
{
qRH
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
qRB
	Meaning.Init();
	Meaning.SetValue( ErrorLabel );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	ReportAndAbort( Meaning );
qRR
qRT
qRE
}

static void Initialize_(
	err::err___ *qRRor,
	sclerror::error___ *SCLError,
	const cio::set__ &CIO )
{
	err::qRRor = qRRor;
	sclerror::SCLERRORError = SCLError;

	cio::target__ Target = cio::GetTarget( CIO );

	if ( Target != cio::tUser )
		cio::Initialize( Target );
	else
		cio::Initialize( CIO );
}

void sclmisc::Initialize(
	err::err___ *qRRor,
	sclerror::error___ *SCLError,
	const cio::set__ &CIO,
	const rgstry::entry__ &Configuration )
{
	Initialize_( qRRor, SCLError, CIO );

	sclrgstry::SetConfiguration( Configuration );
//	scllocale::Set( scllocale::tMain, Locale );
}


static void GetConfigurationLocaleParsingErrorMeaning_(
	const rgstry::context___ &Context,
	lcl::meaning_ &Meaning )
{
qRH
	lcl::meaning MeaningBuffer;
qRB
	Meaning.SetValue( SCLMISC_NAME "_ConfigurationLocaleParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );
qRR
qRT
qRE
}

# if 0	// Obsolete ?
static void MergeLocale_(
	const str::strings_ &SubLocales,
	utf::format__ Format,
	str::string_ &MergedLocale )
{
	ctn::E_CMITEM( str::string_ ) SubLocale;
	sdr::row__ Row = SubLocales.First();

	SubLocale.Init( SubLocales );

	while ( Row != qNIL ) {
		MergedLocale.Append( SubLocale( Row ) );

		Row = SubLocales.Next( Row );
	}
}

static void LoadLocale_(
	scllocale::target__ Target,
	const str::string_ &Locale,
	utf::format__ Format )
{
	qRH
		flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
	qRB
		Flow.Init( Locale );
	XFlow.Init( Flow, Format );

	scllocale::LoadLocale( Target, XFlow, NULL, "Locale" );
	qRR
		qRT
		qRE
}

static void LoadLocale_(
	rgstry::level__ Level,
	scllocale::target__ Target,
	utf::format__ Format )
{
qRH
	str::strings SubLocales;
	str::string MergedLocale;
qRB
	SubLocales.Init();

	sclrgstry::GetCommonRegistry().GetValues( Level, sclrgstry::Locale, SubLocales );

	MergedLocale.Init();
	MergedLocale.Append("<Locale>");

	MergeLocale_( SubLocales, Format, MergedLocale );

	MergedLocale.Append( "</Locale>" );

	LoadLocale_( Target, MergedLocale, Format );
qRR
qRT
qRE
}
# else
static bso::fBool LoadLocale_(
	rgstry::level__ Level,
	scllocale::target__ Target,
	utf::format__ Format )
{
	bso::fBool Found = false;
qRH
	rgstry::entry__ Entry;
	rgstry::row__ Row = qNIL;
qRB
	Row = sclrgstry::GetCommonRegistry().Search( Level, sclrgstry::Locale );

	if ( Row == qNIL )
		qRReturn;

	Entry.Init( Row, sclrgstry::GetCommonRegistry().GetRegistry( Level ) );

	scllocale::Set( Target, Entry );

	Found = true;
qRR
qRT
qRE
	return Found;
}
# endif

static void Initialize_(
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryDirectory )
{
qRH
	str::string LocaleRootPath, RegistryRootPath;
	TOL_CBUFFER___ Buffer;
qRB
	LocaleRootPath.Init();
	sclrgstry::BuildRootPath( "Locale", SCLMISCTargetName, LocaleRootPath );

	RegistryRootPath.Init();
	sclrgstry::BuildRootPath( "Configuration", SCLMISCTargetName, RegistryRootPath );

	scllocale::Load( scllocale::tMain, LocaleFlow, LocaleDirectory, LocaleRootPath.Convert( Buffer ) );

	sclrgstry::LoadConfiguration( RegistryFlow, RegistryDirectory, RegistryRootPath.Convert( Buffer ) );

	RefreshBaseLanguage();

	LoadLocale_( sclrgstry::GetLevel( sclrgstry::nConfiguration ), scllocale::tConfiguration, RegistryFlow.Format() );
qRR
qRT
qRE
}

void sclmisc::Initialize(
	err::err___ *qRRor,
	sclerror::error___ *SCLError,
	const cio::set__ &CIO,
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryDirectory )
{
	Initialize_( qRRor, SCLError, CIO );

	Initialize_( LocaleFlow, LocaleDirectory, RegistryFlow, RegistryDirectory );
}

static bso::bool__ GuessFileName_(
	const fnm::name___ &Affix,
	const fnm::name___ &Suffix,
	const fnm::name___ &SuggestedDirectory,
	fnm::name___ &Filename )
{
	bso::bool__ Success = false;
qRH
	fnm::name___ Dir;
qRB
	Filename.Init();
	fnm::BuildPath( "", Affix, Suffix, Filename );

	if ( !fil::Exists( Filename ) ) {
		Filename.Init();
		fnm::BuildPath( SuggestedDirectory, Affix, Suffix, Filename );

		if ( !fil::Exists( Filename ) ) {
				Filename.Init();
				Dir.Init();
				fnm::BuildPath( dir::GetSelfDir( Dir ), Affix, Suffix, Filename );

				Success = fil::Exists( Filename );
		} else
			Success = true;
	} else
		Success = true;
qRR
qRT
qRE
	return Success;
}

bso::bool__ InitializeFlow_(
	const char *Suffix,
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
	bso::bool__ Success = false;
qRH
	fnm::name___ FileName;
	TOL_CBUFFER___ LocationBuffer;
	fnm::name___ Location;
qRB
	FileName.Init();
	Success = GuessFileName_( SCLMISCTargetName, Suffix, SuggestedDirectory, FileName );

	if ( Success )
		if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess )
			Success = false;

	Location.Init();
	Directory.Append( fnm::GetLocation( FileName, Location ).UTF8( LocationBuffer ) );
qRR
qRT
qRE
	return Success;
}

static flw::iflow__ &InitializeLocaleFlow_(
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
qRH
	lcl::meaning Meaning;
qRB
	if ( !InitializeFlow_( LOCALE_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		Meaning.Init();
		Meaning.SetValue( "" );	// Ne sera pas traduit, puisque la locale n'a pas pu tre lu.
		Meaning.AddTag( "Unable to open locale file" );	// Ceci remplacera le '%0' ci-dessus.
		ReportAndAbort( Meaning );
	}
qRR
qRT
qRE
	return Flow;
}

static flw::iflow__ &InitializeConfigurationFlow_(
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
qRH
	lcl::meaning Meaning;
qRB
	if ( !InitializeFlow_( CONFIGURATION_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		Meaning.Init();
		Meaning.SetValue( SCLMISC_NAME "_UnableToOpenConfigurationFile" );
		ReportAndAbort( Meaning );
	}
qRR
qRT
qRE
	return Flow;
}

namespace {
	void DumpLocale_( str::string_ &Tree )
	{
	qRH
		flx::E_STRING_OFLOW___ SFlow;
		txf::text_oflow__ TFlow;
		xml::writer Writer;
	qRB
		SFlow.Init( Tree );
		TFlow.Init( SFlow );
		Writer.Init( TFlow, xml::oCompact, xml::e_Default );

		scllocale::Dump( scllocale::tMain, true, Writer );
	qRR
	qRT
	qRE
	}
}

void sclmisc::Initialize(
	err::err___ *qRRor,
	sclerror::error___ *SCLError,
	const cio::set__ &CIO,
	const fnm::name___ &SuggestedDirectory,
	str::string_ *Locale )
{
qRH
	flf::file_iflow___ LocaleFlow, ConfigurationFlow;
	xtf::extended_text_iflow__ LocaleXFlow, ConfigurationXFlow;
	str::string LocaleDirectory, ConfigurationDirectory;
	TOL_CBUFFER___ LocaleBuffer, ConfigurationBuffer;
qRB
	Initialize_( qRRor, SCLError, CIO );

	LocaleDirectory.Init();
	InitializeLocaleFlow_( SuggestedDirectory, LocaleFlow, LocaleDirectory );
	LocaleXFlow.Init( LocaleFlow, utf::f_Default );

	ConfigurationDirectory.Init();
	InitializeConfigurationFlow_( SuggestedDirectory, ConfigurationFlow, ConfigurationDirectory );
	ConfigurationXFlow.Init( ConfigurationFlow, utf::f_Default );

	Initialize_( LocaleXFlow, LocaleDirectory.Convert( LocaleBuffer ), ConfigurationXFlow, ConfigurationDirectory.Convert( ConfigurationBuffer ) );

	if ( Locale != NULL )
		DumpLocale_( *Locale );
qRR
qRT
qRE
}

namespace {
	void DumpRegistriesIfRequired_( void )
	{
	qRH
		str::string Registries;
	qRB
		Registries.Init();

		if ( sclmisc::BGetValue( sclrgstry::parameter::debug::DumpRegistries, Registries ) )
			sclmisc::DumpRegistries( Registries, cio::COut );
	qRR
	qRT
	qRE
	}

	void DumpLocalesIfRequired_( void )
	{
	qRH
		str::string Locales;
	qRB
		Locales.Init();

		if ( sclmisc::BGetValue( sclrgstry::parameter::debug::DumpLocales, Locales ) )
			sclmisc::DumpLocales( Locales, cio::COut );
	qRR
	qRT
	qRE
	}
}

void sclmisc::DumpRegistriesAndOrLocalesIfRequired( void )
{
	DumpRegistriesIfRequired_();
	DumpLocalesIfRequired_();
}


void sclmisc::EraseProjectRegistry( void )
{
	sclrgstry::EraseProjectRegistry();
	scllocale::Erase( scllocale::tProject );
}

#define C( name ) case pt##name: return #name; break

const char *sclmisc::GetLabel( project_type__ ProjectType )
{
	switch ( ProjectType ) {
	C( New );
	C( Predefined );
	C( Remote );
	C( Embedded );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

static stsfsm::automat ProjectAutomat_;

static void FillProjectAutomat_( void )
{
	ProjectAutomat_.Init();
	stsfsm::Fill( ProjectAutomat_, pt_amount, GetLabel );
}

project_type__ sclmisc::GetProjectType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, ProjectAutomat_, pt_Undefined, pt_amount );
}



void sclmisc::LoadProject(
	flw::iflow__ &Flow,
	str::string_ &Id )
{
	sclrgstry::LoadProject( Flow, SCLMISCTargetName, Id );

	LoadLocale_( sclrgstry::GetLevel( sclrgstry::nProject ), scllocale::tProject, utf::f_Default );
}

void sclmisc::LoadProject(
	const fnm::name___ &FileName,
	str::string_ &Id )
{
	sclrgstry::LoadProject( FileName, SCLMISCTargetName, Id );

	LoadLocale_( sclrgstry::GetLevel( sclrgstry::nProject ), scllocale::tProject, utf::f_Default );
}

static void LoadProject_( const str::string_ &FileName )
{
qRH
	str::string Id;
qRB
	Id.Init();
	LoadProject( FileName, Id );
qRR
qRT
qRE
}

static void LoadPredefinedProject_( const str::string_ &Id )
{
qRH
	str::string ProjectFileName;
qRB
	if ( Id.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLMISC_NAME "_EmptyPredefinedProjectId" );

	ProjectFileName.Init();
	sclrgstry::MGetValue(sclrgstry::GetCommonRegistry(), rgstry::tentry___( sclrgstry::definition::TaggedProject, Id ), ProjectFileName );

	if ( ProjectFileName.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLMISC_NAME "_NoOrBadProjectFileNameInPredefinedProject", Id );

	LoadProject_( ProjectFileName );
qRR
qRT
qRE
}

void sclmisc::LoadProject(
	project_type__ ProjectType,
	const str::string_ &ProjectFeature )
{
	switch ( ProjectType ) {
	case ptNew:
		sclrgstry::EraseProjectRegistry();
		break;
	case ptPredefined:
		LoadPredefinedProject_( ProjectFeature );
		break;
	case ptRemote:
		if ( ProjectFeature.Amount() == 0  )
			sclmisc::ReportAndAbort( SCLMISC_NAME "_NoProjectFileSelected" );
		LoadProject_( ProjectFeature );
		break;
	case ptEmbedded:
		qRVct();
		break;
	case pt_Undefined:
		qRFwk();
		break;
	default:
		qRFwk();
		break;
	}
}


void sclmisc::LoadProject( void )
{
qRH
	str::string Feature;
	str::string RawType;
	project_type__ Type = pt_Undefined;
qRB
	Feature.Init();
	OGetValue( sclrgstry::parameter::project::Feature, Feature );

	if ( Feature.Amount() != 0 ) {
		RawType.Init();
		MGetValue( sclrgstry::parameter::project::Type, RawType );

		Type = GetProjectType( RawType );

		if ( Type == pt_Undefined )
			sclmisc::ReportAndAbort(SCLMISC_NAME "_BadProjectType" );

		LoadProject( Type, Feature );
	}
qRR
qRT
qRE
}

void sclmisc::CreateBackupFile(
	const fnm::name___ &FileName,
	fil::backup_mode__ Mode )
{
qRH
	fil::backup_status__ Status = fil::bs_Undefined;
	lcl::meaning Meaning;
qRB
	if ( ( Status = fil::CreateBackupFile( FileName, Mode, err::hUserDefined ) )!= fil::bsOK ) {
		Meaning.Init();
		fil::GetMeaning( Status, FileName, Meaning );
		ReportAndAbort( Meaning );
	}
qRR
qRT
qRE
}

void sclmisc::RecoverBackupFile( const fnm::name___ &FileName )
{
qRH
	fil::recover_status__ Status = fil::rs_Undefined;
	lcl::meaning Meaning;
qRB
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Meaning.Init();
		fil::GetMeaning( Status, FileName, Meaning );
		ReportAndAbort( Meaning );
	}
qRR
qRT
qRE
}

void sclmisc::ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName )
{
qRH
	lcl::meaning Meaning;
	TOL_CBUFFER___ Buffer;
qRB
	Meaning.Init();
	Meaning.SetValue( SCLMISC_NAME "_UnableToOpenFile" );
	Meaning.AddTag( FileName.UTF8( Buffer ) );
	ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void sclmisc::Load(
	const fnm::name___ &FileName,
	str::string_ &String )
{
qRH
	flf::file_iflow___ Flow;
qRB
	if ( !Flow.Init(FileName) )
		ReportFileOpeningErrorAndAbort( FileName );

	String.FillWith( Flow );
qRR
qRT
qRE
}

void sclmisc::LoadAndTranslateTags(
	const fnm::name___ &FileName,
	const char *Language,
	str::string_ &String,
	char Marker )
{
	Load( FileName, String );

	scllocale::TranslateTags( String, Language, Marker );
}

static void Load_(
	const rgstry::tentry__ &Entry,
	const sclrgstry::registry_ &Registry,
	str::string_ &String,
	str::string_ &FileName )
{
	sclrgstry::MGetValue( Registry, Entry, FileName );

	Load( FileName, String );
}



void sclmisc::Load(
	const rgstry::tentry__ &Entry,
	const sclrgstry::registry_ &Registry,
	str::string_ &String )
{
qRH
	str::string FileName;
qRB
	FileName.Init();

	Load_( Entry, Registry, String, FileName );
qRR
qRT
qRE
}

void sclmisc::LoadAndTranslateTags(
	const rgstry::tentry__ &FileName,
	const sclrgstry::registry_ &Registry,
	str::string_ &String,
	char Marker )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Load( FileName, Registry, String );

	scllocale::TranslateTags( String, sclrgstry::GetLanguage_( Registry, Buffer ), Marker );
qRR
qRT
qRE

}

void sclmisc::LoadXMLAndTranslateTags(
	const rgstry::tentry__ &FileNameEntry,
	const sclrgstry::registry_ &Registry,
	str::string_ &String,
	char Marker )
{
qRH
	str::string FileName, Unprocessed, Untranslated;
	fnm::name___ FileNameLocation;
	TOL_CBUFFER___ Buffer;
qRB
	FileName.Init();
	Unprocessed.Init();
	Load_( FileNameEntry, Registry, Unprocessed, FileName );

	fnm::GetLocation( FileName, FileNameLocation );

	Untranslated.Init();
	xpp::Process( Unprocessed, xml::oIndent, Untranslated, xpp::criterions___( FileNameLocation ) );

	scllocale::TranslateTags( Untranslated, sclrgstry::GetLanguage_( Registry, Buffer ), String, Marker );
qRR
qRT
qRE
}

sclrgstry::registry_ &sclmisc::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}

txf::text_oflow__ &sclmisc::text_oflow_rack___::Init( const fnm::name___ &FileName )
{
	_FileName.Init( FileName );

	if ( _FileName.IsEmpty() ) {
		_BackedUp = false;
		return cio::COut;
	} else {
		sclmisc::CreateBackupFile( _FileName );
		_BackedUp = true;

		if ( _Flow.Init( _FileName ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( _FileName );

		_TFlow.Init( _Flow );

		return _TFlow;
	}
}

namespace {
	void DumpRegistry_(
		sclrgstry::name__ Name,
		txf::text_oflow__ &Flow	)
	{
		Flow << txf::tab << "----- " << sclrgstry::GetLabel( Name ) << " registry -----" << txf::nl;
		sclmisc::GetRegistry().Dump( sclmisc::GetRegistryLevel( Name ), qNIL, true, xml::oIndent, xml::e_Default, Flow );
	}
}

#define T( c, name )\
	if ( All || ( List.Search( c ) != qNIL ) )\
		DumpRegistry_( sclrgstry::n##name, Flow )

void sclmisc::DumpRegistries(
	const str::string_ &RawList,
	txf::text_oflow__ &Flow )
{
qRH
	bso::bool__ All = false;
	str::string List;
	rgstry::row__ Row = qNIL;
	rgstry::level__ Level = qNIL;
qRB
	List.Init( RawList );
	List.StripCharacter(' ');

	str::ToLower( List );

	if ( List.Amount() == 0 )
		All = true;

	T( 'c', Configuration );
	T( 'p', Project );
	T( 's', Setup );
	T( 'a', Arguments );

	Flow << txf::commit;
qRR
qRT
qRE
}

#undef T

namespace {
	void DumpLocale_(
		scllocale::target__ Target,
		txf::text_oflow__ &Flow	)
	{
		Flow << txf::tab << "----- " << scllocale::GetLabel( Target ) << " locale -----" << txf::nl;
		scllocale::GetLocale().Dump( Target, qNIL, true, xml::oIndent, xml::e_Default, Flow );
	}
}



#define T( c, target )\
	if ( All || ( List.Search( c ) != qNIL ) )\
		DumpLocale_( scllocale::t##target, Flow )

void sclmisc::DumpLocales(
	const str::string_ &RawList,
	txf::text_oflow__ &Flow )
{
qRH
	bso::bool__ All = false;
	str::string List;
	rgstry::row__ Row = qNIL;
	rgstry::level__ Level = qNIL;
qRB
	List.Init( RawList );
	List.StripCharacter(' ');

	str::ToLower( List );

	if ( List.Amount() == 0 )
		All = true;

	T( 'm', Main );
	T( 'c', Configuration );
	T( 'p', Project );

	Flow << txf::commit;
qRR
qRT
qRE
}

#undef T

void sclmisc::text_oflow_rack___::HandleError( void )
{
	if ( _BackedUp )
		sclmisc::RecoverBackupFile( _FileName );
}

static void GetPluginRelatedTags_(
	const char *Target,
	rgstry::tags_ &Tags )
{
qRH
	str::string Id;
qRB
	Tags.Append( str::string( Target ) );

	Id.Init();
	sclmisc::MGetValue( rgstry::tentry___( sclrgstry::parameter::targeted_plugin::Id, Target ), Id );

	Tags.Append( Id );
qRR
qRT
qRE
}

static void GetPluginFeature_(
	const rgstry::tentry__ &Path,
	rgstry::entry__ &Entry )
{
	rgstry::level__ Level = rgstry::UndefinedLevel;

	Entry.Root = GetRegistry().Search( Path, Level );

	if ( Entry.Root == qNIL )
		qRFwk();

	Entry.Registry = &GetRegistry().GetRegistry( Level );
}

const str::string_ &sclmisc::GetPluginFeatures(
	const char *Target,
	str::string_ &Filename,
	rgstry::entry__ &Configuration,
	rgstry::entry__ &Locale,
	str::string_ &Arguments )
{
qRH
	rgstry::tags Tags;
qRB
	Tags.Init();

	GetPluginRelatedTags_( Target, Tags );

	sclmisc::MGetValue( rgstry::tentry__( sclrgstry::definition::plugin::Filename, Tags ), Filename );

	GetPluginFeature_( rgstry::tentry__( sclrgstry::definition::plugin::Configuration, Tags ), Configuration );
	GetPluginFeature_( rgstry::tentry__( sclrgstry::definition::plugin::Locale, Tags ), Locale );

	sclmisc::MGetValue( rgstry::tentry___( sclrgstry::parameter::TargetedPlugin, Target ), Arguments );
qRR
qRT
qRE
	return Filename;
}

namespace {
	void Dump_(
		const rgstry::entry__ &Entry,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_OFLOW___ SFlow;
		txf::text_oflow__ TFlow;
		xml::writer Writer;
	qRB
		SFlow.Init( XML );
		TFlow.Init( SFlow );
		Writer.Init( TFlow, xml::oCompact, xml::e_Default );

		rgstry::Dump( Entry, true, Writer );
	qRR
	qRT
	qRE
	}
}

void sclmisc::HandleLocale_(
	const rgstry::entry__ &Entry,
	const str::string_ &Filename )
{
qRH
	str::string XML;
	fnm::name___ Location;
qRB
	XML.Init();
	Dump_( Entry, XML );

	Location.Init();
	fnm::GetLocation( Filename, Location );

	scllocale::Insert( scllocale::tMain, Location, XML, rgstry::rthIgnore );
qRR
qRT
qRE
}

namespace {
	static void GetPluginItemRelatedTags_(
		const char *Target,
		const str::vString &Id,
		rgstry::tags_ &Tags )
	{
		Tags.Append( str::iString( Target ) );
		Tags.Append( Id );
	}
	
	const void GetPluginItemFeatures_(
		const char *Target,
		const str::string_ &Id,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale,
		str::string_ &Arguments )
	{
	qRH
		rgstry::tags Tags;
	qRB
		Tags.Init();

		GetPluginItemRelatedTags_( Target, Id, Tags );

		sclmisc::MGetValue( rgstry::fTEntry( sclrgstry::definition::plugin::Filename, Tags ), Filename );

		GetPluginFeature_( rgstry::fTEntry( sclrgstry::definition::plugin::Configuration, Tags ), Configuration );
		GetPluginFeature_( rgstry::fTEntry( sclrgstry::definition::plugin::Locale, Tags ), Locale );

		sclmisc::MGetValue( rgstry::rTEntry( sclrgstry::parameter::PluginItem, Target, Id ), Arguments );
	qRR
	qRT
	qRE
	}

	void Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		const str::vString &Id,
		plgn::rLooseRetriever &Retriever )
	{
	qRH
		str::iString Filename, Arguments;
		rgstry::entry__ Configuration, Locale;
	qRB
		Filename.Init();
		Arguments.Init();
		Configuration.Init();
		Locale.Init();

		GetPluginItemFeatures_( Target, Id, Filename, Configuration, Locale, Arguments );
		HandleLocale_( Locale, Filename );

		Retriever.Initialize( Filename, Label, Identifier, Configuration, Arguments );
	qRR
	qRT
	qRE
	}

	void Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		const str::vString &Id,
		plgn::iRetrievers &Retrievers )
	{
	qRH
		plgn::rLooseRetriever Retriever;
	qRB
		Retriever.Init();

		Plug_( Target, Label, Identifier, Id, Retriever );

		Retrievers.Add( Retriever );

		Retriever.reset( false );	// To avoid the deletion, as is it store in 'Retrievers'.
	qRR
	qRT
	qRE
	}

	void Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		const str::vStrings &Ids,
		plgn::iRetrievers &Retrievers )
	{
		ctn::qCMITEMl( str::vString ) Id;
		sdr::fRow Row = Ids.First();

		Id.Init( Ids );

		while ( Row != qNIL ) {
			Plug_( Target, Label, Identifier, Id( Row ), Retrievers );

			Row = Ids.Next( Row );
		}
	}
}

void sclmisc::Plug_(
	const char *Target,
	const char *Label,
	const char *Identifier,
	plgn::iRetrievers &Retrievers )
{
qRH
	str::iStrings Ids;
qRB
	Ids.Init();

	sclmisc::GetValues( rgstry::rTEntry( sclrgstry::parameter::loose_plugin_item::Id, Target ), Ids );

	::Plug_( Target, Label, Identifier, Ids, Retrievers );
qRR
qRT
qRE
}



Q37_GCTOR( sclmisc )
{
	FillProjectAutomat_();
}
