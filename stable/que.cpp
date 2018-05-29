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
		qQSTACKd( sdr::row__ ) &Stack,
		sdr::row__ Begin,
		direction Direction )
{
	if ( Direction == que::dAscending )
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Next( Begin );
		}
		while ( Begin != qNIL );
	else
		do
		{
			Stack.Push( Begin );
			Begin = Queue.Previous( Begin );
		}
		while ( Begin != qNIL );
}

