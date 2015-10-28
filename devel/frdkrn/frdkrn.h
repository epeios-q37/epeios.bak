/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

//	$Id: frdkrn.h,v 1.51 2013/06/18 09:36:03 csimon Exp $

#ifndef FRDKRN__INC
#define FRDKRN__INC

#define FRDKRN_NAME		"FRDKRN"

#define	FRDKRN_VERSION	"$Revision: 1.51 $"

#define FRDKRN_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( FRDKRN_NODBG )
#define FRDKRN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.51 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/06/18 09:36:03 $

/* End of automatic documentation generation part. */

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

# if 0

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
		void plug( qAS_ &AS )
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
		void plug( qAS_ &AS )
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

# endif

# if 0

	typedef csducl::features___ _features___;

	struct features___
	: public _features___
	{
	public:
		const char *Language;
		void *UP;
		void reset( bso::bool__ P = true )
		{
			_features___::reset( P );
			Language = NULL;
			UP = NULL;
		}
		E_CDTOR( features___ )
		void Init( void )
		{
			_features___::Init();
			Language = NULL;
			UP = NULL;
		}
	};

# endif

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

# if 0

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
				qRFwk();

			return *_Registry;
		}
# endif
		bso::bool__ _LoadProjectLocale( void );
		bso::bool__ _Connect(
			const features___ &Features,
			const compatibility_informations__ &CompatibilityInformations,
			error_set___ &ErrorSet,
			csdsnc::log_callback__ *LogCallback = NULL );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				qRFwk();

			FRDKRNDisconnection();

			_Frontend.reset();
			_ClientCore.reset();
		}
		virtual void FRDKRNConnection( fblfrd::frontend___ &Frontend ) = 0;	// Appel lors aprs connection au 'backend'.
		virtual void FRDKRNDisconnection( void ) = 0;	// Appel avant dconnexion du 'backend'.
	public:
		void reset( bso::bool__ P = true )
		{
			_Frontend.reset( P );
			_ClientCore.reset( P );
			_ErrorMeaning.reset( P );
			_ReportingCallback = NULL;
		}
		E_CVDTOR( kernel___ );
		bso::bool__ Init( reporting_callback__ &ReportingCallback )	//  PAS dupliqu !
		{
			_ErrorMeaning.Init();
			_ReportingCallback = &ReportingCallback;
			// L'initialisation de '_Frontend' et '_ClientCore' se fait  la connection.

			return true;
		}
		fblfrd::frontend___ &Frontend( void )
		{
			if ( !_Frontend.IsConnected() )
				qRFwk();

			return _Frontend;
		}
		void DismissRequest( void )	// A appeler uniquement lorsque l'un des paramtres de sortie est un 'flow', ds que tout son contenu ('EndOfFlow()' retourne 'true') est lu.
		{
			Frontend().Dismiss();
		}
		bso::bool__ Launch(
			const features___ &Features,
			const compatibility_informations__ &CompatibilityInformations,
			error_set___ &ErrorSet )	// Les paramtres de connection sont rcuprs de la 'registry'.
		{
			if ( Features.Type != csducl::t_Undefined )
				return _Connect( Features, CompatibilityInformations, ErrorSet );

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
			const features___ &BackendFeatures,
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
				qRFwk();

			_Frontend.ThrowqRFwk();
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Frontend.IsConnected();
		}
	};

# endif

	enum authentication_prompt_mode__ 
	{
		apmNone,	// Pas de prompt, 'login' et 'password' envoys sont vides (droits minimaux).
		apmAuto,	// Pas de prompt, 'login' et 'password' envoys sont ceux stocks dans le fichier de configuration.
		apmEmpty,	// Prompt affich, avec champs vides.
		apmPartial,	// Prompt affich, avec champ 'login' prrenpli.
		apmFull,	// Prompt affich, avec champs 'login' et 'password' prremplis.
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
#endif
