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

# define PLGNCORE_SHARED_DATA_VERSION	"2"

# define PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME	PluginIdentification
# define PLGNCORE_RETRIEVE_PLUGIN_FUNCTION_NAME			RetrievePlugin
# define PLGNCORE_RELEASE_PLUGIN_FUNCTION_NAME			ReleasePlugin

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
		void *UP;				// A la discrétion de l'utilisateur.
		void reset( bso::bool__ = true )
		{
			Version = NULL;
			ERRError = NULL;
			SCLError = NULL;
			Directory = NULL;
			UP = NULL;
		}
		E_CDTOR( data__ );
		data__(
			err::err___ *ERRError,
			sclerror::error___ *SCLError,
			const char *Directory,
			void *UP = NULL )
		{
			Init( ERRError, SCLError, Directory, UP );
		}
		void Init(
			err::err___ *ERRError,
			sclerror::error___ *SCLError,
			const char *Directory,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->ERRError = ERRError;
			this->SCLError = SCLError;
			this->Directory = Directory;
			this->UP = UP;
		}
		static bso::size__ Control( void )
		{
			return sizeof( data__ );
		}

	};
#pragma pack( pop )

	typedef const char *(plugin_identification)( void );
	typedef void *(retrieve_plugin)( const data__ *Data );
	typedef void (release_plugin)( void *);
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
