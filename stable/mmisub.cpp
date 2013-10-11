/*
	'mmisub' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmisub' header file ('mmisub.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: mmisub.cpp,v 1.4 2012/11/14 16:06:31 csimon Exp $

#define MMISUB__COMPILATION

#include "mmisub.h"

class mmisubtutor
: public ttr_tutor
{
public:
	mmisubtutor( void )
	: ttr_tutor( MMISUB_NAME )
	{
#ifdef MMISUB_DBG
		Version = MMISUB_VERSION "\b\bD $";
#else
		Version = MMISUB_VERSION;
#endif
		Owner = MMISUB_OWNER;
		Date = "$Date: 2012/11/14 16:06:31 $";
	}
	virtual ~mmisubtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mmi;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mmisubpersonnalization
: public mmisubtutor
{
public:
	mmisubpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmisubpersonnalization( void )
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

static mmisubpersonnalization Tutor;

ttr_tutor &MMISUBTutor = Tutor;
