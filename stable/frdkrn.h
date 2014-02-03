/*
	Header for the 'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
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
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: frdkrn.h,v 1.51 2013/06/18 09:36:03 csimon Exp $

#ifndef FRDKRN__INC
#define FRDKRN__INC

#define FRDKRN_NAME		"FRDKRN"

#define	FRDKRN_VERSION	"$Revision: 1.51 $"

#define FRDKRN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDKRNTutor;

#if defined( E_DEBUG ) && !defined( FRDKRN_NODBG )
#define FRDKRN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.51 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/18 09:36:03 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD KeRNel 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "frdfrd.h"
// #include "frdrgy.h"
#include "csdleo.h"

#define FRDKRN_CONFIGURATION_FILE_EXTENSION	"xcfg"

# define FRDKRN_SETUP_LABEL "Setup"

namespace frdkrn {
	using namespace frdfrd;

	typedef rgstry::multi_level_registry_ registry_;
	typedef rgstry::multi_level_registry registry;

	enum project_type__
	{
		ptNew,
		ptPredefined,
		ptUser,
		pt_amount,
		pt_Undefined
	};

	project_type__ GetProjectType(
		const str::string_ &Label,
		err::handling__ ErrHandling = err::h_Default );

	const str::string_ & GetPredefinedProjectLocation(
		const str::string_ &Id,
		const registry_ &Registry,
		str::string_ &Location );

	enum backend_extended_type__ {
		bxtNone,	// Non utilisation de 'backend'.
		bxtPredefined,
		bxtDaemon,
		bxtEmbedded,
		bxt_amount,
		bxt_Undefined
	};

	backend_extended_type__ GetBackendExtendedType( const str::string_ &RawType );

	backend_extended_type__ GetBackendExtendedType( const registry_ &Registry );

	void SetBackendExtendedType(
		registry_ &Registry,
		backend_extended_type__ Type );

	csducl::type__ GetBackendTypeAndLocation(
		const registry_ &Registry,
		str::string_ &Location );

	class log_functions__
	: public csdsnc::log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			csdsnc::log__ Log,
			const void *Flow,
			sdr::size__ Amount )
		{}
	};

	enum status__ {
		sOK,
		sError,		// Il y aune erreur, l'action en cours doit être interrompue (Lancer 'ERRAbort()').
		sWarning,		// Il y a eu un prblème, mais l'action peut être poursuivie.
		s_amount,
		s_Undefined
	};

	// Si modifié, modifier 'GetLabel_(...)' en conséquence ainsi que le '.xlcl' associé.
	enum recap__ {
		rProjectParsingError,		// Error during project file handling. See 'ErrorSet' for more details.
		rSetupParsingError,			// Error during setup data handling. See 'ErrorSet' for more details.
		rNoOrBadProjectId,
		rNoOrBadBackendDefinition,
		rNoBackendLocation,
		rUnableToConnect,
		rIncompatibleBackend,
		rBackendError,
		rUnableToOpenFile,
		r_amount,
		r_Undefined,
		r_OK,
	};

#define FRDKRN__R_AMOUNT	9	// Pour détecter les fonctions devant être modifiée si le nombre d'entrée de 'report__' est modifié.

	const char *GetLabel( recap__ Recap );

	struct error_set___
	{
	public:
		rgstry::context___ Context;
		incompatibility_informations IncompatibilityInformations;	// Quand survient un 'report__::rIncompatibleBackend'.
		str::string BackendLocation;
		str::string Misc;
		void reset( bso::bool__ P = true )
		{
			Context.reset( P );
			IncompatibilityInformations.reset( P );
			BackendLocation.reset( P );
			Misc.reset( P );
		}
		E_CDTOR( error_set___ );
		void Init( void )
		{
			Context.Init();
			IncompatibilityInformations.Init();
			BackendLocation.Init();
			Misc.Init();
		}
	};

	inline void GetMeaning(
		const error_set___ &ErrorSet,
		lcl::meaning_ &Meaning  )
	{
		return rgstry::GetMeaning( ErrorSet.Context, Meaning );
	}

	void GetMeaning(
		recap__ Recap,
		const error_set___ &ErrorSet,
		lcl::meaning_ &Meaning );


	bso::bool__ GetDefaultConfigurationFileName(
		const char *Affix,
		str::string_ &FileName );

	typedef fblfrd::reporting_functions__ _backend_error_reporting_functions__;

	class reporting_functions__
	: public _backend_error_reporting_functions__
	{
	private:
		const class kernel___ *_Kernel;
	protected:
		void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message );
		virtual void FRDKRNReport( const str::string_ &Message ) = 0;
		virtual void FRDKRNNotify( const str::string_ &Message ) = 0;
	public:
		void reset ( bso::bool__ P = true )
		{
			_backend_error_reporting_functions__::reset( P );
			_Kernel = NULL;
		}
		E_VDTOR( reporting_functions__ )
		void Init( const class kernel___ &Kernel )
		{
			_backend_error_reporting_functions__::Init();
			_Kernel = &Kernel;
		}
		void Report( const str::string_ &Message )
		{
			FRDKRNReport( Message );
		}
		void Notify( const str::string_ &Message )
		{
			FRDKRNNotify( Message );
		}
	};

	class kernel___
	{
	private:
		registry_ *_Registry;	// Configuration and project registry.
		csducl::universal_client_core _ClientCore;
//		rgstry::level__ _RegistryProjectLevel;	// Old.
		frdfrd::frontend___ _Frontend;
		lcl::locale _Locale;
		const char *_Language;	// Langue d'administration.
		lcl::meaning _Meaning;
		time_t _ProjectOriginalTimeStamp;	// Horodatage de la créationn du chargement du projet ou de sa dernière sauvegarde. Si == 0, pas de projet en cours d'utilisation.
		time_t _ProjectModificationTimeStamp;	// Horodatage de la dernière modification du projet.
		reporting_functions__ *_ReportingFunctions;
		registry_ &_R( void )
		{
			if ( _Registry == NULL )
				ERRFwk();

			return *_Registry;
		}
		const registry_ &_R( void ) const
		{
			if ( _Registry == NULL )
				ERRFwk();

			return *_Registry;
		}
		status__ _LoadProjectLocale( void );
		recap__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			reporting_functions__ &ReportingFunctions,
			const char *Language,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRFwk();

			FRDKRNDisconnection();

			_Frontend.reset();
			_ClientCore.reset();
		}
	protected:
# if 0	// Old.
		recap__ _FillProjectRegistry(
			const fnm::name___ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id,
			error_set___ &ErrorSet );
# endif
		recap__ _DumpProjectRegistry(
			const fnm::name___ &FileName,
			const char *TargetName,
			const str::string_ &Id,
			error_set___ &ErrorSet );
		recap__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			reporting_functions__ &ReportingFunctions,
			const char *Language,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		recap__ _Connect( // Try to connect using registry content.
			const compatibility_informations__ &CompatibilityInformations,
			reporting_functions__ &ReportingFunctions,
			const char *Language,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		virtual void FRDKRNConnection( fblfrd::frontend___ &Frontend ) = 0;	// Appelé lors aprés connection au 'backend'.
		virtual void FRDKRNDisconnection( void ) = 0;	// Appelé avant déconnexion du 'backend'.
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				CloseProject();

			_Registry = NULL;
			_Frontend.reset( P );
			_ClientCore.reset( P );
			_Locale.reset( P );
			_Language = NULL;
			_Meaning.reset( P );
			_ProjectOriginalTimeStamp = 0;
			_ProjectModificationTimeStamp = 0;
			_ReportingFunctions = NULL;
		}
		E_CVDTOR( kernel___ );
		status__ Init(
			registry_ &Registry,
			const lcl::locale_ &Locale,
			const char *Language,
			reporting_functions__ &ReportingFunctions )
		{
			_Registry = &Registry;

//			_RegistryProjectLevel = RGSTRY_UNDEFINED_LEVEL;

			_Meaning.Init();

			_Locale.Init();
			_Locale.Push( Locale );

			_Language = Language;

			_ProjectOriginalTimeStamp = 0;
			_ReportingFunctions = &ReportingFunctions;
			// L'initialisation de '_Frontend' et '_ClientCore' se fait à la connection.

			return sOK;
		}
		fblfrd::frontend___ &Frontend( void )
		{
			if ( !_Frontend.IsConnected() )
				ERRFwk();

			return _Frontend;
		}
		registry_ &Registry( void )
		{
			return _R();
		}
		const registry_ &Registry( void ) const
		{
			return _R();
		}
# if 0
		registry_ &Registry( void )
		{
			return _Registry;
		}
# endif
		const lcl::locale_ &Locale( void ) const
		{
			return _Locale;
		}
		const char *Language( void ) const
		{
			if ( _Language == NULL )
				ERRFwk();

			return _Language;
		}
		void Report( const lcl::meaning_ &Meaning ) const
		{
		ERRProlog
			str::string Translation;
		ERRBegin
			if( _ReportingFunctions == NULL )
				ERRFwk();

			Translation.Init();

			Locale().GetTranslation( Meaning, Language(), Translation );

			_ReportingFunctions->Report( Translation );
		ERRErr
		ERREnd
		ERREpilog
		}
		void Notify( const lcl::meaning_ &Meaning ) const
		{
		ERRProlog
			str::string Translation;
		ERRBegin
			if( _ReportingFunctions == NULL )
				ERRFwk();

			Translation.Init();

			Locale().GetTranslation( Meaning, Language(), Translation );

			_ReportingFunctions->Notify( Translation );
		ERRErr
		ERREnd
		ERREpilog
		}
#if 0
		recap__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id,
			error_set___ &ErrorSet )	// Ne réalise qu'une mise à jour de la registry avec le contenu du projet. Ne réalise pas de connection.
		{
			return _FillProjectRegistry( FileName, TargetName, Criterions, Id, ErrorSet );
		}
		void CreateNewProject( void )
		{
			_RegistryProjectLevel = _Registry.PushEmbeddedLevel( str::string( "Project" ) );
		}
#endif
		recap__ LaunchProject(
			const compatibility_informations__ &CompatibilityInformations,
			reporting_functions__ &ReportingFunctions,
			error_set___ &ErrorSet )	// Les paramètres de connection sont récupèrés de la 'registry'.
		{
			recap__ Recap = r_OK;
			
			if ( GetBackendExtendedType( Registry() ) != bxtNone )
				Recap = _Connect( CompatibilityInformations, ReportingFunctions, Language(), ErrorSet );

			if ( Recap == r_OK ) {
				_ProjectOriginalTimeStamp = time( NULL );
				_ProjectModificationTimeStamp = 0;
			}

			return Recap;
		}
# if 0	// Old
		status__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id );
# endif
		status__ LaunchProject(
			const compatibility_informations__ &CompatibilityInformations,
			reporting_functions__ &ReportingFunctions );
		time_t ProjectTimeStamp( void ) const
		{
			return _R().TimeStamp( _R().TopLevel() );
		}
		recap__ SaveProject(
			const str::string_ &FileName,
			const char *TargetName,
			const str::string_ &Id, 
			error_set___ &ErrorSet );
		status__ SaveProject(
			const str::string_ &FileName,
			const char *TargetName,
			const str::string_ &Id );
		void Touch( void )
		{
			_ProjectModificationTimeStamp = time( NULL );
		}
		time_t ProjectModificationTimeStamp( void ) const
		{
			return _ProjectModificationTimeStamp;
		}
		bso::bool__ IsProjectInProgress( void ) const
		{
			return _ProjectOriginalTimeStamp != 0;
		}
		bso::bool__ IsProjectModified( void ) const
		{
			return _ProjectOriginalTimeStamp < ProjectModificationTimeStamp();
		}
		E_RWDISCLOSE__( lcl::meaning_ , Meaning );
		const str::string_ &GetTranslatedMeaning( str::string_ &Translation ) const
		{
			return Locale().GetTranslation( Meaning(), Language(), Translation );
		}
		const str::string_ &GetTranslation(
			const char *Message,
			str::string_ &Translation ) const
		{
			return Locale().GetTranslation( Message, Language(), Translation );
		}
		void AboutBackend(
			str::string_ &ProtocolVersion,
			str::string_ &BackendLabel,
			str::string_ &APIVersion,
			str::string_ &BackendInformations,
			str::string_ &BackendCopyright,
			str::string_ &SoftwareInformations )
		{
			_Frontend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );
		}
		void ThrowERRFwk( void )
		{
			if ( !IsConnected() )
				ERRFwk();

			_Frontend.ThrowERRFwk();
		}
		status__ CloseProject( void )
		{
			if ( IsConnected() ) {
				_CloseConnection();
			}

			_ProjectOriginalTimeStamp =	_ProjectModificationTimeStamp = 0;

			return sOK;
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Frontend.IsConnected();
		}
		bso::bool__ GetRegistryValue(
			const char *Path,
			str::string_ &Value )
		{
			return _R().GetValue( str::string( Path ), Value );
		}
	};

	enum authentication_prompt_mode__ 
	{
		apmNone,	// Pas de prompt, 'login' et 'password' envoyés sont vides (droits minimaux).
		apmAuto,	// Pas de prompt, 'login' et 'password' envoyés sont ceux stockés dans le fichier de configuration.
		apmEmpty,	// Prompt affiché, avec champs vides.
		apmPartial,	// Prompt affiché, avec champ 'login' prérenpli.
		apmFull,	// Prompt affiché, avec champs 'login' et 'password' préremplis.
		apm_amount,
		apm_Undefined
	};

	authentication_prompt_mode__ GetAuthenticationPromptMode( const registry_ &Registry );

	inline fblfrd::id32__ _ExtractId32( const str::string_ &Value )
	{
		return Value.ToU32();
	}

	inline fblfrd::id16__ _ExtractId16( const str::string_ &Value )
	{
		return Value.ToU16();
	}

	inline fblfrd::id8__ _ExtractId8( const str::string_ &Value )
	{
		return Value.ToU8();
	}

	void GetPredefinedProjects(
		const registry_ &Registry,
		const lcl::locale_ &Locale,
		const char *Language,
		xml::writer_ &Writer );

	void GetPredefinedBackends(
		const registry_ &Registry,
		const lcl::locale_ &Locale,
		const char *Language,
		xml::writer_ &Writer );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
