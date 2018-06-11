/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define THT__COMPILATION

#include "tht.h"

#include "err.h"

using namespace tht;

#ifdef THT__WIN
# include <stdlib.h>
# include "windows.h"
#elif defined( THT__POSIX )
# include <unistd.h>
# include <sched.h>
# include <errno.h>
# include <time.h>
#else
# error "Unknown compilation enviroment"
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
			qRLbr();
#else
#	error
#endif
}


void tht::Defer( void )
{
#ifdef THT__WIN
	Sleep( 0 );	// Pas 0, sinon l'usage CPU monte  100%
#elif defined( THT__POSIX )
	if( sched_yield() != 0 )
		qRLbr();
#else
	#error
#endif
}

