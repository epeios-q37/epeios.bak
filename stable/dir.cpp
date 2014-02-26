/*
	'dir.cpp' by Claude SIMON (http://zeusw.org/).

	'dir' is part of the Epeios framework.

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

#define DIR__COMPILATION

#include "dir.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace dir;

state__ dir::HandleError( void )
{
	switch ( errno ) {
	case EEXIST:
		return sExists;
		break;
	case EPERM:
	case EACCES:
	case EROFS:
	case ELOOP:
	case ENAMETOOLONG:
	case ENOENT:
		return sInadequatePath;
		break;
	case ENOTDIR:
		return sBadPath;
		break;
	case EFAULT:
		ERRFwk();
		break;
	default:
		ERRSys();
		break;
	}

	return s_Undefined;	// Pour éviter un 'warning'.
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dirpersonnalization
{
public:
	dirpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dirpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static dirpersonnalization Tutor;
