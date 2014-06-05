/*
	Header for the 'frdrgy' library by Claude SIMON (csimon at zeusw dot org)
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

//	$Id: frdrgy.h,v 1.38 2013/04/26 14:43:39 csimon Exp $

#ifndef FRDRGY__INC
#define FRDRGY__INC

#define FRDRGY_NAME		"FRDRGY"

#define	FRDRGY_VERSION	"$Revision: 1.38 $"

#define FRDRGY_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDRGYTutor;

#if defined( E_DEBUG ) && !defined( FRDRGY_NODBG )
#define FRDRGY_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.38 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/26 14:43:39 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD ReGistrY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "rgstry.h"

# define FRDRGY_PATH_SEPARATOR	"/"	// Path item separator

# define FRDRGY_PATH_TAG_CHAR	'$'	// Joker for tags as character. NOTA : if modified, modify below too !
# define FRRRGY_PATH_TAG_STRING	"$"	// Joker for tags as string

# define FRDRGY_PARAMETERS_PATH	"Parameters" FRDRGY_PATH_SEPARATOR
# define FRDRGY_PROFILES_PATH	"Profiles" FRDRGY_PATH_SEPARATOR

namespace frdrgy {

	extern rgstry::entry___ Parameters;
	extern rgstry::entry___ Layouts;
	extern rgstry::entry___ Definitions;

	extern rgstry::entry___ DefaultProject;
	extern rgstry::entry___ Locale;

	extern rgstry::entry___ PredefinedProjects;
	extern rgstry::entry___ DefaultPredefinedProject;
	extern rgstry::entry___ PredefinedProjectId;
	extern rgstry::entry___ PredefinedProject;
	extern rgstry::entry___ PredefinedProjectAlias;

	extern rgstry::entry___ Backend;
	extern rgstry::entry___ BackendAccessMode;
	extern rgstry::entry___ BackendType;
	extern rgstry::entry___ BackendPingDelay;

	extern rgstry::entry___ Authentication;
	extern rgstry::entry___ AuthenticationCypherKey;
	extern rgstry::entry___ AuthenticationMode;
	extern rgstry::entry___ AuthenticationLogin;
	extern rgstry::entry___ AuthenticationPassword;

	extern rgstry::entry___ PredefinedBackends;
	extern rgstry::entry___ DefaultPredefinedBackend;
	extern rgstry::entry___ PredefinedBackendId;
	extern rgstry::entry___ PredefinedBackend;
	extern rgstry::entry___ PredefinedBackendAlias;
	extern rgstry::entry___ PredefinedBackendType;

	extern rgstry::entry___ Internals;
	extern rgstry::entry___ ProjectId;

	struct paths {
		struct profiles {
			static const char *FallbackProfile;
			static const char *DefaultProfile;
			static const char *UserProfile;
		};
	};

	typedef rgstry::multi_level_registry_ _registry_;

	class registry_
	: public _registry_
	{
	private:
		sdr::size__ _DumpRegistry(
			rgstry::level__ Level,
			rgstry::row__ Node,	// Si ==' 'E_NIL', on part de la racine.
			bso::bool__ NodeToo,
			xml::outfit__ Outfit,
			xml::encoding__ Encoding,
			txf::text_oflow__ &TFlow ) const
		{
			return _registry_::Dump( Level, Node, NodeToo, Outfit, Encoding, TFlow );
		}
		sdr::size__ _DumpRegistry(
			rgstry::level__ Level,
			rgstry::row__ Node,	// Si ==' 'E_NIL', on part de la racine.
			bso::bool__ NodeToo,
			xml::writer_ &Writer ) const
		{
			return _registry_::Dump( Level, Node, NodeToo, Writer );
		}
	public:
		struct s
		: public _registry_::s
		{
			rgstry::level__ Setup;
		} &S_;
		registry_( s &S )
		: S_( S ),
			_registry_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_registry_::reset( P );

			S_.Setup = RGSTRY_UNDEFINED_LEVEL;
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
			_registry_::plug( AS );
		}
		registry_ &operator =( const registry_ &R )
		{
			_registry_::operator =( R );

			S_.Setup = R.S_.Setup;

			return *this;
		}
		void Init( const rgstry::multi_level_registry_ &Registry )
		{
			_registry_::Init();

			_registry_::Push( Registry );
			S_.Setup= _registry_::PushEmbeddedLevel( rgstry::name( "Setup" ) );
		}
		void DumpSetup(
			rgstry::row__ Node,	// Si ==' 'E_NIL', on part de la racine.
			bso::bool__ NodeToo,
			xml::writer_ &Writer ) const
		{
			_DumpRegistry( S_.Setup, Node, NodeToo, Writer );
		}
		rgstry::status__ FillSetup(
			const char *FileName,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			rgstry::context___ &Context )
		{
			return _registry_::Fill( S_.Setup, FileName, Criterions, RootPath, Context );
		}
		rgstry::status__ FillSetup(
			xtf::extended_text_iflow__ &XFlow,
			const xpp::criterions___ &Criterions,
			const char *RootPath,
			rgstry::context___ &Context )
		{
			return _registry_::Fill( S_.Setup, XFlow, Criterions, RootPath, Context );
		}
		void CreateSetupPath( const str::string_ &Path )
		{
			_registry_::Create( S_.Setup, Path );
		}
		time_t SetupTimeStamp( void ) const
		{
			return TimeStamp( S_.Setup );
		}
		bso::bool__ GetSetupValue(
			const str::string_ &Path,
			str::string_ &Value ) const
		{
			return _registry_::GetValue( S_.Setup, Path, Value );
		}
	};


	E_AUTO( registry );

	// typedef rgstry::overloaded_unique_registry___ registry___;

	inline bso::bool__ GetValue(
		const char *Path,
		const _registry_ &Registry,
		str::string_ &Value )
	{
		return Registry.GetValue( str::string( Path ), Value );
	}

	bso::bool__ GetProfileValue(
		const char *Path,
		const _registry_ &Registry,
		str::string_ &Value );

	bso::bool__ GetProfileUIntValue(
		const char *Path,
		const _registry_ &Registry,
		bso::uint__ &Id );

	void SetProfileValue(
		const char *Path,
		_registry_ &Registry,
		const str::string_ &Value );

	inline void SetProfileUIntValue(
		const char *Path,
		_registry_ &Registry,
		bso::uint__ Id )
	{
		bso::integer_buffer__ Buffer;

		SetProfileValue( Path, Registry, str::string( bso::Convert( Id, Buffer ) ) );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
