/*
	'idxbtq' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'idxbtq' header file ('idxbtq.h').
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



//	$Id: idxbtq.cpp,v 1.48 2013/05/17 15:03:50 csimon Exp $

#define IDXBTQ__COMPILATION

#include "idxbtq.h"

class idxbtqtutor
: public ttr_tutor
{
public:
	idxbtqtutor( void )
	: ttr_tutor( IDXBTQ_NAME )
	{
#ifdef IDXBTQ_DBG
		Version = IDXBTQ_VERSION "\b\bD $";
#else
		Version = IDXBTQ_VERSION;
#endif
		Owner = IDXBTQ_OWNER;
		Date = "$Date: 2013/05/17 15:03:50 $";
	}
	virtual ~idxbtqtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class idxbtqpersonnalization
: public idxbtqtutor
{
public:
	idxbtqpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~idxbtqpersonnalization( void )
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

static idxbtqpersonnalization Tutor;

ttr_tutor &IDXBTQTutor = Tutor;
