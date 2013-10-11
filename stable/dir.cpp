/*
	'dir' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dir' header file ('dir.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: dir.cpp,v 1.15 2013/04/06 14:50:44 csimon Exp $

#define DIR__COMPILATION

#include "dir.h"

class dirtutor
: public ttr_tutor
{
public:
	dirtutor( void )
	: ttr_tutor( DIR_NAME )
	{
#ifdef DIR_DBG
		Version = DIR_VERSION "\b\bD $";
#else
		Version = DIR_VERSION;
#endif
		Owner = DIR_OWNER;
		Date = "$Date: 2013/04/06 14:50:44 $";
	}
	virtual ~dirtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dir;

state__ dir::HandleError( void )
{
	switch ( errno ) {
	case EEXIST:
		return sExists;
		break;
#ifdef DIR__POSIX
	case EPERM:
	case EACCES:
	case EROFS:
	case ELOOP:
	case ENAMETOOLONG:
#endif
		return sInadequatePath;
		break;
	case ENOENT:
#ifdef DIR__POSIX
	case ENOTDIR:
		return sBadPath;
		break;
	case EFAULT:
		ERRc();
		break;
#endif
	default:
		ERRSys();
		break;
	}

	return s_Undefined;	// Pour éviter un 'warning'.
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dirpersonnalization
: public dirtutor
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


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static dirpersonnalization Tutor;

ttr_tutor &DIRTutor = Tutor;
