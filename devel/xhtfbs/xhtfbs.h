/*
	'xhtfbs.h' by Claude SIMON (http://zeusw.org/).

	'xhtfbs' is part of the Epeios framework.

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

#ifndef XHTFBS__INC
# define XHTFBS__INC

# define XHTFBS_NAME		"XHTFBS"

# if defined( E_DEBUG ) && !defined( XHTFBS_NODBG )
#  define XHTFBS_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// XHT(ML) Frontend Base

# include "xhtagent.h"

# include "rgstry.h"

# include "lcl.h"
# include "err.h"
# include "flw.h"
# include "xml.h"

namespace xhtfbs {

	class action__
	{
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		void Init( void )
		{
			// Standadisation;
		}
	};

	void Start(
		const rgstry::multi_level_registry_ &Registry,
		const lcl::locale_ &Locale,
		const char *Language,
		xml::writer_ &Writer );

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
