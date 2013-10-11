/*
	'wllio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'wllio' header file ('wllio.h').
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



//	$Id: wllio.cpp,v 1.22 2013/04/05 16:33:45 csimon Exp $

#define WLLIO__COMPILATION

#include "wllio.h"

class wlliotutor
: public ttr_tutor
{
public:
	wlliotutor( void )
	: ttr_tutor( WLLIO_NAME )
	{
#ifdef WLLIO_DBG
		Version = WLLIO_VERSION "\b\bD $";
#else
		Version = WLLIO_VERSION;
#endif
		Owner = WLLIO_OWNER;
		Date = "$Date: 2013/04/05 16:33:45 $";
	}
	virtual ~wlliotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace wllio;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class wlliopersonnalization
: public wlliotutor
{
public:
	wlliopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~wlliopersonnalization( void )
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

static wlliopersonnalization Tutor;

ttr_tutor &WLLIOTutor = Tutor;
