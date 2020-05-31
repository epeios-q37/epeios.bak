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

#define SCLM_COMPILATION_

#include "sclm.h"

#include "dir.h"
#include "mtk.h"

using namespace sclm;

using scli::sInfo;

namespace {
	str::wString BinPath_;
}

const str::dString &sclm::GetBinPath( void )
{
	return BinPath_;
}

scle::rError *scle::SCLEError = NULL;

bso::bool__ sclm::IsInitialized( void )
{
	return scle::SCLEError != NULL;
}

static TOL_CBUFFER___ BaseLanguage_;	// De base, le language d'admnistration (lu  partir du fichier de configuration), pouvant devenir language utiliateur selon le contexte.

#define DEFAULT_BASE_LANGUAGE	"en"

#define CONFIGURATION_DEFAULT_FILENAME_SUFFIX ".xcfg"

#define LOCALE_DEFAULT_FILENAME_SUFFIX ".xlcl"

const char *sclm::GetBaseLanguage( void )
{
	if ( BaseLanguage_ == NULL )
		return DEFAULT_BASE_LANGUAGE;

	return BaseLanguage_;
}

void sclm::SetBaseLanguage( const str::dString &Language )
{
	Language.Convert( BaseLanguage_ );
}

void sclm::RefreshBaseLanguage( void )
{
qRH
	str::string Language;
qRB
	Language.Init();
	if ( sclr::OGetValue( GetRegistry(), sclr::parameter::Language, Language ) )
		SetBaseLanguage( Language );
qRR
qRT
qRE
}

void sclm::ErrorDefaultHandling(void)
{
	if ( ERRType != err::t_Abort ) {
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		qRH
		qRB
			if ( cio::IsInitialized() ) {
				if ( cio::Type() == cio::tTerminal ) {
					cio::COut << txf::commit;
					cio::CErr << txf::nl << txf::tab;
				}

				cio::CErr << "{ " << Message << " }";

				if ( cio::Type() == cio::tTerminal )
					cio::CErr << txf::nl;

				cio::CErr << txf::commit;
			} else
				qRFwk();
		qRR
		qRT
		qRE
	} else if ( scle::IsErrorPending() )
		DisplaySCLBasePendingError( cio::CErr );
	else
		ERRRst();
}

bso::bool__ sclm::DisplaySCLBasePendingError( txf::sWFlow &Flow )
{
	bso::bool__ Exists = false;
qRH
	str::string Translation;
qRB
	Translation.Init();

	if ( ( Exists = sclm::GetSCLBasePendingErrorTranslation( Translation ) ) )
		scle::ResetPendingError();

	Flow << txf::nl << Translation << txf::nl  << txf::commit;
qRR
qRT
qRE
	return Exists;
}

void sclm::ReportAndAbort_( const lcl::meaning_ &Meaning )
{
	scle::SetMeaning( Meaning );
	qRAbort();
}

void sclm::ReportAndAbort( const char *Text )
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

void sclm::ReportParsingErrorAndAbort(
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
	scle::SCLEError = Rack.SCLError;

	if ( Rack.CIO->Type() == cio::tVoid )
		cio::Initialize( cio::tVoid );
	else
		cio::Initialize( *Rack.CIO );

	scll::SetLocale( *Rack.Locale );
}

void sclm::Initialize(
	const sRack &Rack,
	const rgstry::entry__ &Configuration )
{
	Initialize_( Rack );

	sclr::Reset( sclr::lMain, Configuration );
}

namespace {
	bso::sBool GetLocale_(
		rgstry::layer__ Layer,
		str::dString &Locale )
	{
		bso::sBool Found = false;
	qRH
		rgstry::row__ Row = qNIL;
		flx::E_STRING_OFLOW___ Flow;
		txf::text_oflow__ TFlow;
		xml::rWriter Writer;
	qRB
		Row = GetRegistry().Search( Layer, sclr::Locale );

		if ( Row != qNIL ) {
			Flow.Init( Locale );
			TFlow.Init( Flow );
			Writer.Init( TFlow, xml::lCompact, xml::e_Default );

			GetRegistry().Dump( Layer, Row, true, Writer );

			Found = true;
		}
	qRR
	qRT
	qRE
		return Found;
	}

	bso::sBool LoadLocale_(
		rgstry::layer__ Layer,
		scll::target__ Target )
	{
		bso::sBool Found = false;
	qRH
		rgstry::entry__ Entry;
		str::wString Locale;
	qRB
		Locale.Init();

		Found = GetLocale_( Layer, Locale );

		if ( Found )
			scll::Insert( Target, "", Locale, rgstry::rthIgnore );
	qRR
	qRT
	qRE
		return Found;
	}
}

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
		sclr::eLayer Layer )
	{
	qRH
		flf::rIFlow Flow;
	qRB
		Flow.Init( Filename );
		sclr::Load( Layer, Flow, "", "" );
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
			LoadAppData_( Filename, sclr::lLasting );
	qRR
	qRT
	qRE
	}

	// Common registry locked upstream.
	void UnconditionalStoreAppData_(
		const fnm::rName &Filename,
		sclr::eLayer Layer )
	{
	qRH
		flf::rOFlow Flow;
		txf::sWFlow TFlow;
		xml::rWriter Writer;
		tol::bDateAndTime Buffer;
	qRB
		Flow.Init( Filename );
		TFlow.Init( Flow );
		Writer.Init( TFlow, xml::lIndent, xml::e_Default );

		Writer.PushTag( ApplicationDataRootTag_ );

		Writer.PutAttribute( "Timestamp", tol::DateAndTime( Buffer ) );

		GetRegistry().Dump( Layer, qNIL, false, Writer  );

		Writer.PopTag();
	qRR
	qRT
	qRE
	}

	void StoreAppData_(
		const fnm::rName &Filename,
		sclr::eLayer Layer )
	{
	qRH
		flf::rOFlow Flow;
		txf::sWFlow TFlow;
		xml::rWriter Writer;
	qRB
		if ( !GetRegistry().IsEmpty( Layer ) ) {
			if ( !fil::Exists(Filename) || ( fil::GetLastModificationTime(Filename) <= GetRegistry().TimeStamp( Layer ) ) )
				UnconditionalStoreAppData_( Filename, Layer );	// Accesses to common registry too, so no unlock.
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

		StoreAppData_( Filename, sclr::lLasting );
	qRR
	qRT
	qRE
	}
}

void sclm::StoreLastingRegistry( const sInfo &Info )
{
	StoreAppData_( Info );
}

#define GET( name )	if ( name == NULL ) name = Get##name##Name()

void sclm::DumpLastingRegistryFile(
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


void sclm::DeleteLastingRegistryFile( const sInfo &Info )
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
		sclr::BuildRootPath( "Locale", Info.Target(), LocaleRootPath );

		RegistryRootPath.Init();
		sclr::BuildRootPath( "Configuration", Info.Target(), RegistryRootPath );

		scll::Load( scll::tMain, LocaleFlow, LocaleDirectory );

		sclr::Load( sclr::lMain, RegistryFlow, RegistryDirectory, RegistryRootPath.Convert( Buffer ) );

		RefreshBaseLanguage();

		LoadLocale_( sclr::GetRawLayer( sclr::lMain ), scll::tConfiguration );
	}

	LoadAppData_( Info );

	BinPath_.Init();
	BinPath.UTF8( BinPath_ );
qRR
qRT
qRE
}

void sclm::Initialize(
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
			Meaning.SetValue( SCLM_NAME "_UnableToOpenConfigurationFile" );
			Meaning.AddTag( Info.Target() );
			ReportAndAbort( Meaning );
		}
	qRR;
	qRT;
	qRE;
		return Success;
	}
}

void sclm::Initialize(
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

void sclm::Quit( const sInfo &Info )
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

		if ( sclm::BGetValue(sclr::parameter::debug::DumpRegistries, Registries,qRPU) )
			sclm::DumpRegistries( Registries, cio::COut );
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

		if ( sclm::BGetValue(sclr::parameter::debug::DumpLocales, Locales, qRPU) )
			sclm::DumpLocales( Locales, cio::COut );
	qRR
	qRT
	qRE
	}
}

void sclm::DumpRegistriesAndOrLocalesIfRequired( void )
{
	DumpRegistriesIfRequired_();
	DumpLocalesIfRequired_();
}


void sclm::EraseProjectRegistry( void )
{
	sclr::Erase( sclr::lProject );
	scll::Erase( scll::tProject );
}

#define C( name ) case pt##name: return #name; break

const char *sclm::GetLabel( eProjectType ProjectType )
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

eProjectType sclm::GetProjectType( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, ProjectAutomat_, pt_Undefined, pt_amount );
}



void sclm::LoadProject(
	flw::iflow__ &Flow,
	const fnm::name___ &Directory,
	const sInfo &Info,
	str::string_ &Id )
{
	sclr::LoadProject( Flow, Info.Target(), Directory, Id );

	LoadLocale_( sclr::GetRawLayer( sclr::lProject ), scll::tProject );
}

void sclm::LoadProject(
	const fnm::name___ &FileName,
	const sInfo &Info,
	str::string_ &Id )
{
	sclr::LoadProject( FileName, Info.Target(), Id );

	LoadLocale_( sclr::GetRawLayer( sclr::lProject ), scll::tProject );
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
		sclm::ReportAndAbort( SCLM_NAME "_EmptyPredefinedProjectId" );

	ProjectFileName.Init();

	sclr::MGetValue( GetRegistry(), rgstry::tentry___( sclr::definition::TaggedProject, Id ), ProjectFileName );

	if ( ProjectFileName.Amount() == 0 )
		sclm::ReportAndAbort( SCLM_NAME "_NoOrBadProjectFileNameInPredefinedProject", Id );

	LoadProject_( ProjectFileName, Info );
qRR
qRT
qRE
}

void sclm::LoadProject(
	eProjectType ProjectType,
	const str::string_ &ProjectFeature,
	const sInfo &Info )
{
	switch ( ProjectType ) {
	case ptNew:
		sclr::Erase( sclr::lProject );
		break;
	case ptPredefined:
		LoadPredefinedProject_( ProjectFeature, Info );
		break;
	case ptRemote:
		if ( ProjectFeature.Amount() == 0  )
			sclm::ReportAndAbort( SCLM_NAME "_NoProjectFileSelected" );
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


void sclm::LoadProject( const sInfo &Info )
{
qRH
	str::string Feature;
	str::string RawType;
	eProjectType Type = pt_Undefined;
qRB
	Feature.Init();
	OGetValue( sclr::parameter::project::Feature, Feature );

	if ( Feature.Amount() != 0 ) {
		RawType.Init();
		if ( OGetValue( sclr::parameter::project::Type, RawType ) )
            Type = GetProjectType( RawType );
        else
            Type = ptRemote;

		if ( Type == pt_Undefined )
			sclm::ReportAndAbort(SCLM_NAME "_BadProjectType" );

		LoadProject( Type, Feature, Info );
	}
qRR
qRT
qRE
}

void sclm::CreateBackupFile(
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

void sclm::RecoverBackupFile( const fnm::name___ &FileName )
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

void sclm::ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName )
{
qRH
	lcl::meaning Meaning;
	TOL_CBUFFER___ Buffer;
qRB
	Meaning.Init();
	Meaning.SetValue( SCLM_NAME "_UnableToOpenFile" );
	Meaning.AddTag( FileName.UTF8( Buffer ) );
	ReportAndAbort( Meaning );
qRR
qRT
qRE
}

void sclm::Load(
	const fnm::name___ &FileName,
	str::string_ &Content )
{
qRH
	flf::file_iflow___ Flow;
qRB
	if ( !Flow.Init( FileName, err::hUserDefined ) )
		ReportFileOpeningErrorAndAbort( FileName );

	Content.FillWith( Flow );
qRR
qRT
qRE
}

void sclm::LoadAndTranslateTags(
	const fnm::name___ &FileName,
	const char *Language,
	str::string_ &Content,
	char Marker )
{
	Load( FileName, Content );

	scll::TranslateTags( Content, Language, Marker );
}

static bso::sBool Load_(
	const rgstry::tentry__ &Entry,
	const sclr::registry_ &Registry,
	str::string_ &Content,
	str::string_ &FileName,
	qRPN )
{
	if ( sclr::BGetValue( Registry, Entry,Content, qRP ) ) {
		Load( FileName, Content );
		return true;
	} else
		return false;
}

bso::sBool sclm::Load(
	const rgstry::tentry__ &Entry,
	const sclr::registry_ &Registry,
	str::string_ &Content,
	qRPN )
{
	bso::sBool Result = false;
qRH
	str::string FileName;
qRB
	FileName.Init();

	Load_( Entry, Registry, Content, FileName, qRP );
qRR
qRT
qRE
	return Result;
}

bso::sBool sclm::LoadAndTranslateTags(
	const rgstry::tentry__ &FileName,
	const sclr::registry_ &Registry,
	str::string_ &Content,
	char Marker,
	qRPN )
{
	bso::sBool Found = false;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	if ( ( Found = Load( FileName, Registry, Content, qRP ) ) )
		scll::TranslateTags( Content, sclr::GetLanguage( Registry, Buffer ), Marker );
qRR
qRT
qRE
return Found;
}

void sclm::LoadXMLAndTranslateTags(
	const fnm::rName &Filename,
	const char *Language,
	str::string_ &Content,
	xml::sLevel StartLevel,
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
	xpp::Process( Unprocessed, xml::oIndent, Untranslated, xpp::criterions___( FileNameLocation, StartLevel ) );

	scll::TranslateTags( Untranslated, Language, Content, Marker );
qRR;
qRT;
qRE;
}

bso::sBool sclm::LoadXMLAndTranslateTags(
	const rgstry::tentry__ &FilenameEntry,
	const sclr::registry_ &Registry,
	str::string_ &Content,
	xml::sLevel StartLevel,
	char Marker,
	qRPN )
{
	bso::sBool Found = false;
qRH;
	str::string Filename;
	TOL_CBUFFER___ Buffer;
qRB;
	Filename.Init();

	if ( ( Found = sclr::BGetValue( Registry, FilenameEntry, Filename, qRP ) ) )
		LoadXMLAndTranslateTags( Filename, sclr::GetLanguage( Registry, Buffer ), Content, StartLevel, Marker );
qRR;
qRT;
qRE;
	return Found;
}

const sclr::registry_ &sclm::GetRegistry( void )
{
	return sclr::GetCommonRegistry();
}

sclr::registry_ &sclm::GetRWRegistry( void )
{
	return sclr::GetRWCommonRegistry();
}

flf::rWFlow &sclm::rWFlowRack::Init( const fnm::name___ &FileName )
{
	_FileName.Init( FileName );

    sclm::CreateBackupFile( _FileName );
    _BackedUp = true;

    if ( _Flow.Init( _FileName ) != tol::rSuccess )
        sclm::ReportFileOpeningErrorAndAbort( _FileName );

    return _Flow;
}

txf::text_oflow__ &sclm::text_oflow_rack___::Init( const fnm::name___ &FileName )
{
	if ( FileName.IsEmpty() ) {
		return cio::COut;
	} else {
		_TFlow.Init( rWFlowRack::Init( FileName ) );

		return _TFlow;
	}
}

namespace {
	// Common registry locked upstream.
	void DumpRegistry_(
		sclr::eLayer Layer,
		txf::text_oflow__ &Flow	)
	{
		Flow << txf::tab << "----- " << sclr::GetLabel( Layer ) << " registry -----" << txf::nl;
		sclm::GetRegistry().Dump( sclm::GetRegistryRawLayer( Layer ), qNIL, true, xml::oIndent, xml::e_Default, Flow );
	}
}

#define T( c, name )\
	if ( All || ( List.Search( c ) != qNIL ) )\
		DumpRegistry_( sclr::l##name, Flow )

void sclm::DumpRegistries(
	const str::string_ &RawList,
	txf::text_oflow__ &Flow )
{
qRH
	bso::bool__ All = false;
	str::string List;
qRB
	List.Init( RawList );
	List.StripChars(' ');

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
		scll::target__ Target,
		txf::text_oflow__ &Flow	)
	{
		Flow << txf::tab << "----- " << scll::GetLabel( Target ) << " locale -----" << txf::nl;
		scll::GetLocale().Dump( Target, qNIL, true, xml::oIndent, xml::e_Default, Flow );
	}
}



#define T( c, target )\
	if ( All || ( List.Search( c ) != qNIL ) )\
		DumpLocale_( scll::t##target, Flow )

void sclm::DumpLocales(
	const str::string_ &RawList,
	txf::text_oflow__ &Flow )
{
qRH
	bso::bool__ All = false;
	str::string List;
qRB
	List.Init( RawList );
	List.StripChars(' ');

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

void sclm::rWFlowRack::HandleError( void )
{
	if ( _BackedUp )
		sclm::RecoverBackupFile( _FileName );
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

	 bso::sBool GetPluginRelatedTags_(
		const char *Target,
		rgstry::tags_ &Tags )
	{
		 bso::sBool Exists = false;
	qRH
		str::string Id;
	qRB
		Id.Init();
		if ( ( Exists = sclm::OGetValue( rgstry::tentry___( sclr::parameter::targeted_plugin::Id, Target ), Id ) ) )
			FillPluginRelatedTags_( Target, Id, Tags );
	qRR
	qRT
	qRE
		return Exists;
	}
}

static void GetPluginFeature_(
	const rgstry::tentry__ &Path,
	rgstry::entry__ &Entry )
{
qRH
	rgstry::layer__ Layer = rgstry::UndefinedLayer;
	rgstry::hLock Lock;
qRB
	Entry.Root = GetRegistry().Search( Path, Layer );

	if ( Entry.Root == qNIL )
		qRFwk();

	Entry.Registry = &GetRegistry().GetRegistry( Layer, Lock );
	Entry.Locker = GetRegistry().GetLocker( Layer );
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
		sclm::MGetValue( rgstry::tentry__( sclr::definition::plugin::Filename, Tags ), Filename );

		GetPluginFeature_( rgstry::tentry__( sclr::definition::plugin::Configuration, Tags ), Configuration );
		GetPluginFeature_( rgstry::tentry__( sclr::definition::plugin::Locale, Tags ), Locale );

		return Filename;
	}
}

const str::string_ &sclm::GetPluginFeatures(
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


bso::sBool sclm::GetPluginFeatures(
	const char *Target,
	str::string_ &Filename,
	rgstry::entry__ &Configuration,
	rgstry::entry__ &Locale,
	str::string_ &Arguments,
	qRPN )
{
	bso::sBool Exists = false;
qRH
	rgstry::tags Tags;
qRB
	Tags.Init();

	if ( ( Exists = GetPluginRelatedTags_( Target, Tags ) ) ) {
		GetPluginFeatures_( Target, Tags, Filename, Configuration, Locale );

		sclm::MGetValue( rgstry::tentry___( sclr::parameter::TargetedPlugin, Target ), Arguments );
	} else if ( qRPT )
		qRFwk();
qRR
qRT
qRE
	return Exists;
}

namespace {
	void Dump_(
		const rgstry::entry__ &Entry,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_OFLOW___ SFlow;
		txf::text_oflow__ TFlow;
		xml::rWriter Writer;
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

void sclm::HandleLocale_(
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

	scll::Insert( scll::tMain, Location, XML, rgstry::rthIgnore );
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

		sclm::MGetValue( rgstry::sTEntry( sclr::definition::plugin::Filename, Tags ), Filename );

		GetPluginFeature_( rgstry::sTEntry( sclr::definition::plugin::Configuration, Tags ), Configuration );
		GetPluginFeature_( rgstry::sTEntry( sclr::definition::plugin::Locale, Tags ), Locale );

		sclm::MGetValue( rgstry::rTEntry( sclr::parameter::PluginItem, Target, Id ), Arguments );
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

void sclm::Plug_(
	const char *Target,
	const char *Label,
	const char *Identifier,
	plgn::dRetrievers &Retrievers )
{
qRH
	str::wStrings Ids;
qRB
	Ids.Init();

	sclm::GetValues( rgstry::rTEntry( sclr::parameter::loose_plugin_item::Id, Target ), Ids );

	::Plug_( Target, Label, Identifier, Ids, Retrievers );
qRR
qRT
qRE
}

fdr::rWDriver &sclm::rWDriverRack::Init( const fnm::name___ &FileName )
{
	Filename_.Init( FileName );

	if ( Filename_.IsEmpty() ) {
		BackedUp_ = false;
		return cio::GetOutDriver();
	} else {
		sclm::CreateBackupFile( Filename_ );
		BackedUp_ = true;

		if ( Driver_.Init( Filename_, qRPU ) != tol::rSuccess )
			sclm::ReportFileOpeningErrorAndAbort( Filename_ );

		return Driver_;
	}
}

void sclm::rWDriverRack::HandleError( void )
{
	if ( BackedUp_ )
		sclm::RecoverBackupFile( Filename_ );
}


fdr::rRDriver &sclm::rRDriverRack::Init( const fnm::name___ &FileName )
{
	Filename_.Init( FileName );

	if ( Filename_.IsEmpty() ) {
		return cio::GetInDriver();
	} else {
		if ( Driver_.Init( Filename_, qRPU ) != tol::rSuccess )
			sclm::ReportFileOpeningErrorAndAbort( Filename_ );

		return Driver_;
	}
}

namespace {
	inline void signal_( int s )
	{
		exit( EXIT_SUCCESS );
	}
}

void sclm::ExitOnSignal( void )
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

void (* mtk::MTKErrorHandling)(void) = sclm::ErrorDefaultHandling;

Q37_GCTOR( sclm )
{
	BinPath_.Init();
	FillProjectAutomat_();
}
