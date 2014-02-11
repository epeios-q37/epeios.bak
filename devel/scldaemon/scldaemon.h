/*
	'scldaemon.h' by Claude SIMON (http://zeusw.org/).

	'scldaemon' is part of the Epeios framework.

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

#ifndef SCLDAEMON__INC
# define SCLDAEMON__INC

# define SCLDAEMON_NAME		"SCLDAEMON"

# if defined( E_DEBUG ) && !defined( SCLDAEMON_NODBG )
#  define SCLDAEMON_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SoCLe DAEMON

# include "err.h"
# include "flw.h"

# include "csdleo.h"

namespace scldaemon {
	const char *GetLanguage( void );

	// A définir par l'utilisateur.
	csdleo::callback__ *SCLDAEMONRetrieveSteering(
		csdleo::mode__ Mode,
		const lcl::locale_ &Locale );	// To overload !

	// A définir par l'utilisateur.
	void SCLDAEMONReleaseSteering( csdleo::callback__ *Steering );	// To overload.

	void DisplayModuleClosingMessage( void );

	void DisplayModuleClosedMessage( void );
}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
