/*
	'cvm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cvm' header file ('cvm.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: cvm.cpp,v 1.23 2013/03/14 11:09:19 csimon Exp $

#define CVM__COMPILATION

#include "cvm.h"

class cvmtutor
: public ttr_tutor
{
public:
	cvmtutor( void )
	: ttr_tutor( CVM_NAME )
	{
#ifdef CVM_DBG
		Version = CVM_VERSION "\b\bD $";
#else
		Version = CVM_VERSION;
#endif
		Owner = CVM_OWNER;
		Date = "$Date: 2013/03/14 11:09:19 $";
	}
	virtual ~cvmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class cvmpersonnalization
: public cvmtutor
{
public:
	cvmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cvmpersonnalization( void )
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

static cvmpersonnalization Tutor;

ttr_tutor &CVMTutor = Tutor;
