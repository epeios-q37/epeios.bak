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

# define PLGNCORE_RETRIEVE_PLUGIN_CALLBACK_FUNCTION_NAME	RetrievePluginCallback
# define PLGNCORE_RELEASE_PLUGIN_CALLBACK_FUNCTION_NAME		ReleasePluginCallback

namespace plgncore {
	typedef void *(retrieve_plugin_callback)( void );
	typedef void (release_plugin_callback)( void *);
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
