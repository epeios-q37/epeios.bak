/*
	'csdles.h' by Claude SIMON (http://zeusw.org/).

	'csdles' is part of the Epeios framework.

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

#ifndef CSDLES__INC
# define CSDLES__INC

# define CSDLES_NAME		"CSDLES"

# if defined( E_DEBUG ) && !defined( CSDLES_NODBG )
#  define CSDLES_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// Client-Server Devices Library Embedded Server

# include "err.h"
# include "flw.h"

# include "csdleo.h"

namespace csdles {
	csdleo::user_functions__ *CSDLESRetrieveSteering( csdleo::data__ *Data );	// A surcharger.
	void CSDLESReleaseSteering( csdleo::user_functions__ * );	// A surcharger.
}		

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
