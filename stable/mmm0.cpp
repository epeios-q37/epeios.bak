/*
	'mmm0' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mmm0' header file ('mmm0.h').
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



//	$Id: mmm0.cpp,v 1.8 2012/11/14 16:06:31 csimon Exp $

#define MMM0__COMPILATION

#include "mmm0.h"

class mmm0tutor
: public ttr_tutor
{
public:
	mmm0tutor( void )
	: ttr_tutor( MMM0_NAME )
	{
#ifdef MMM0_DBG
		Version = MMM0_VERSION "\b\bD $";
#else
		Version = MMM0_VERSION;
#endif
		Owner = MMM0_OWNER;
		Date = "$Date: 2012/11/14 16:06:31 $";
	}
	virtual ~mmm0tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mmm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mmm0personnalization
: public mmm0tutor
{
public:
	mmm0personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmm0personnalization( void )
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

static mmm0personnalization Tutor;

ttr_tutor &MMM0Tutor = Tutor;
