/*
	'wbcllbck.h' by Claude SIMON (http://zeusw.org/).

	'wbcllbck' is part of the Epeios framework.

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

#ifndef WBCLLBCK__INC
# define WBCLLBCK__INC

# define WBCLLBCK_NAME		"WBCLLBCK"

# if defined( E_DEBUG ) && !defined( WBCLLBCK_NODBG )
#  define WBCLLBCK_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// WebKit CaLLBaCK

# include "err.h"
# include "flw.h"
# include "tol.h"

# define WBCLLBCK_RETRIEVE_CALLBACK_FUNCTION_NAME		WBCLLBCKRetrieveCallback
# define WBCLLBCK_RELEASE_CALLBACK_FUNCTION_NAME		WBCLLBCKReleaseCallback


namespace wbcllbck {

	class callback__ {
	protected:
	public:
		void reset( bso::bool__ = true )
		{
			// Standadisation.
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			// Standadisation.
		}
	};

	typedef callback__ *(retrieve_callback)( void );
	typedef void (release_callback)( callback__ * );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
