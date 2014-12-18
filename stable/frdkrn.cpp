/*
	'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'frdkrn' header file ('frdkrn.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/345
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: frdkrn.cpp,v 1.53 2013/04/22 15:28:01 csimon Exp $

#define FRDKRN__COMPILATION

#include "frdkrn.h"

class frdkrntutor
: public ttr_tutor
{
public:
	frdkrntutor( void )
	: ttr_tutor( FRDKRN_NAME )
	{
#ifdef FRDKRN_DBG
		Version = FRDKRN_VERSION "\b\bD $";
#else
		Version = FRDKRN_VERSION;
#endif
		Owner = FRDKRN_OWNER;
		Date = "$Date: 2013/04/22 15:28:01 $";
	}
	virtual ~frdkrntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

# include "frdrgy.h"

using namespace frdkrn;

using rgstry::tentry__;

#define CASE( m )\
	case r##m:\
	return FRDKRN_NAME "_" #m;\
	break

const char *frdkrn::GetLabel( recap__ Recap )
{
#if FRDKRN__R_AMOUNT != 9
#	error "'report__' modified !"
#endif

	switch( Recap ) {
		CASE( ProjectParsingError );
		CASE( SetupParsingError );
		CASE( NoOrBadProjectId );
		CASE( NoOrBadBackendDefinition );
		CASE( NoBackendLocation );
		CASE( UnableToConnect );
		CASE( IncompatibleBackend );
		CASE( BackendError );
		CASE( UnableToOpenFile );
		default:
			ERRPrm();
			break;
	}

	return NULL;	// Pour �viter un 'warning'.
}

static void GetAuthorText_(
	const char *Name,
	const char *Contact,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( FRDKRN_NAME "_AuthorText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( Contact );

	Locale.GetTranslation( Meaning, Language, Text );
ERRErr
ERREnd
ERREpilog
}

static void GetAffiliationText_(
	const char *Name,
	const char *URL,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &Text )
{
ERRProlog
	lcl::meaning Meaning;
ERRBegin
	Meaning.Init();
	Meaning.SetValue( FRDKRN_NAME "_AffiliatedSoftwareText" );
	Meaning.AddTag( Name );
	Meaning.AddTag( URL );

	Locale.GetTranslation( Meaning, Language, Text );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &frdkrn::GetAboutText(
	const char *BuildInformations,
	const char *AppName,
	const char *Version,
	const char *AuthorName,
	const char *AuthorContact,
	const char *Copyright,
	const char *SoftwareDetails,
	const char *SoftwareURL,
	kernel___ &Kernel,
	str::string_ &Text )
{
ERRProlog
	str::string AuthorText, ProjectAffiliationText, SoftwareAffiliationText, ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
ERRBegin
	Text.Append( AppName );
	Text.Append( ' ' );
	Text.Append( Version );
	Text.Append( " Build " __DATE__ " " __TIME__ );
	Text.Append( " (" );
	Text.Append( BuildInformations );
	Text.Append( ")\n\t" );

	AuthorText.Init();
	GetAuthorText_( AuthorName, AuthorContact, Kernel.Locale(), Kernel.Language(), AuthorText );
	Text.Append( AuthorText );

	Text.Append( "\nCopyright " );
	Text.Append( Copyright );

	Text.Append( "\n\n\t\t" );
	SoftwareAffiliationText.Init();
	GetAffiliationText_( SoftwareDetails, SoftwareURL, Kernel.Locale(), Kernel.Language(), SoftwareAffiliationText );
	Text.Append( SoftwareAffiliationText );
	Text.Append( '.' );

	Text.Append( "\n\n(" );
	Text.Append( Kernel.LauncherIdentification() );
	Text.Append( ")" );


	if ( Kernel.IsConnected() ) {
		Text.Append( '\n' );
		Text.Append( '\n' );

		ProtocolVersion.Init();
		BackendLabel.Init();
		APIVersion.Init();
		BackendInformations.Init();
		BackendCopyright.Init();
		SoftwareInformations.Init();

		Kernel.AboutBackend( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

		Text.Append( "Backend : " );
		Text.Append( BackendInformations );
		Text.Append( " (" );
		Text.Append( SoftwareInformations );
		Text.Append( ')' );
	}
ERRErr
ERREnd
ERREpilog
	return Text;
}

#if FRDKRN__R_AMOUNT != 9
# error "'report__' modified !"
#endif

void frdkrn::GetMeaning(
	recap__ Recap,
	const error_set___ &ErrorSet,
	lcl::meaning_ &Meaning )
{
ERRProlog
	lcl::meaning MeaningBuffer;
ERRBegin
	Meaning.SetValue( GetLabel( Recap ) );

	switch ( Recap ) {
		case rProjectParsingError:
		case rSetupParsingError:
			MeaningBuffer.Init();
			rgstry::GetMeaning( ErrorSet.Context, MeaningBuffer );
			Meaning.AddTag( MeaningBuffer );
			break;
		case rNoOrBadProjectId:
			Meaning.AddTag( ErrorSet.Misc );
			break;
		case rNoOrBadBackendDefinition:
			break;
		case rNoBackendLocation:
			break;
		case rUnableToConnect:
		case rIncompatibleBackend:
		case rBackendError:
			Meaning.AddTag( ErrorSet.BackendLocation );
		case rUnableToOpenFile:
			Meaning.AddTag( ErrorSet.Misc );	// NOTA : Dans certains cas, n'est pas utilis� ('ErrorSet.Misc' vide ET tag correspondant non utilis� pour l'entr�e correspondante dans le fichier des locales), mais cela ne d�range pas de le d�finir quand m�me.
			break;
		default:
			ERRPrm();
			break;
		}

ERRErr
ERREnd
ERREpilog
}

#if 0
#define PROJECT_TYPE_NEW		"New"
#define PROJECT_TYPE_PREDEFINED	"Predefined"
#define PROJECT_TYPE_USER		"User"

project_type__ frdkrn::GetProjectType(
	const str::string_ &Label,
	err::handling__ ErrHandling )
{
	if ( Label == PROJECT_TYPE_NEW )
		return ptNew;
	else if ( Label == PROJECT_TYPE_PREDEFINED )
		return ptPredefined;
	else if ( Label == PROJECT_TYPE_USER )
		return ptUser;
	else if ( ErrHandling = err::hThrowException )
		ERRFwk();
	else if ( ErrHandling != err::hUserDefined )
		ERRPrm();

	return pt_Undefined;
}

const str::string_ &frdkrn::GetPredefinedProjectLocation(
	const str::string_ &Id,
	const registry_ &Registry,
	str::string_ &Location )
{
ERRProlog
	rgstry::tags Tags;
ERRBegin
	Tags.Init();
	Tags.Append( Id );

	Registry.GetValue( tentry__( frdrgy::PredefinedProject, Tags ), Location );
ERRErr
ERREnd
ERREpilog
	return Location;
}


#define BACKEND_EXTENDED_TYPE_NONE		"None"
#define BACKEND_EXTENDED_TYPE_EMBEDDED		"Embedded"
#define BACKEND_EXTENDED_TYPE_DAEMON		"Daemon"
#define BACKEND_EXTENDED_TYPE_PREDEFINED	"Predefined"

backend_extended_type__ frdkrn::GetBackendExtendedType( const str::string_ &RawType )
{
	if ( RawType == BACKEND_EXTENDED_TYPE_EMBEDDED )
		return bxtEmbedded;
	else if ( RawType == BACKEND_EXTENDED_TYPE_DAEMON )
		return bxtDaemon;
	else if ( RawType == BACKEND_EXTENDED_TYPE_PREDEFINED )
		return bxtPredefined;
	else if ( RawType == BACKEND_EXTENDED_TYPE_NONE )
		return bxtNone;
	else
		return bxt_Undefined;
}

static const char *GetBackendExtendedTypeLabel_( backend_extended_type__ Type )
{
	switch ( Type ) {
	case bxtNone:
		return BACKEND_EXTENDED_TYPE_NONE;
		break;
	case bxtPredefined:
		return BACKEND_EXTENDED_TYPE_PREDEFINED;
		break;
	case bxtDaemon:
		return BACKEND_EXTENDED_TYPE_DAEMON;
		break;
	case bxtEmbedded:
		return BACKEND_EXTENDED_TYPE_EMBEDDED;
		break;
	default:
		ERRPrm();
		break;
	}

	return NULL;	// Pour �viter un 'warning'.
}


backend_extended_type__ frdkrn::GetBackendExtendedType(	const registry_ &Registry )
{
	backend_extended_type__ Type = bxt_Undefined;
ERRProlog
	str::string RawType;
ERRBegin
	RawType.Init();

	Registry.GetValue( frdrgy::BackendType, RawType );

	Type = GetBackendExtendedType( RawType );
ERRErr
ERREnd
ERREpilog
	return Type;
}

void frdkrn::SetBackendExtendedType(
	registry_ &Registry,
	backend_extended_type__ Type )
{
	Registry.SetValue( frdrgy::BackendType, str::string( GetBackendExtendedTypeLabel_( Type ) ) );
}


bso::bool__ frdkrn::GetDefaultConfigurationFileName(
	const char *Affix,
	str::string_ &FileName )
{
	bso::bool__ Exists = false;
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	FileName.Init( Affix );
	FileName.Append( '.' );
	FileName.Append( FRDKRN_CONFIGURATION_FILE_EXTENSION );

	Exists = fil::Exists( FileName.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

static str::string_ &AppendTargetAttributePathItem_(
	const char *TargetName,
	str::string_ &Target )
{
	Target.Append( "[target=\"" );
	Target.Append( TargetName );
	Target.Append( "\"]" );

	return Target;
}
#endif
void frdkrn::reporting_callback__::FBLFRDReport(
	fblovl::reply__ Reply,
	const char *Message )
{
ERRProlog
	lcl::meaning Meaning;
	str::string Translation;
ERRBegin
	if ( _Kernel == NULL )
		ERRFwk();

	Meaning.Init();
	Meaning.SetValue( fblovl::GetLabel( Reply ) );
	Meaning.AddTag( Message );

	Translation.Init();
	_Kernel->Locale().GetTranslation( Meaning, _Kernel->Language(), Translation );

	this->FRDKRNReport( Translation );
ERRErr
ERREnd
ERREpilog
}


#if 0
static inline bso::uint__ GetBackendPingDelay_( const registry_ &Registry )
{
	return rgstry::GetUInt( Registry, frdrgy::BackendPingDelay, 0 );
}
#endif

recap__ frdkrn::kernel___::_Connect(
	const char *RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	const char *Language, 
	bso::uint__ PingDelay,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	flx::E_STRING_OFLOW_DRIVER___ OFlowDriver;
	csdlec::library_data__ LibraryData;
	csdleo::mode__ Mode = csdleo::m_Undefined;
	str::string Buffer;
ERRBegin
	OFlowDriver.Init( ErrorSet.Misc, fdr::ts_Default );
	LibraryData.Init( csdleo::mEmbedded, flx::VoidOFlowDriver, flx::VoidOFlowDriver, false, (void *)RemoteHostServiceOrLocalLibraryPath );
	// Attention ; les erreurs arrivant dans la biblioth�que lors de son chargement ne sont plus rapport�es. A modifier.
	// Auparavent, c'�tait 'OFlowDriver' qui �tait uilis� pour le param�tre 'CErr', mais cela ne focntionnait pas lorsque la bitlioth�ques �tait charg�e plusieurs fois...

	Buffer.Init();

	if ( !_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, LibraryData, LogFunctions, Type, PingDelay ) ) {
		OFlowDriver.reset();	// Pour vider les caches.
		if ( ErrorSet.Misc.Amount() != 0 )
			Recap = rBackendError;
		else
			Recap = rUnableToConnect;
		ERRReturn;
	}

	if ( !_Frontend.Init( Language, CompatibilityInformations, _ClientCore, *_ReportingCallback, ErrorSet.IncompatibilityInformations ) ) {
		Recap = rIncompatibleBackend;
		ERRReturn;
	}

	FRDKRNConnection( _Frontend );

	Recap = r_OK;
ERRErr
ERREnd
	if ( Recap != r_OK )
		ErrorSet.BackendLocation.Init( RemoteHostServiceOrLocalLibraryPath );
ERREpilog
	return Recap;
}

recap__ frdkrn::kernel___::_Connect(
	const str::string_ &RemoteHostServiceOrLocalLibraryPath,
	const compatibility_informations__ &CompatibilityInformations,
	csducl::type__ Type,
	const char *Language,
	bso::uint__ PingDelay,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	TOL_CBUFFER___ RemoteHostServiceOrLocalLibraryPathBuffer;
ERRBegin
	Recap = _Connect( RemoteHostServiceOrLocalLibraryPath.Convert( RemoteHostServiceOrLocalLibraryPathBuffer ), CompatibilityInformations, Type, Language, PingDelay, ErrorSet, LogFunctions );
ERRErr
ERREnd
ERREpilog
	return Recap;
}

#if 0

static csducl::type__ GetPredefinedBackendTypeAndLocation_(
	const registry_ &Registry,
	str::string_ &Location )
{
	csducl::type__ Type = csducl::t_Undefined;
ERRProlog
	str::string Id;
	str::string RawType;
	rgstry::tags Tags;
ERRBegin
	Id.Init();

	if ( !Registry.GetValue( frdrgy::Backend, Id ) )
		ERRReturn;

	Tags.Init();
	Tags.Append( Id );

	RawType.Init();
	if ( !Registry.GetValue( tentry__( frdrgy::PredefinedBackendType , Tags), RawType ) )
		ERRReturn;

	if ( !Registry.GetValue( tentry__( frdrgy::PredefinedBackend, Tags ), Location ) )
		ERRReturn;

	switch ( GetBackendExtendedType( RawType ) ) {
	case bxtEmbedded:
		Type = csducl::tLibrary;
		break;
	case bxtDaemon:
		Type = csducl::tDaemon;
		break;
	default:
		ERRReturn;
		break;
	}

ERRErr
ERREnd
ERREpilog
	return Type;
}

csducl::type__ frdkrn::GetBackendTypeAndLocation(
	const registry_ &Registry,
	str::string_ &Location )
{
	csducl::type__ Type = csducl::t_Undefined;

	switch ( GetBackendExtendedType( Registry ) ) {
	case bxtEmbedded:
		Type = csducl::tLibrary;
		Registry.GetValue( frdrgy::Backend, Location );
		break;
	case bxtDaemon:
		Type = csducl::tDaemon;
		Registry.GetValue( frdrgy::Backend, Location );
		break;
	case bxtPredefined:
		Type = GetPredefinedBackendTypeAndLocation_( Registry, Location );
		break;
	case bxtNone:
	case bxt_Undefined:
		break;
	default:
		ERRFwk();
		break;
	}

	return Type;
}
#endif

#if 0
recap__ frdkrn::kernel___::_Connect(
	const compatibility_informations__ &CompatibilityInformations,
	const char *Language,
	error_set___ &ErrorSet,
	csdsnc::log_functions__ &LogFunctions )
{
	recap__ Recap = r_Undefined;
ERRProlog
	str::string Location;
	csducl::type__ Type = csducl::t_Undefined;
ERRBegin
	Location.Init();

	switch ( Type = GetBackendTypeAndLocation( _R(), Location ) ) {
	case csducl::tLibrary:
	case csducl::tDaemon:
		Recap = _Connect( Location, CompatibilityInformations, Type, Language, ErrorSet, LogFunctions );
		break;
	case csducl::t_Undefined:
		Recap = rNoOrBadBackendDefinition;
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Recap;
}
#endif

static bso::bool__ IsProjectIdValid_( const str::string_ &Id )
{
	sdr::row__ Row = Id.First();

	if ( Id.Amount() == 0 )
		return false;

	while ( Row != E_NIL ) {
		if ( !isalnum( Id( Row ) ) && ( Id( Row ) != '_' ) )
			return false;

		Row = Id.Next( Row );
	}

	return true;
}

#define PROJECT_ROOT_PATH	"Projects/Project[@target=\"%1\"]"

#define PROJECT_ID_RELATIVE_PATH "@id"

#if 0	// Old
recap__ frdkrn::kernel___::_FillProjectRegistry(
	const fnm::name___ &FileName,
	const char *Target,
	const xpp::criterions___ &Criterions,
	str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	str::string Path;
	TOL_CBUFFER___ FileNameBuffer, PathBuffer;
ERRBegin
	Path.Init( PROJECT_ROOT_PATH );
	str::ReplaceShortTag( Path, 1, str::string( Target ), '%' );

	if ( _Registry.Fill( _RegistryProjectLevel = _Registry.PushEmbeddedLevel( str::string( "Project" ) ), FileName, Criterions, Path.Convert( PathBuffer ),  ErrorSet.Context ) != rgstry::sOK ) {
		Recap = rProjectParsingError;
		ERRReturn;
	}

	if ( !_Registry.GetValue( str::string( PROJECT_ID_RELATIVE_PATH ), Id ) || !IsProjectIdValid_( Id ) ) {
		Recap = rNoOrBadProjectId;
		Path.Append( '/' );
		Path.Append( PROJECT_ID_RELATIVE_PATH );
		ErrorSet.Misc.Init( Path );
		ERRReturn;
	}

	Recap = r_OK;
ERRErr
ERREnd
ERREpilog
	return Recap;
}
# endif

recap__ frdkrn::kernel___::_DumpProjectRegistry(
	const fnm::name___ &FileName,
	const char *TargetName,
	const str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;
ERRProlog
	flf::file_oflow___ FFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	bso::bool__ Backuped = false;
	txf::text_oflow__ TVoidOFlow;
ERRBegin
	TVoidOFlow.Init( flx::VoidOFlow );

	if ( fil::CreateBackupFile( FileName, fil::bmRename, err::hUserDefined ) == fil::bsOK )
		Backuped = true;

	if ( FFlow.Init( FileName, err::hUserDefined ) != tol::rSuccess ) {
		Recap = rUnableToOpenFile;
		FileName.UTF8( ErrorSet.Misc );
		ERRReturn;
	}

	TFlow.Init( FFlow );

	Writer.Init( TFlow, xml::oCompact, xml::e_Default );

	Writer.PushTag( "Projects" );

	Writer.PushTag( "Project" );

	Writer.PutAttribute( "target", TargetName );
	Writer.PutAttribute( "id", Id );

	_R().Dump( _R().TopLevel(), E_NIL, false, Writer );
	
	Recap = r_OK;
ERRErr
	if ( Backuped )
		fil::RecoverBackupFile( FileName );
ERREnd
ERREpilog
	return Recap;
}


#if 0	// Old.
static bso::bool__ LoadProjectLocale_(
	const str::string_ &LocaleContent,
	lcl::locale_ &Locale,
	lcl::context___ &Context )
{
	bso::bool__ Success = false;
ERRProlog
	flx::E_STRING_IFLOW__ Flow;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	Flow.Init( LocaleContent );
	XFlow.Init( Flow, utf::f_Default );

	if ( Locale.Push( XFlow, xpp::criterions___(), "Locale", Context ) != LCL_UNDEFINED_LEVEL )
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

status__ frdkrn::kernel___::_LoadProjectLocale( void )
{
	status__ Status = s_Undefined;
ERRProlog
	str::string Locale;
	rgstry::context___ Context;
ERRBegin
	Locale.Init();

	_R().GetValue( frdrgy::Locale, Locale );

	if ( Locale.Amount() != 0 ) {
		Context.Init();
		if ( !LoadProjectLocale_( Locale, _Locale, Context ) ) {
			_Meaning.Init();
			rgstry::GetMeaning( Context, _Meaning );
			Status = sWarning;
		} else
			Status = sOK;
	} else
		Status = sOK;

ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ frdkrn::kernel___::LoadProject(
	const str::string_ &FileName,
	const char *TargetName,
	const xpp::criterions___ &Criterions,
	str::string_ &Id )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Report = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Report = LoadProject( FileName, TargetName, Criterions, Id, ErrorSet ) ) != r_OK ) {
		_Meaning.Init();
		frdkrn::GetMeaning( Report, ErrorSet, _Meaning );
		Status = sError;
		ERRReturn;
	} else
		Status = _LoadProjectLocale();
ERRErr
ERREnd
ERREpilog
	return Status;
}
#endif


status__ frdkrn::kernel___::Launch(
	const backend_features___ &BackendFeatures,
	const compatibility_informations__ &CompatibilityInformations )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = Launch( BackendFeatures, CompatibilityInformations, ErrorSet ) ) != r_OK ) {
		_ErrorMeaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _ErrorMeaning );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

recap__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id,
	error_set___ &ErrorSet )
{
	recap__ Recap = r_Undefined;

	if ( ( Recap = _DumpProjectRegistry( FileName, TargetName, Id, ErrorSet ) ) == r_OK ) {
		_ProjectOriginalTimeStamp = time( NULL );
		_ProjectModificationTimeStamp = 0;
	}

	return Recap;
}

status__ frdkrn::kernel___::SaveProject(
	const str::string_ &FileName,
	const char *TargetName,
	const str::string_ &Id )
{
	status__ Status = s_Undefined;
ERRProlog
	error_set___ ErrorSet;
	recap__ Recap = r_Undefined;
ERRBegin
	ErrorSet.Init();

	if ( ( Recap = SaveProject( FileName, TargetName, Id, ErrorSet ) ) != r_OK ) {
		_ErrorMeaning.Init();
		frdkrn::GetMeaning( Recap, ErrorSet, _ErrorMeaning );
		Status = sError;
		ERRReturn;
	} else
		Status = sOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static stsfsm::automat AuthenticationAutomat_;

void InitAuthenticationAutomat_( void )
{
	AuthenticationAutomat_.Init();
	stsfsm::Fill( AuthenticationAutomat_, apm_amount, GetLabel );
}

#define APM( name )	case apm##name : return #name; break

const char *frdkrn::GetLabel( authentication_prompt_mode__ Mode )
{
	switch ( Mode ) {
		APM( None );
		APM( Auto );
		APM( Empty );
		APM( Partial );
		APM( Full );
	default:
		ERRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

authentication_prompt_mode__ frdkrn::GetAuthenticationPromptMode( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, AuthenticationAutomat_, apm_Undefined, apm_amount );
}

static void InitAutomats_( void )
{
	InitAuthenticationAutomat_();
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class frdkrnpersonnalization
: public frdkrntutor
{
public:
	frdkrnpersonnalization( void )
	{
		if ( FRDKRN__R_AMOUNT != r_amount )
			ERRChk();	// 

		InitAutomats_();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~frdkrnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static frdkrnpersonnalization Tutor;

ttr_tutor &FRDKRNTutor = Tutor;
