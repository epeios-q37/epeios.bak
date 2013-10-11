/*
	'fblfph' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblfph' header file ('fblfph.h').
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



//	$Id: xxx.cpp,v 1.9 2012/11/14 16:06:23 csimon Exp $

#define FBLFPH__COMPILATION

#include "fblfph.h"

class fblfphtutor
: public ttr_tutor
{
public:
	fblfphtutor( void )
	: ttr_tutor( FBLFPH_NAME )
	{
#ifdef FBLFPH_DBG
		Version = FBLFPH_VERSION "\b\bD $";
#else
		Version = FBLFPH_VERSION;
#endif
		Owner = FBLFPH_OWNER;
		Date = "$Date: 2012/11/14 16:06:23 $";
	}
	virtual ~fblfphtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblfph;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblfphpersonnalization
: public fblfphtutor
{
public:
	fblfphpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblfphpersonnalization( void )
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

static fblfphpersonnalization Tutor;

ttr_tutor &FBLFPHTutor = Tutor;
