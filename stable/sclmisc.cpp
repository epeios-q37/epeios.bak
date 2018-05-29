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

#define SCLMISC__COMPILATION

#include "sclmisc.h"

#include "str.h"
#include "fnm.h"
#include "dir.h"
#include "lcl.h"

#include "scllocale.h"
#include "sclrgstry.h"
#include "sclerror.h"

#include "plgn.h"

using namespace sclmisc;

using scli::sInfo;

namespace {
	str::wString BinPath_;
}

const str::dString &sclmisc::GetBinPath( void )
{
	return BinPath_;
}

sclerror::rError *sclerror::SCLERRORError = NULL;

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

void sclmisc::SetBaseLanguage( const str::dString &Language )
{
	Language.Convert( BaseLanguage_ );
}

void sclmisc::RefreshBaseLanguage( void )
{
qRH
	str::string Language;
qRB
	Language.Init();
	if ( sclrgstry::BGetValue( GetRegistry(), sclrgstry::parameter::Language, Language ) )
		SetBaseLanguage( Language );
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
	} else if ( sclerror::IsErrorPending() )
		DisplaySCLBasePendingError( cio::CErr );
	else
		ERRRst();
}

bso::bool__ sclmisc::DisplaySCLBasePendingError( txf::sWFlow &Flow )
{
	bso::bool__ Exists = false;
qRH
	str::string Translation;
qRB
	Translation.Init();
	
	if ( ( Exists = sclmisc::GetSCLBasePendingErrorTranslation( Translation ) ) )
		sclerror::ResetPendingError();

	Flow << txf::nl << Translation << txf::nl  << txf::commit;
qRR
qRT
qRE
	return Exists;
}

void sclmisc::ReportAndAbort_( const lcl::meaning_ &Meaning )
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

static void Initialize_( const sRack &Rack )
{
	err::qRRor = Rack.qRRor;
	sclerror::SCLERRORError = Rack.SCLError;

	cio::target__ Target = cio::GetTarget( *Rack.CIO );

	if ( Target != cio::tUser )
		cio::Initialize( Target );
	else
		cio::Initialize( *Rack.CIO );

	scllocale::SetLocale( *Rack.Locale );
}

void sclmisc::Initialize(
	const sRack &Rack,
	const rgstry::entry__ &Configuration )
{
	Initialize_( Rack );

	sclrgstry::Reset( sclrgstry::lMain, Configuration );
//	scllocale::Set( scllocale::tMain, Locale );
}

/*
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
*/
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

namespace {
	bso::sBool GetLocale_(
		rgstry::level__ Level,
		str::dString &Locale )
	{
		bso::sBool Found = false;
	qRH
		rgstry::row__ Row = qNIL;
		flx::E_STRING_OFLOW___ Flow;
		txf::text_oflow__ TFlow;
		xml::writer Writer;
	qRB
		Row = GetRegistry().Search( Level, sclrgstry::Locale );

		if ( Row == qNIL )
			qRReturn;

		Flow.Init( Locale );
		TFlow.Init( Flow );
		Writer.Init( TFlow, xml::lCompact, xml::e_Default );

		GetRegistry().Dump( Level, Row, true, Writer );

		Found = true;
	qRR
	qRT
	qRE
		return Found;
	}

	bso::sBool LoadLocale_(
		rgstry::level__ Level,
		scllocale::target__ Target )
	{
		bso::sBool Found = false;
	qRH
		rgstry::entry__ Entry;
		str::wString Locale;
	qRB
		Locale.Init();

		Found = GetLocale_( Level, Locale );

		if ( Found )
			scllocale::Insert( Target, "", Locale, rgstry::rthIgnore );
	qRR
	qRT
	qRE
		return Found;
	}
}
# endif

namespace {
	bso::sBool GetAppDataConfigurationFilename_(
		const fnm::rName &Path,
		fnm::rName &Name,
		const char *Target,
		const char *Product,
		const char *Organization,
		bso::sBool CreateDir )
	{
		bso::sBool Exist = false;
	qRH
		fnm::rName Buffer, Dir;
		str::wString OrganizationPath;
	qRB
#ifdef CPE_S_WIN
		OrganizationPath.Init( Organization );
#elif defined( CPE_S_POSIX )
		OrganizationPath.Init( "." );
		OrganizationPath.Append( Organization );
#else
# error
#endif
		Buffer.Init();
		fnm::BuildPath( Path, OrganizationPath, "", Buffer );

		Dir.Init();
		fnm::BuildPath( Buffer, Product, "", Dir );

		fnm::BuildPath( Dir, Target, CONFIGURATION_DEFAULT_FILENAME_SUFFIX, Name );

		Exist = fil::Exists( Name );
		if ( CreateDir && !fil::Exists( Dir ) )
			dir::CreateDirWithParents( Dir );
	qRR
	qRT
	qRE
		return Exist;
	}

	bso::sBool GetAppDataConfigurationFilename_(
		const fnm::rName &Path,
		fnm::rName &Name,
		const sInfo &Info,
		bso::sBool CreateDir )
	{
		return GetAppDataConfigurationFilename_( Path, Name, Info.Target(), Info.Product(), Info.Organization(), CreateDir );
	}

	bso::sBool GetAppDataConfigurationFilename_(
		fnm::rName &Name,
		const sInfo &Info,
		bso::sBool CreateDir )
	{
		bso::sBool Exist = false;
	qRH
		fnm::rName Path;
	qRB
		Path.Init();
		dir::GetAppDataPath( Path );

		Exist = GetAppDataConfigurationFilename_( Path, Name, Info, CreateDir );
	qRR
	qRT
	qRE
		return Exist;
	}

	qCDEF(char *, ApplicationDataRootTag_, "Configuration" );

	void LoadAppData_(
		const fnm::rName &Filename,
		sclrgstry::eLevel Level )
	{
	qRH
		flf::rIFlow Flow;
	qRB
		Flow.Init( Filename );
		sclrgstry::Load( Level, Flow, "", "" );
	qRR
	qRT
	qRE
	}

	void LoadAppData_( const sInfo Info )
	{
	qRH
		fnm::rName Filename;
	qRB
		Filename.Init();

		if ( GetAppDataConfigurationFilename_( Filename, Info, false ) )
			LoadAppData_( Filename, sclrgstry::lLasting );
	qRR
	qRT
	qRE
	}

	// Common registry locked upstream.
	void UnconditionalStoreAppData_(
		const fnm::rName &Filename,
		sclrgstry::eLevel Level )
	{
	qRH
		flf::rOFlow Flow;
		txf::sWFlow TFlow;
		xml::wWriter Writer;
		tol::bDateAndTime Buffer;
	qRB
		Flow.Init( Filename );
		TFlow.Init( Flow );
		Writer.Init( TFlow, xml::lIndent, xml::e_Default );

		Writer.PushTag( ApplicationDataRootTag_ );

		Writer.PutAttribute( "Timestamp", tol::DateAndTime( Buffer ) );

		GetRegistry().Dump( Level, qNIL, false, Writer  );

		Writer.PopTag();
	qRR
	qRT
	qRE
	}

	void StoreAppData_(
		const fnm::rName &Filename,
		sclrgstry::eLevel Level )
	{
	qRH
		flf::rOFlow Flow;
		txf::sWFlow TFlow;
		xml::wWriter Writer;
	qRB
		if ( !GetRegistry().IsEmpty( Level ) ) {
			if ( !fil::Exists(Filename) || ( fil::GetLastModificationTime(Filename) <= GetRegistry().TimeStamp( Level ) ) )
				UnconditionalStoreAppData_( Filename, Level );	// Accesses to common registry too, so no unlock.
		} else if ( fil::Exists( Filename ) ) {
			fil::Remove( Filename );
		}
	qRR
	qRT
	qRE
	}

	void StoreAppData_( const sInfo &Info )
	{
	qRH
		fnm::rName Filename;
	qRB
		Filename.Init();

		GetAppDataConfigurationFilename_( Filename, Info, true );

		StoreAppData_( Filename, sclrgstry::lLasting );
	qRR
	qRT
	qRE
	}
}

void sclmisc::StoreLastingRegistry( const sInfo &Info )
{
	StoreAppData_( Info );
}

#define GET( name )	if ( name == NULL ) name = Get##name##Name()

void sclmisc::DumpLastingRegistryFile(
	txf::sWFlow &OFlow,
	const sInfo &Info )
{
qRH
	fnm::rName Name;
	flf::rIFlow IFlow;
	xtf::extended_text_iflow__ XIFlow;
qRB
	Name.Init();

	if ( GetAppDataConfigurationFilename_( Name, Info, false) ) {
		IFlow.Init( Name );
		XIFlow.Init( IFlow, utf::f_Default );
		xpp::Process( XIFlow, xpp::criterions___( "" ), xml::oIndent, OFlow );
	}
qRR
qRT
qRE
}


void sclmisc::DeleteLastingRegistryFile( const sInfo &Info )
{
qRH
	fnm::rName Name;
qRB
	Name.Init();

	if ( GetAppDataConfigurationFilename_( Name, Info, false ) )
		fil::Remove( Name );
qRR
qRT
qRE
}

static void Initialize_(
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryDirectory,
	const fnm::name___ &BinPath,
	const sInfo &Info,
	bso::sBool IgnoreXFiles )
{
qRH
	str::string LocaleRootPath, RegistryRootPath;
	TOL_CBUFFER___ Buffer;
qRB
	if ( !IgnoreXFiles ) {
		LocaleRootPath.Init();
		sclrgstry::BuildRootPath( "Locale", Info.Target(), LocaleRootPath );

		RegistryRootPath.Init();
		sclrgstry::BuildRootPath( "Configuration", Info.Target(), RegistryRootPath );

		scllocale::Load( scllocale::tMain, LocaleFlow, LocaleDirectory );

		sclrgstry::Load( sclrgstry::lMain, RegistryFlow, RegistryDirectory, RegistryRootPath.Convert( Buffer ) );

		RefreshBaseLanguage();

		LoadLocale_( sclrgstry::GetRawLevel( sclrgstry::lMain ), scllocale::tConfiguration );
	}

	LoadAppData_( Info );

	BinPath_.Init();
	BinPath.UTF8( BinPath_ );
qRR
qRT
qRE
}

void sclmisc::Initialize(
	const sRack &Rack,
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryDirectory,
	const fnm::name___ &BinPath,
	const sInfo &Info)
{
	Initialize_( Rack );

	Initialize_( LocaleFlow, LocaleDirectory, RegistryFlow, RegistryDirectory, BinPath, Info, false );
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
	const sInfo &Info,
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
	Success = GuessFileName_( Info.Target(), Suffix, SuggestedDirectory, FileName );

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

namespace {
	bso::sBool InitializeLocaleFlow_(
		const fnm::name___ &SuggestedDirectory,
		const sInfo &Info,
		flf::file_iflow___ &Flow,
		str::string_ &Directory,
		qRPN )
	{
		bso::sBool Success = false;
	qRH
		lcl::meaning Meaning;
	qRB
		Success = InitializeFlow_( LOCALE_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Info, Flow, Directory );

		if ( !Success && qRPT ) {
			Meaning.Init();
			Meaning.SetValue( "" );	// Will not be translated, as the locale file could not be red.
			// Both tags below will replace the '%0' above.
			Meaning.AddTag( "Unable to open locale file" );	
			Meaning.AddTag( Info.Target() );
			ReportAndAbort( Meaning );
		}
	qRR
	qRT
	qRE
		return Success;
	}

	bso::sBool InitializeConfigurationFlow_(
		const fnm::name___ &SuggestedDirectory,
		const sInfo &Info,
		flf::file_iflow___ &Flow,
		str::string_ &Directory,
		qRPN )
	{
		bso::sBool Success = false;
	qRH;
		lcl::meaning Meaning;
	qRB;
		Success = InitializeFlow_( CONFIGURATION_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Info, Flow, Directory );

		if ( !Success && qRPT ) {
			Meaning.Init();
			Meaning.SetValue( SCLMISC_NAME "_UnableToOpenConfigurationFile" );
			Meaning.AddTag( Info.Target() );
			ReportAndAbort( Meaning );
		}
	qRR;
	qRT;
	qRE;
		return Success;
	}
}


/*
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
*/
void sclmisc::Initialize(
	const sRack &Rack,
	const fnm::name___ &BinPath,
	const sInfo &Info,
	qRPN )
{
qRH
	flf::file_iflow___ LocaleFlow, ConfigurationFlow;
	xtf::extended_text_iflow__ LocaleXFlow, ConfigurationXFlow;
	str::string LocaleDirectory, ConfigurationDirectory;
	TOL_CBUFFER___ LocaleBuffer, ConfigurationBuffer;
	bso::sBool XFilesPresent = false;
qRB
	Initialize_( Rack );

	LocaleDirectory.Init();
	XFilesPresent = InitializeLocaleFlow_( BinPath, Info, LocaleFlow, LocaleDirectory, qRP );

	if ( XFilesPresent )
		LocaleXFlow.Init( LocaleFlow, utf::f_Default );

	ConfigurationDirectory.Init();
	XFilesPresent = XFilesPresent && InitializeConfigurationFlow_( BinPath, Info, ConfigurationFlow, ConfigurationDirectory, qRP );

	if ( XFilesPresent )
		ConfigurationXFlow.Init( ConfigurationFlow, utf::f_Default );

	Initialize_( LocaleXFlow, LocaleDirectory.Convert( LocaleBuffer ), ConfigurationXFlow, ConfigurationDirectory.Convert( ConfigurationBuffer ), BinPath, Info, !XFilesPresent );
qRR
qRT
qRE
}

void sclmisc::Quit( const sInfo &Info )
{
	if ( IsInitialized() ) {
		StoreLastingRegistry( Info );
	}
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
	sclrgstry::Erase( sclrgstry::lProject );
	scllocale::Erase( scllocale::tProject );
}

#define C( name ) case pt##name: return #name; break

const char *sclmisc::GetLabel( eProjectType ProjectType )
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

eProjectType sclmisc::GetProjectType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, ProjectAutomat_, pt_Undefined, pt_amount );
}



void sclmisc::LoadProject(
	flw::iflow__ &Flow,
	const fnm::name___ &Directory,
	const sInfo &Info,
	str::string_ &Id )
{
	sclrgstry::LoadProject( Flow, Info.Target(), Directory, Id );

	LoadLocale_( sclrgstry::GetRawLevel( sclrgstry::lProject ), scllocale::tProject );
}

void sclmisc::LoadProject(
	const fnm::name___ &FileName,
	const sInfo &Info,
	str::string_ &Id )
{
	sclrgstry::LoadProject( FileName, Info.Target(), Id );

	LoadLocale_( sclrgstry::GetRawLevel( sclrgstry::lProject ), scllocale::tProject );
}

static void LoadProject_(
	const str::string_ &FileName,
	const sInfo &Info )
{
qRH
	str::string Id;
qRB
	Id.Init();
	LoadProject( FileName, Info, Id );
qRR
qRT
qRE
}

static void LoadPredefinedProject_(
	const str::string_ &Id,
	const sInfo &Info )
{
qRH
	str::string ProjectFileName;
qRB
	if ( Id.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLMISC_NAME "_EmptyPredefinedProjectId" );

	ProjectFileName.Init();

	sclrgstry::MGetValue( GetRegistry(), rgstry::tentry___( sclrgstry::definition::TaggedProject, Id ), ProjectFileName );

	if ( ProjectFileName.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLMISC_NAME "_NoOrBadProjectFileNameInPredefinedProject", Id );

	LoadProject_( ProjectFileName, Info );
qRR
qRT
qRE
}

void sclmisc::LoadProject(
	eProjectType ProjectType,
	const str::string_ &ProjectFeature,
	const sInfo &Info )
{
	switch ( ProjectType ) {
	case ptNew:
		sclrgstry::Erase( sclrgstry::lProject );
		break;
	case ptPredefined:
		LoadPredefinedProject_( ProjectFeature, Info );
		break;
	case ptRemote:
		if ( ProjectFeature.Amount() == 0  )
			sclmisc::ReportAndAbort( SCLMISC_NAME "_NoProjectFileSelected" );
		LoadProject_( ProjectFeature, Info );
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


void sclmisc::LoadProject( const sInfo &Info )
{
qRH
	str::string Feature;
	str::string RawType;
	eProjectType Type = pt_Undefined;
qRB
	Feature.Init();
	OGetValue( sclrgstry::parameter::project::Feature, Feature );

	if ( Feature.Amount() != 0 ) {
		RawType.Init();
		MGetValue( sclrgstry::parameter::project::Type, RawType );

		Type = GetProjectType( RawType );

		if ( Type == pt_Undefined )
			sclmisc::ReportAndAbort(SCLMISC_NAME "_BadProjectType" );

		LoadProject( Type, Feature, Info );
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
	if ( !Flow.Init( FileName, err::hUserDefined ) )
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
	const fnm::rName &Filename,
	const char *Language,
	str::string_ &String,
	char Marker )
{
qRH;
	str::string Unprocessed, Untranslated;
	fnm::name___ FileNameLocation;
qRB;
	Unprocessed.Init();
	Load( Filename, Unprocessed );

	fnm::GetLocation( Filename, FileNameLocation );

	Untranslated.Init();
	xpp::Process( Unprocessed, xml::oIndent, Untranslated, xpp::criterions___( FileNameLocation ) );

	scllocale::TranslateTags( Untranslated, Language, String, Marker );
qRR;
qRT;
qRE;
}

void sclmisc::LoadXMLAndTranslateTags(
	const rgstry::tentry__ &FilenameEntry,
	const sclrgstry::registry_ &Registry,
	str::string_ &String,
	char Marker )
{
qRH;
	str::string Filename;
	TOL_CBUFFER___ Buffer;
qRB;
	Filename.Init();
	sclrgstry::MGetValue( Registry, FilenameEntry, Filename );

	LoadXMLAndTranslateTags( Filename, sclrgstry::GetLanguage_( Registry, Buffer ), String, Marker );
qRR;
qRT;
qRE;
}

const sclrgstry::registry_ &sclmisc::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}

sclrgstry::registry_ &sclmisc::GetRWRegistry( void )
{
	return sclrgstry::GetRWCommonRegistry();
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
	// Common registry locked upstream.
	void DumpRegistry_(
		sclrgstry::eLevel Level,
		txf::text_oflow__ &Flow	)
	{
		Flow << txf::tab << "----- " << sclrgstry::GetLabel( Level ) << " registry -----" << txf::nl;
		sclmisc::GetRegistry().Dump( sclmisc::GetRegistryRawLevel( Level ), qNIL, true, xml::oIndent, xml::e_Default, Flow );
	}
}

#define T( c, name )\
	if ( All || ( List.Search( c ) != qNIL ) )\
		DumpRegistry_( sclrgstry::l##name, Flow )

void sclmisc::DumpRegistries(
	const str::string_ &RawList,
	txf::text_oflow__ &Flow )
{
qRH
	bso::bool__ All = false;
	str::string List;
qRB
	List.Init( RawList );
	List.StripCharacter(' ');

	str::ToLower( List );

	if ( List.Amount() == 0 )
		All = true;

	T( 'm', Main );
	T( 'l', Lasting );
	T( 'p', Project );
	T( 's', Setup );
	T( 'a', Arguments );
	T( 'r', Runtime );

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

namespace {
	 inline void FillPluginRelatedTags_(
		const char *Target,
		const str::dString &Id,
		rgstry::tags_ &Tags )
	{
		Tags.Append( str::string( Target ) );
		Tags.Append( Id );
	}

	 void GetPluginRelatedTags_(
		const char *Target,
		rgstry::tags_ &Tags )
	{
	qRH
		str::string Id;
	qRB
		Id.Init();
		sclmisc::MGetValue( rgstry::tentry___( sclrgstry::parameter::targeted_plugin::Id, Target ), Id );

		FillPluginRelatedTags_( Target, Id, Tags );
	qRR
	qRT
	qRE
	}
}

static void GetPluginFeature_(
	const rgstry::tentry__ &Path,
	rgstry::entry__ &Entry )
{
qRH
	rgstry::level__ Level = rgstry::UndefinedLevel;
	rgstry::hLock Lock;
qRB
	Entry.Root = GetRegistry().Search( Path, Level );

	if ( Entry.Root == qNIL )
		qRFwk();

	Entry.Registry = &GetRegistry().GetRegistry( Level, Lock );
	Entry.Locker = GetRegistry().GetLocker( Level );
qRR
qRT
qRE
}

namespace {
	const str::string_ &GetPluginFeatures_(
		const char *Target,
		const rgstry::tags_ &Tags,
		str::string_ &Filename,
		rgstry::entry__ &Configuration,
		rgstry::entry__ &Locale )
	{
		sclmisc::MGetValue( rgstry::tentry__( sclrgstry::definition::plugin::Filename, Tags ), Filename );

		GetPluginFeature_( rgstry::tentry__( sclrgstry::definition::plugin::Configuration, Tags ), Configuration );
		GetPluginFeature_( rgstry::tentry__( sclrgstry::definition::plugin::Locale, Tags ), Locale );

		return Filename;
	}
}

const str::string_ &sclmisc::GetPluginFeatures(
	const char *Target,
	const str::dString &Id,
	str::string_ &Filename,
	rgstry::entry__ &Configuration,
	rgstry::entry__ &Locale )
{
qRH
	rgstry::tags Tags;
qRB
	Tags.Init();

	FillPluginRelatedTags_( Target, Id, Tags );

	GetPluginFeatures_( Target, Tags, Filename, Configuration, Locale );
qRR
qRT
qRE
	return Filename;
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

	GetPluginFeatures_( Target, Tags, Filename, Configuration, Locale );
	
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
	void GetPluginItemRelatedTags_(
		const char *Target,
		const str::dString &Id,
		rgstry::tags_ &Tags )
	{
		Tags.Append( str::wString( Target ) );
		Tags.Append( Id );
	}
	
	void GetPluginItemFeatures_(
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

		sclmisc::MGetValue( rgstry::sTEntry( sclrgstry::definition::plugin::Filename, Tags ), Filename );

		GetPluginFeature_( rgstry::sTEntry( sclrgstry::definition::plugin::Configuration, Tags ), Configuration );
		GetPluginFeature_( rgstry::sTEntry( sclrgstry::definition::plugin::Locale, Tags ), Locale );

		sclmisc::MGetValue( rgstry::rTEntry( sclrgstry::parameter::PluginItem, Target, Id ), Arguments );
	qRR
	qRT
	qRE
	}

	sdr::sRow Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		const str::dString &Id,
		const plgn::dAbstracts &Abstracts,
		plgn::rLooseRetriever &Retriever )
	{
		sdr::sRow Row = qNIL;
	qRH
		str::wString Filename, Arguments;
		rgstry::entry__ Configuration, LocaleContent;
	qRB
		Filename.Init();
		Arguments.Init();
		Configuration.Init();
		LocaleContent.Init();

		GetPluginItemFeatures_( Target, Id, Filename, Configuration, LocaleContent, Arguments );

		if ( ( Row = Retriever.Initialize( Filename, Label, Identifier, Configuration, Arguments, Abstracts ) ) == qNIL )
			HandleLocale_( LocaleContent, Filename );
	qRR
	qRT
	qRE
		return Row;
	}

	void Plug_(
		const char *Target,
		const char *Label,
		const char *Identifier,
		const str::dString &Id,
		plgn::dRetrievers &Retrievers )
	{
	qRH
		plgn::rLooseRetriever Retriever;
	qRB
		Retriever.Init();

		if ( Plug_( Target, Label, Identifier, Id, plgn::EmptyAbstracts, Retriever ) != qNIL )
			qRFwk();

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
		const str::dStrings &Ids,
		plgn::dRetrievers &Retrievers )
	{
		sdr::sRow Row = Ids.First();

		while ( Row != qNIL ) {
			Plug_( Target, Label, Identifier, Ids( Row ), Retrievers );

			Row = Ids.Next( Row );
		}
	}
}

void sclmisc::Plug_(
	const char *Target,
	const char *Label,
	const char *Identifier,
	plgn::dRetrievers &Retrievers )
{
qRH
	str::wStrings Ids;
qRB
	Ids.Init();

	sclmisc::GetValues( rgstry::rTEntry( sclrgstry::parameter::loose_plugin_item::Id, Target ), Ids );

	::Plug_( Target, Label, Identifier, Ids, Retrievers );
qRR
qRT
qRE
}

fdr::rWDriver &sclmisc::rODriverRack::Init( const fnm::name___ &FileName )
{
	Filename_.Init( FileName );

	if ( Filename_.IsEmpty() ) {
		BackedUp_ = false;
		return cio::GetOutDriver();
	} else {
		sclmisc::CreateBackupFile( Filename_ );
		BackedUp_ = true;

		if ( Driver_.Init( Filename_, qRPU ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Filename_ );

		return Driver_;
	}
}

void sclmisc::rODriverRack::HandleError( void )
{
	if ( BackedUp_ )
		sclmisc::RecoverBackupFile( Filename_ );
}


fdr::rRDriver &sclmisc::rIDriverRack::Init( const fnm::name___ &FileName )
{
	Filename_.Init( FileName );

	if ( Filename_.IsEmpty() ) {
		return cio::GetInDriver();
	} else {
		if ( Driver_.Init( Filename_, qRPU ) != tol::rSuccess )
			sclmisc::ReportFileOpeningErrorAndAbort( Filename_ );

		return Driver_;
	}
}

namespace {
	inline void signal_( int s )
	{
		exit( EXIT_SUCCESS );
	}
}

void sclmisc::ExitOnSignal( void )
{
#ifdef CPE_S_POSIX
	signal( SIGHUP, signal_ );
#elif defined( CPE_S_WIN )
	signal( SIGBREAK, signal_ );
	_set_abort_behavior( 0, _WRITE_ABORT_MSG );
#else
#	error "Undefined target !"
#endif
	signal( SIGTERM, signal_ );
	signal( SIGABRT, signal_ );
	signal( SIGINT, signal_ );	// Documentations about this signal not very clear, but this handles Ctrl-C.
}

Q37_GCTOR( sclmisc )
{
	BinPath_.Init();
	FillProjectAutomat_();
}
