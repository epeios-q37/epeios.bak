/*
	'clthvy' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'clthvy' header file ('clthvy.h').
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



//	$Id: clthvy.cpp,v 1.6 2012/11/14 16:06:25 csimon Exp $

#define CLTHVY__COMPILATION

#include "clthvy.h"

class clthvytutor
: public ttr_tutor
{
public:
	clthvytutor( void )
	: ttr_tutor( CLTHVY_NAME )
	{
#ifdef CLTHVY_DBG
		Version = CLTHVY_VERSION "\b\bD $";
#else
		Version = CLTHVY_VERSION;
#endif
		Owner = CLTHVY_OWNER;
		Date = "$Date: 2012/11/14 16:06:25 $";
	}
	virtual ~clthvytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace clthvy;


#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *clthvy::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( Creation );
		CASE( Retrieval );
		CASE( Release );
	default:
		ERRu();
		return NULL;	// Pour éviter un 'warning'.
		break;
	}
}



void clthvy::core_::_DeleteFlows( void )
{
	while ( Flows.Amount() != 0 )
		delete Flows.Pop();
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class clthvypersonnalization
: public clthvytutor
{
public:
	clthvypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~clthvypersonnalization( void )
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

static clthvypersonnalization Tutor;

ttr_tutor &CLTHVYTutor = Tutor;
