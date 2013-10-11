/*
	'aem' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'aem' header file ('aem.h').
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: aem.cpp,v 1.34 2013/04/26 14:43:40 csimon Exp $

#define AEM__COMPILATION

#include "aem.h"

class aemtutor
: public ttr_tutor
{
public:
	aemtutor( void )
	: ttr_tutor( AEM_NAME )
	{
#ifdef AEM_DBG
		Version = AEM_VERSION "\b\bD $";
#else
		Version = AEM_VERSION;
#endif
		Owner = AEM_OWNER;
		Date = "$Date: 2013/04/26 14:43:40 $";
	}
	virtual ~aemtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace aem {
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class aempersonnalization
: public aemtutor
{
public:
	aempersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~aempersonnalization( void )
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

static aempersonnalization Tutor;

ttr_tutor &AEMTutor = Tutor;
