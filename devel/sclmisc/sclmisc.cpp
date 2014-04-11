/*
	'sclmisc.cpp' by Claude SIMON (http://zeusw.org/).

	'sclmisc' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLMISC__COMPILATION

#include "sclmisc.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "str.h"
#include "cio.h"
#include "fnm.h"
#include "dir.h"
#include "lcl.h"

#include "scllocale.h"
#include "sclrgstry.h"
#include "sclerror.h"

using namespace sclmisc;

static TOL_CBUFFER___ Language_;

#define DEFAULT_LANGUAGE	"en"

#define CONFIGURATION_DEFAULT_FILENAME_SUFFIX ".xcfg"

#define LOCALE_DEFAULT_FILENAME_SUFFIX ".xlcl"

const char *sclmisc::GetLanguage( void )
{
	if ( Language_ == NULL )
		return DEFAULT_LANGUAGE;

	return Language_;
}

void sclmisc::ReportAndAbort( const lcl::meaning_ &Meaning )
{
	sclerror::SetMeaning( Meaning );
	ERRAbort();
}

void sclmisc::ReportAndAbort( const char *Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Meaning.SetValue( Text );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void sclmisc::ReportAndAbort(
	const char *Text,
	const str::string_ &Tag )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();

	Meaning.SetValue( Text );
	Meaning.AddTag( Tag );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

void sclmisc::ReportParsingErrorAndAbort(
	const char *ErrorLabel,
	const rgstry::context___ &Context )
{
ERRProlog
	lcl::meaning Meaning;
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( ErrorLabel );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );

	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

static void GetConfigurationLocaleParsingErrorMeaning_(
	const rgstry::context___ &Context,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.SetValue( SCLMISC_NAME "_ConfigurationLocaleParsingError" );

	MeaningBuffer.Init();
	rgstry::GetMeaning( Context, MeaningBuffer );

	Meaning.AddTag( MeaningBuffer );
ERRErr
ERREnd
ERREpilog
}

static void LoadLocale_(
	scllocale::target__ Target,
	const str::string_ &Locale,
	utf::format__ Format )
{
ERRProlog
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Flow.Init( Locale );
	XFlow.Init( Flow, Format );
	
	scllocale::LoadLocale( Target, XFlow, NULL, "Locale" );
ERRErr
ERREnd
ERREpilog
}

static void LoadLocale_(
	rgstry::level__ Level,
	scllocale::target__ Target,
	utf::format__ Format )
{
ERRProlog
	str::string Locale;
ERRBegin
	Locale.Init();

	sclrgstry::GetRegistry().GetValue( Level, sclrgstry::Locale, Locale );

	if ( Locale.Amount() != 0 ) {
		Locale.Insert( "<Locale>" );
		Locale.Append( "</Locale>" );
		LoadLocale_( Target, Locale, Format );
	}
ERRErr
ERREnd
ERREpilog
}

static void Initialize_(
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleRootPath,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryRootPath,
	const char *RegistryDirectory )
{
ERRProlog
	str::string Language;
ERRBegin
	scllocale::LoadLocale( scllocale::tSoftware, LocaleFlow, LocaleDirectory, LocaleRootPath );

	sclrgstry::LoadConfiguration( RegistryFlow, RegistryDirectory, RegistryRootPath );

	Language.Init();
	if ( sclrgstry::GetValue( sclrgstry::Language, Language ) )
		Language.Convert( Language_ );

	LoadLocale_( sclrgstry::GetConfigurationLevel(), scllocale::tConfiguration, RegistryFlow.Format());
ERRErr
ERREnd
ERREpilog
}

static void BuildRootPath_(
	const char *Subject,
	const char *Target,
	str::string_ &Path )
{
	Path.Append( Subject );
	Path.Append( "s/" );
	Path.Append( Subject );
	Path.Append( "[target=\"" );
	Path.Append( Target );
	Path.Append( "\"]" );
}

void sclmisc::Initialize(
	xtf::extended_text_iflow__ &LocaleFlow,
	const char *LocaleDirectory,
	xtf::extended_text_iflow__ &RegistryFlow,
	const char *RegistryDirectory )
{
ERRProlog
	str::string LocaleRootPath, RegistryRootPath;
	TOL_CBUFFER___ LocaleBuffer, RegistryBuffer;
ERRBegin
	LocaleRootPath.Init();
	BuildRootPath_( "Locale", SCLMISCTargetName, LocaleRootPath );

	RegistryRootPath.Init();
	BuildRootPath_( "Configuration", SCLMISCTargetName, RegistryRootPath );

	Initialize_( LocaleFlow, LocaleRootPath.Convert( LocaleBuffer ), LocaleDirectory, RegistryFlow, RegistryRootPath.Convert( RegistryBuffer ), RegistryDirectory );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ GuessFileName_(
	const fnm::name___ &Affix,
	const fnm::name___ &Suffix,
	const fnm::name___ &SuggestedDirectory,
	fnm::name___ &FileName )
{
	bso::bool__ Success = false;
ERRProlog
	fnm::name___ Path;
ERRBegin
	FileName.Init();
	fnm::BuildFileName( "", Affix, Suffix, FileName );

	if ( !fil::Exists( FileName ) ) {
		FileName.Init();
		fnm::BuildFileName( SuggestedDirectory, Affix, Suffix, FileName );

		if ( !fil::Exists( FileName ) ) {
				FileName.Init();
				Path.Init();
				fnm::BuildFileName( dir::GetSelfPath( Path ), Affix, Suffix, FileName );

				Success = fil::Exists( FileName );
		} else
			Success = true;
	} else
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ InitializeFlow_(
	const char *Suffix,
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
	bso::bool__ Success = false;
ERRProlog
	fnm::name___ FileName;
	TOL_CBUFFER___ LocationBuffer;
	fnm::name___ Location;
ERRBegin
	FileName.Init();
	Success = GuessFileName_( SCLMISCTargetName, Suffix, SuggestedDirectory, FileName );

	if ( Success )
		if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess )
			Success = false;

	Location.Init();
	Directory.Append( fnm::GetLocation( FileName, Location ).UTF8( LocationBuffer ) );
ERRErr
ERREnd
ERREpilog
	return Success;
}

static flw::iflow__ &InitializeLocaleFlow_(
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	if ( !InitializeFlow_( LOCALE_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		Meaning.Init();
		Meaning.SetValue( "" );	// Ne sera pas traduit, puisque la locale n'a pas pu être lu.
		Meaning.AddTag( "Unable to open locale file" );	// Ceci remaplacera le '%0' ci-dessus.
		ReportAndAbort( Meaning );
	}
ERRErr
ERREnd
ERREpilog
	return Flow;
}

static flw::iflow__ &InitializeConfigurationFlow_(
	const fnm::name___ &SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	if ( !InitializeFlow_( CONFIGURATION_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		Meaning.Init();
		Meaning.SetValue( SCLMISC_NAME "_UnableToOpenConfigurationFile" );
		ReportAndAbort( Meaning );
	}
ERRErr
ERREnd
ERREpilog
	return Flow;
}

void sclmisc::Initialize( const fnm::name___ &SuggestedDirectory )
{
ERRProlog
	flf::file_iflow___ LocaleFlow, ConfigurationFlow;
	xtf::extended_text_iflow__ LocaleXFlow, ConfigurationXFlow;
	str::string LocaleDirectory, ConfigurationDirectory;
	TOL_CBUFFER___ LocaleBuffer, ConfigurationBuffer;
ERRBegin
	LocaleDirectory.Init();
	InitializeLocaleFlow_( SuggestedDirectory, LocaleFlow, LocaleDirectory );
	LocaleXFlow.Init( LocaleFlow, utf::f_Default );

	ConfigurationDirectory.Init();
	InitializeConfigurationFlow_( SuggestedDirectory, ConfigurationFlow, ConfigurationDirectory );
	ConfigurationXFlow.Init( ConfigurationFlow, utf::f_Default );

	Initialize( LocaleXFlow, LocaleDirectory.Convert( LocaleBuffer ), ConfigurationXFlow, ConfigurationDirectory.Convert( ConfigurationBuffer ) );
ERRErr
ERREnd
ERREpilog
}

void sclmisc::EraseProject( void )
{
	sclrgstry::EraseProject();
	scllocale::EraseLocale( scllocale::tProject );
}


void sclmisc::LoadProject(
	const fnm::name___ &FileName,
	str::string_ &Id )
{
	sclrgstry::LoadProject( FileName, SCLMISCTargetName, Id );

	LoadLocale_( sclrgstry::GetProjectLevel(), scllocale::tConfiguration, utf::f_Default );
}


void sclmisc::CreateBackupFile(
	const fnm::name___ &FileName,
	fil::backup_mode__ Mode )
{
ERRProlog
	fil::backup_status__ Status = fil::bs_Undefined;
	lcl::meaning Meaning;
ERRBegin
	if ( ( Status = fil::CreateBackupFile( FileName, Mode, err::hUserDefined ) )!= fil::bsOK ) {
		Meaning.Init();
		fil::GetMeaning( Status, FileName, Meaning );
		ReportAndAbort( Meaning );
	}
ERRErr
ERREnd
ERREpilog
}

void sclmisc::RecoverBackupFile( const fnm::name___ &FileName )
{
ERRProlog
	fil::recover_status__ Status = fil::rs_Undefined;
	lcl::meaning Meaning;
ERRBegin
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Meaning.Init();
		fil::GetMeaning( Status, FileName, Meaning );
		ReportAndAbort( Meaning );
	}
ERRErr
ERREnd
ERREpilog
}

void sclmisc::ReportFileOpeningErrorAndAbort( const fnm::name___ &FileName )
{
ERRProlog
	lcl::meaning Meaning;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( SCLMISC_NAME "_UnableToOpenFile" );
	Meaning.AddTag( FileName.UTF8( Buffer ) );
	ReportAndAbort( Meaning );
ERRErr
ERREnd
ERREpilog
}

txf::text_oflow__ &sclmisc::text_oflow_rack___::Init( const fnm::name___ &FileName )
{
	_FileName.Init( FileName );

	if ( _FileName.Size() == 0 ) {
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

void sclmisc::text_oflow_rack___::HandleError( void )
{
	if ( _BackedUp )
		sclmisc::RecoverBackupFile( _FileName );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclmiscpersonnalization
{
public:
	sclmiscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclmiscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclmiscpersonnalization Tutor;
