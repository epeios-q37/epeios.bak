/*
	'plgncore.h' by Claude SIMON (http://zeusw.org/).

	'plgncore' is part of the Epeios framework.

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

# define PLGNCORE_SHARED_DATA_VERSION	"3"

# define PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME	PluginIdentification
# define PLGNCORE_RETRIEVE_CALLBACK_FUNCTION_NAME		RetrieveCallback

namespace plgncore {
#pragma pack( push, 1)
	// NOTA : Si modifié, modifier 'CSDLEO_SHARED_DATA_VERSION' !
	class data__
	{
	public:
		const char *Version;	// Toujours en première position.
		bso::size__ ControlValue;
		err::err___ *ERRError;
		sclerror::error___ *SCLError;
		const char *Directory;
		rgstry::entry__ Configuration;
		rgstry::entry__ Locale;
		void *UP;				// A la discrétion de l'utilisateur.
		void reset( bso::bool__ P = true )
		{
			Version = NULL;
			ERRError = NULL;
			SCLError = NULL;
			Directory = NULL;
			UP = NULL;
			Configuration.reset( P );
			Locale.reset( P );
		}
		E_CDTOR( data__ );
		data__(
			err::err___ *ERRError,
			sclerror::error___ *SCLError,
			const char *Directory,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			void *UP = NULL )
		{
			Init( ERRError, SCLError, Directory, Configuration, Locale, UP );
		}
		void Init(
			err::err___ *ERRError,
			sclerror::error___ *SCLError,
			const char *Directory,
			const rgstry::entry__ &Configuration,
			const rgstry::entry__ &Locale,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->ERRError = ERRError;
			this->SCLError = SCLError;
			this->Directory = Directory;
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
			... /* Autres paramètres. Le dernier doit être = 'NULL' */ )
		{
			va_list Parameters;
			va_start( Parameters, FirstParameter );

			PLGNCOREInitialize( Data, FirstParameter, Parameters );	// 'FirstParameter' est inclus dans le '...' de la méthode appelée.
																// Il n'existe en tant que paramètre de cette méthode que pour en faciliter la compréhension.
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
