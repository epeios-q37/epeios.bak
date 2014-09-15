/*
	'xhtfmn.h' by Claude SIMON (http://zeusw.org/).

	'xhtfmn' is part of the Epeios framework.

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

#ifndef XHTFMN__INC
# define XHTFMN__INC

# define XHTFMN_NAME		"XHTFMN"

# if defined( E_DEBUG ) && !defined( XHTFMN_NODBG )
#  define XHTFMN_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) Frontend MaiN

# include "xhtfbs.h"

# include "err.h"
# include "flw.h"
# include "xml.h"

namespace xhtfmn {

	typedef xhtfbs::event_handler__ _event_handler__;

	class event_handlers__
	: protected _event_handler__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
		}
		E_CVDTOR( event_handlers__ );
		void Init( void )
		{
			_event_handler__::Init();
		}
		void HandleProjectTypeSelection( xhtagent::agent___ &Agent );
		void HandleSubmission( xml::writer_ &Writer );
	};

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
