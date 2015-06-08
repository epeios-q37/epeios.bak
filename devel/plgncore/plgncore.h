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

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// PLuGiN CORE

# include "err.h"
# include "flw.h"
# include "tol.h"

# include "sclerror.h"

# include <stdarg.h>

# define PLGNCORE_SHARED_DATA_VERSION	"4"

# define PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME	PluginIdentification
# define PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME		RetrieveCallback

namespace plgncore {
#pragma pack( push, 1)
	// NOTA : Si modifi, modifier 'CSDLEO_SHARED_DATA_VERSION' !
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
		void *UP;				// A la discrtion de l'utilisateur.
		void reset( bso::bool__ P = true )
		{
			Version = NULL;
			qRRor = NULL;
			SCLError = NULL;
			UP = NULL;
			Configuration.reset( P );
			Locale.reset( P );
		}
		E_CDTOR( data__ );
		data__(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			void *UP = NULL )
		{
			Init( qRRor, SCLError, Configuration, Locale, UP );
		}
		void Init(
			err::err___ *qRRor,
			sclerror::error___ *SCLError,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->qRRor = qRRor;
			this->SCLError = SCLError;
			this->CIO = &cio::GetCurrentSet();
			this->Configuration.Init( Configuration );
			this->Locale.Init( Locale );
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
		virtual void PLGNCOREInitialize(
			const data__ *Data,
			... ) = 0;
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
		void Initialize(
			const data__ *Data,
			const ntvstr::char__ *FirstParameter = NULL,
			... /* Autres paramtres. Le dernier doit tre = 'NULL' */ )
		{
			va_list Parameters;
			va_start( Parameters, FirstParameter );

			PLGNCOREInitialize( Data, FirstParameter, Parameters );	// 'FirstParameter' est inclus dans le '...' de la mthode appele.
																// Il n'existe en tant que paramtre de cette mthode que pour en faciliter la comprhension.
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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
