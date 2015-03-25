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

# define FRDKRN_SESSION_LABEL "Session"

namespace frdkrn {
	using namespace frdfrd;

	typedef rgstry::multi_level_registry_ registry_;

	template <typename t> class il_	// id, template.
	{
	public:
		struct s {
			t Id;
			str::string_::s Label;
		} &S_;
		str::string_ Label;
		il_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Id = t(-1);
			Label.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Label.plug( AS );
		}
		il_ &operator =(const il_ &IL)
		{
			S_.Id = IL.S_.Id;
			Label = IL.Label;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label )
		{
			S_.Id = Id;
			this->Label.Init( Label );
		}
		E_RODISCLOSE_( t, Id );
	};

	template <typename t> class ilw_	// id, template, wording.
	: public il_<t>
	{
	public:
		struct s
		: public il_<t>::s
		{
			str::string_::s Wording;
		};
		str::string_ Wording;
		ilw_( s &S )
		: il_<t>( S ),
		  Wording( S.Wording )
		{}
		void reset( bso::bool__ P = true )
		{
			il_<t>::reset( P );
			Wording.reset( P );
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			il_<t>::plug( AS );
			Wording.plug( AS );
		}
		ilw_ &operator =(const ilw_ &ILW)
		{
			il_<t>::operator = ( ILW );
			Wording = ILW.Wording;

			return *this;
		}
		void Init(
			t Id,
			const str::string_ &Label,
			const str::string_ &Wording )
		{
			il_<t>::Init( Id, Label );
			this->Wording.Init( Wording );
		}
	};

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

	struct backend_features___
	{
	public:
		const char *Language;
		str::string Location;
		csducl::type__ Type;
		bso::uint__ PingDelay;
		void *UP;
		void reset( bso::bool__ P = true )
		{
			Language = NULL;
			Location.reset( P );
			Type = csducl::t_Undefined;
			PingDelay = 0;
			UP = NULL;
		}
		E_CDTOR( backend_features___ )
		void Init(
			const char *Language,
			bso::uint__ PingDelay,
			void *UP )
		{
			this->Language = Language;
			Location.Init();
			Type = csducl::t_Undefined;
			this->PingDelay = PingDelay;
			this->UP = UP;
		}
	};

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

	bso::bool__ GetDefaultConfigurationFileName(
		const char *Affix,
		str::string_ &FileName );

	typedef fblfrd::reporting_callback__ reporting_callback__;


# if  0
	class reporting_callback__
	: public _reporting_callback__
	{
	private:
		const lcl::locale_ *_Locale;
		const char *_Language;
	protected:
		void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message );
		virtual void FRDKRNReport( const str::string_ &Message ) = 0;
	public:
		void reset ( bso::bool__ P = true )
		{
			_reporting_callback__::reset( P );
			_Locale = NULL;
			_Language = NULL;
		}
		E_VDTOR( reporting_callback__ )
		void Init(
			const lcl::locale_ &Locale,
			const char *Language )
		{
			_reporting_callback__::Init();
			_Locale = &Locale;
			_Language = Language;
		}
		void Report( const str::string_ &Message )
		{
			FRDKRNReport( Message );
		}
	};
# endif

	class kernel___
	{
	private:
		csducl::universal_client_core _ClientCore;
		frdfrd::frontend___ _Frontend;
		lcl::meaning _ErrorMeaning;
		reporting_callback__ *_ReportingCallback;
# if 0
		registry_ &_R( void )
		{
			if ( _Registry == NULL )
				ERRFwk();

			return *_Registry;
		}
# endif
		bso::bool__ _LoadProjectLocale( void );
		bso::bool__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			const char *Language,
			bso::uint__ PingDelay,
			error_set___ &ErrorSet,
			void *UP,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRFwk();

			FRDKRNDisconnection();

			_Frontend.reset();
			_ClientCore.reset();
		}
	protected:
		bso::bool__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			const char *Language,
			bso::uint__ PingDelay,
			error_set___ &ErrorSet,
			void *UP,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		virtual void FRDKRNConnection( fblfrd::frontend___ &Frontend ) = 0;	// Appelé lors aprés connection au 'backend'.
		virtual void FRDKRNDisconnection( void ) = 0;	// Appelé avant déconnexion du 'backend'.
	public:
		void reset( bso::bool__ P = true )
		{
			_Frontend.reset( P );
			_ClientCore.reset( P );
			_ErrorMeaning.reset( P );
			_ReportingCallback = NULL;
		}
		E_CVDTOR( kernel___ );
		bso::bool__ Init( reporting_callback__ &ReportingCallback )	//  PAS dupliqué !
		{
			_ErrorMeaning.Init();
			_ReportingCallback = &ReportingCallback;
			// L'initialisation de '_Frontend' et '_ClientCore' se fait à la connection.

			return true;
		}
		fblfrd::frontend___ &Frontend( void )
		{
			if ( !_Frontend.IsConnected() )
				ERRFwk();

			return _Frontend;
		}
		void DismissRequest( void )	// A appeler uniquement lorsque l'un des paramètres de sortie est un 'flow', dés que tout son contenu ('EndOfFlow()' retourne 'true') est lu.
		{
			Frontend().Dismiss();
		}
		bso::bool__ Launch(
			const backend_features___ &BackendFeatures,
			const compatibility_informations__ &CompatibilityInformations,
			error_set___ &ErrorSet )	// Les paramètres de connection sont récupèrés de la 'registry'.
		{
			if ( BackendFeatures.Type != csducl::t_Undefined )
				return _Connect( BackendFeatures.Location, CompatibilityInformations, BackendFeatures.Type, BackendFeatures.Language, BackendFeatures.PingDelay, ErrorSet, BackendFeatures.UP );

			return true;
		}
		void Close( void )
		{
			_CloseConnection();
		}
# if 0	// Old
		status__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id );
# endif
		bso::bool__ Launch(
			const backend_features___ &BackendFeatures,
			const compatibility_informations__ &CompatibilityInformations );
		E_RWDISCLOSE__( lcl::meaning_ , ErrorMeaning );
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
		bso::bool__ IsConnected( void ) const
		{
			return _Frontend.IsConnected();
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

	const char *GetLabel( authentication_prompt_mode__ Mode );

	authentication_prompt_mode__ GetAuthenticationPromptMode( const str::string_ &Pattern );

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

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
