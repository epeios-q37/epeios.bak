/*
	'skt.h' by Claude SIMON (http://zeusw.org/).

	'skt' is part of the Epeios framework.

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

#ifndef SKT__INC
#define SKT__INC

#define SKT_NAME		"SKT"

#include "ttr.h"

extern class ttr_tutor &SKTTutor;

#if defined( E_DEBUG ) && !defined( SKT_NODBG )
#define SKT_DBG
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// SKeTch

#include "err.h"
#include "flw.h"

namespace skt {

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
