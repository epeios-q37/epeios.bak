/*
	'bkdrqm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'bkdrqm' header file ('bkdrqm.h').
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



//	$Id: bkdrqm.cpp,v 1.4 2011/06/11 18:16:00 csimon Exp $

#define BKDRQM__COMPILATION

#include "bkdrqm.h"

class bkdrqmtutor
: public ttr_tutor
{
public:
	bkdrqmtutor( void )
	: ttr_tutor( BKDRQM_NAME )
	{
#ifdef BKDRQM_DBG
		Version = BKDRQM_VERSION "\b\bD $";
#else
		Version = BKDRQM_VERSION;
#endif
		Owner = BKDRQM_OWNER;
		Date = "$Date: 2011/06/11 18:16:00 $";
	}
	virtual ~bkdrqmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bkdrqm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bkdrqmpersonnalization
: public bkdrqmtutor
{
public:
	bkdrqmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bkdrqmpersonnalization( void )
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

static bkdrqmpersonnalization Tutor;

ttr_tutor &BKDRQMTutor = Tutor;
