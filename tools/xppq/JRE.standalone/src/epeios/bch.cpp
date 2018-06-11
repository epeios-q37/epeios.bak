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

#define BCH__COMPILATION

#include "bch.h"

#include "sdr.h"

using namespace bch;

void bch::_GetRelations(
	const uys::untyped_storage_ &Sorted,
	const uys::untyped_storage_ &Unsorted,
	sdr::size__ Size,
	sdr::row_t__ Limit,
	sdr::byte__ *Buffer,
	E_BUNCH_( sdr::row__ ) &Relations )
{
	sdr::row_t__ Row = 0;

	while ( Row < Limit ) {
		Sorted.Recall( Row, Size, Buffer );
		Relations.Append( Unsorted.Search( Buffer, Size, 0, Limit ) / Size );

		Row += Size;
	}
}
