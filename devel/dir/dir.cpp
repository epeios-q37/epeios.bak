/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
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
# ifdef CPE_C_MINGW	// 'MinGW' ne connait pas 'ELOOP'.
	case WSAELOOP:
# else
	case ELOOP:
# endif
	case ENAMETOOLONG:
	case ENOENT:
		return sInadequatePath;
		break;
	case ENOTDIR:
		return sBadPath;
		break;
	case EFAULT:
		qRFwk();
		break;
	case ENOTEMPTY:
		return sNotEmpty;
		break;
	default:
		qRSys();
		break;
	}

	return s_Undefined;	// Pour viter un 'warning'.
}

