/*
	'fblfub' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblfub' header file ('fblfub.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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



//	$Id: fblfub.cpp,v 1.1 2011/06/11 18:15:59 csimon Exp $

#define FBLFUB__COMPILATION

#include "fblfub.h"

class fblfubtutor
: public ttr_tutor
{
public:
	fblfubtutor( void )
	: ttr_tutor( FBLFUB_NAME )
	{
#ifdef FBLFUB_DBG
		Version = FBLFUB_VERSION "\b\bD $";
#else
		Version = FBLFUB_VERSION;
#endif
		Owner = FBLFUB_OWNER;
		Date = "$Date: 2011/06/11 18:15:59 $";
	}
	virtual ~fblfubtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblfub;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblfubpersonnalization
: public fblfubtutor
{
public:
	fblfubpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblfubpersonnalization( void )
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

static fblfubpersonnalization Tutor;

ttr_tutor &FBLFUBTutor = Tutor;
