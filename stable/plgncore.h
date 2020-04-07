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

#ifndef PLGNCORE_INC_
# define PLGNCORE_INC_

# define PLGNCORE_NAME		"PLGNCORE"

# if defined( E_DEBUG ) && !defined( PLGNCORE_NODBG )
#  define PLGNCORE_DBG
# endif

// PLuGiN CORE

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "rgstry.h"

# include <stdarg.h>

# define PLGNCORE_SHARED_DATA_VERSION	"7"

# define PLGNCORE_PLUGIN_LABEL_FUNCTION_NAME			PluginLabel
# define PLGNCORE_PLUGIN_IDENTIFIER_FUNCTION_NAME		PluginIdentifier
# define PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME		RetrieveCallback

namespace sclm {
	struct sRack;
}

namespace plgncore {
#pragma pack( push, 1)
	// NOTA : If modified, increment 'PLGNCORE_SHARED_DATA_VERSION' !
	class sData
	{
	public:
		const char *Version;	// Toujours en premire position.
		bso::size__ ControlValue;
		sclm::sRack *SCLRack;
		const str::string_ *Arguments;
		void *UP;				// User pointer.
		void reset( bso::bool__ P = true )
		{
			Version = NULL;
			UP = NULL;
			SCLRack = NULL;
			Arguments = NULL;
		}
		E_CDTOR( sData );
		sData(
			sclm::sRack &Rack,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Init( Rack, Arguments, UP );
		}
		void Init(
			sclm::sRack &SCLRack,
			const str::string_ &Arguments,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->SCLRack = &SCLRack;
			this->Arguments = &Arguments;
			this->UP = UP;
		}
		static bso::size__ Control( void )
		{
			return sizeof( sData );
		}
	};

	// If an error occurs, contains details
	class sAbstract {
	protected:
		virtual const char *PLGNCOREIdentifier( void ) = 0;
	public:
		void reset( bso::sBool = true )
		{}
		qCVDTOR( sAbstract );
		const char *Identifier( void ) {
			return PLGNCOREIdentifier();
		}
		void Init( void )
		{}
	};

#pragma pack( pop )

	class callback__
	{
	protected:
		virtual void PLGNCOREInitialize(
			const sData *Data,
			const rgstry::entry__ &Configuration ) = 0;
		virtual void PLGNCOREInitialize(
			const sData *Data,
			const fnm::name___ &Directory ) = 0;
		virtual void *PLGNCORERetrievePlugin( sAbstract *Abstract ) = 0;	// When an error occurs, 'NULL' is returned, and if 'Abstract' == NULL the error is handled internally.
		virtual bso::sBool PLGNCOREReleasePlugin( void *Plugin ) = 0;
		virtual const char *PLGNCOREPluginIdentifier( void ) = 0;
		virtual const char *PLGNCOREPluginDetails( void ) = 0;
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
		void Initialize(
			const sData *Data,
			const rgstry::entry__ &Configuration )
		{
			return PLGNCOREInitialize( Data, Configuration );
		}
		void Initialize(
			const sData *Data,
			const fnm::name___ &Directory )
		{
			return PLGNCOREInitialize( Data, Directory );
		}
		void *RetrievePlugin( sAbstract *Abstract )
		{
			return PLGNCORERetrievePlugin( Abstract );
		}
		bso::sBool ReleasePlugin( void *Plugin )
		{
			return PLGNCOREReleasePlugin( Plugin );
		}
		const char *PluginIdentifier( void )
		{
			return PLGNCOREPluginIdentifier();
		}
		const char *PluginDetails( void )
		{
			return PLGNCOREPluginDetails();
		}
	};

	typedef const char *(plugin_label)( void );
	typedef const char *(plugin_identifier)( void );
	typedef callback__ &(retrieve_callback)( void );
}

#endif
