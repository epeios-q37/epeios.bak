/*
	'utf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'utf' header file ('utf.h').
	Copyright (C) 20132004 Claude SIMON.

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



//	$Id: utf.cpp,v 1.1 2013/04/21 16:11:43 csimon Exp $

#define UTF__COMPILATION

#include "utf.h"

class utftutor
: public ttr_tutor
{
public:
	utftutor( void )
	: ttr_tutor( UTF_NAME )
	{
#ifdef UTF_DBG
		Version = UTF_VERSION "\b\bD $";
#else
		Version = UTF_VERSION;
#endif
		Owner = UTF_OWNER;
		Date = "$Date: 2013/04/21 16:11:43 $";
	}
	virtual ~utftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace utf;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class utfpersonnalization
: public utftutor
{
public:
	utfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~utfpersonnalization( void )
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

static utfpersonnalization Tutor;

ttr_tutor &UTFTutor = Tutor;
