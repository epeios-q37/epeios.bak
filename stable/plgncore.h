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

#ifndef PLGNCORE__INC
# define PLGNCORE__INC

# define PLGNCORE_NAME		"PLGNCORE"

# if defined( E_DEBUG ) && !defined( PLGNCORE_NODBG )
#  define PLGNCORE_DBG
# endif

// PLuGiN CORE

# include "err.h"
# include "flw.h"
# include "tol.h"

# include "sclerror.h"

# include <stdarg.h>

# define PLGNCORE_SHARED_DATA_VERSION	"5"

# define PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME	PluginIdentification
# define PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME		RetrieveCallback

namespace plgncore {
#pragma pack( push, 1)
	// NOTA : If modified, increment 'PLGNCORE_SHARED_DATA_VERSION' !
	class data__
	{
	public:
		const char *Version;	// Toujours en premire position.
		bso::size__ ControlValue;
		err::err___ *qRRor;
		sclerror::error___ *SCLError;
		const cio::set__ *CIO;
		rgstry::entry__ Configuration;
		rgstry::entry__ Locale;
		// Either this below, or both above.
		const fnm::name___ *Location;
		const str::string_ *Arguments;
		void *UP;				// A la discrtion de l'utilisateur.
		void reset( bso::bool__ P = true )
		{
			Version = NULL;
			qRRor = NULL;
			SCLError = NULL;
			UP = NULL;
			Configuration.reset( P );
			Locale.reset( P );
			Location = NULL;
			Arguments = NULL;
		}
		E_CDTOR( data__ );
		data__(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Init( qRRor, SCLError, Configuration, Locale, Arguments, UP );
		}
		data__(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const fnm::name___ &Location,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Init( qRRor, SCLError, Location, Arguments, UP );
		}
		void Init(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->qRRor = qRRor;
			this->SCLError = SCLError;
			this->CIO = &cio::GetCurrentSet();
			this->Configuration.Init( Configuration );
			this->Locale.Init( Locale );
			Location = NULL;
			this->Arguments = &Arguments;
			this->UP = UP;
		}
		void Init(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const fnm::name___ &Location,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->qRRor = qRRor;
			this->SCLError = SCLError;
			this->CIO = &cio::GetCurrentSet();
			Configuration.Init();
			Locale.Init();
			this->Location = &Location;
			this->Arguments = &Arguments;
			this->UP = UP;
		}
		static bso::size__ Control( void )
		{
			return sizeof( data__ );
		}
	};
#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void PLGNCOREInitialize( const data__ *Data ) = 0;
		virtual void *PLGNCORERetrievePlugin( void ) = 0;
		virtual void PLGNCOREReleasePlugin( void *Plugin ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			//Standardisaction.
		}
		void Initialize( const data__ *Data )
		{
			PLGNCOREInitialize( Data );
		}
		void *RetrievePlugin( void )
		{
			return PLGNCORERetrievePlugin();
		}
		void ReleasePlugin( void *Plugin )
		{
			PLGNCOREReleasePlugin( Plugin );
		}
	};

	typedef const char *(plugin_identification)( void );
	typedef callback__ &(retrieve_callback)( void );
}

#endif
