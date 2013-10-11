/*
	'stk' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'stk' header file ('stk.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: stk.cpp,v 1.7 2009/10/04 17:28:20 csimon Exp $

#define STK__COMPILATION

#include "stk.h"

class stktutor
: public ttr_tutor
{
public:
	stktutor( void )
	: ttr_tutor( STK_NAME )
	{
#ifdef STK_DBG
		Version = STK_VERSION "\b\bD $";
#else
		Version = STK_VERSION;
#endif
		Owner = STK_OWNER;
		Date = "$Date: 2009/10/04 17:28:20 $";
	}
	virtual ~stktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class stkpersonnalization
: public stktutor
{
public:
	stkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~stkpersonnalization( void )
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

static stkpersonnalization Tutor;

ttr_tutor &STKTutor = Tutor;
