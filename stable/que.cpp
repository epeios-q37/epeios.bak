/*
	'que' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'que' header file ('que.h').
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

#define QUE__COMPILATION

#include "que.h"

using namespace que;

void que::links_::Initialize(
	sdr::row_t__ Begin,
	sdr::row_t__ End )
{
	que::link__ L;

	do {
		Store( L, Begin );
	} while( Begin++ < End );
}


void que::Dump_(
		const E_QUEUE_ &Queue,
		E_QSTACK_( sdr::row__ ) &Stack,
		sdr::row__ Begin,
		direction Direction )
{
	if ( Direction == que::dAscending )
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Next( Begin );
		}
		while ( Begin != E_NIL );
	else
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Previous( Begin );
		}
		while ( Begin != E_NIL );
}

