/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define SPH__COMPILATION

#include "sph.h"

#ifdef SPH__CUSTOM
// Boucle jusqu' ce que le smaphore indique la disponibilit.
void sph__semaphore_::AttendreDisponibilite_( void )
{
	do
	{
		Sleep( 0 );
	}
	while ( !AllouerSemaphore_() );
}
#endif

