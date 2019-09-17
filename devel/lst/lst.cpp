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

#define LST_COMPILATION_

#include "lst.h"

#include "flf.h"
#include "str.h"

// Retourne l'lment succdant  'Element', ou LST_INEXISTANT si inexistant.
sdr::row_t__ lst::Successeur_(
	sdr::row_t__ Element,
	sdr::size__ Amount,
	const dStore &Libres )
{
	while( ( ++Element < Amount ) && Libres.IsAvailable( Element ) ) {};

	if ( Element >= Amount )
		return qNIL;
	else
		return Element;
}

// Retourne l'lment prcdent 'Element', ou LST_INEXISTANT si inexistant.
sdr::row_t__ lst::Predecesseur_(
	sdr::row_t__ Element,
	const dStore &Libres )
{
	bso::bool__ Trouve = false;

	while( ( Element > 0 ) && !( Trouve = !Libres.IsAvailable( --Element ) ) ) {};

	if ( Trouve )
		return Element;
	else
		return qNIL;
}

void lst::MarkAsReleased_(
	sdr::row_t__ First,
	sdr::row_t__ Last,
	dStore &Store )
{
	while ( First < Last )
		Store.RestorationRelease( Last-- );

	Store.RestorationRelease( First );
}
