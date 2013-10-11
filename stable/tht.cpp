/*
	'tht' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'tht' header file ('tht.h').
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



//	$Id: tht.cpp,v 1.10 2013/04/06 14:50:47 csimon Exp $

#define THT__COMPILATION

#include "tht.h"

class thttutor
: public ttr_tutor
{
public:
	thttutor( void )
	: ttr_tutor( THT_NAME )
	{
#ifdef THT_DBG
		Version = THT_VERSION "\b\bD $";
#else
		Version = THT_VERSION;
#endif
		Owner = THT_OWNER;
		Date = "$Date: 2013/04/06 14:50:47 $";
	}
	virtual ~thttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace tht;

#ifdef THT__WIN
#	include <stdlib.h>
#	include "windows.h"
#elif defined( THT__POSIX )
#	include <unistd.h>
#	include <sched.h>
#	include <errno.h>
#	include <time.h>
#else
#	error "Unknown compilation enviroment"
#endif



void tht::Suspend( unsigned long Delay )
{
#ifdef THT__WIN
	Sleep( Delay );
#elif defined( THT__POSIX )
	struct timespec T;

	T.tv_sec = Delay / 1000;
	T.tv_nsec = ( Delay % 1000 ) * 1000000;

	while ( nanosleep( &T, &T ) )
		if ( errno != EINTR )
			ERRLbr();
#else
#	error
#endif
}


void tht::Defer( void )
{
#ifdef THT__WIN
	Sleep( 0 );	// PAs 0, sinon l'usage CPU monte à 100%
#elif defined( THT__POSIX )
	if( sched_yield() != 0 )
		ERRLbr();
#else
	#error
#endif
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class thtpersonnalization
: public thttutor
{
public:
	thtpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~thtpersonnalization( void )
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

static thtpersonnalization Tutor;

ttr_tutor &THTTutor = Tutor;
