/*
	'xhtcllbk.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtcllbk' is part of the Epeios framework.

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

#define XHTCLLBK__COMPILATION

#include "xhtcllbk.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtcllbk;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtcllbkpersonnalization
{
public:
	xhtcllbkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtcllbkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtcllbkpersonnalization Tutor;
