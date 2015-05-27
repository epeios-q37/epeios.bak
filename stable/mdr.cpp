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

#define MDR__COMPILATION

#include "mdr.h"

using namespace mdr;

xsize__ mdr::Convert( size__ Size )
{
	xsize__ XSize;
	length__ Position = MDR__DSIZE_SIZE_MAX - 1;

	XSize._Size[Position] = Size & 0x7f;
	Size >>= 7;

	while ( Size != 0 ) {
		if ( Position-- == 0 )
			ERRc();

		XSize._Size[Position] = ( Size & 0x7f ) | 0x80; 
		Size >>= 7;
	}

	XSize._Length = MDR__DSIZE_SIZE_MAX - Position;

	return XSize;
}

#define LIMIT ( MDR_SIZE_MAX >> 7 )

size__ mdr::Convert( const mdr::datum__ *DSize )
{
	length__ Position = 0;
	size__ Size = 0;

	do {
		if ( Size > LIMIT )
			ERRc();

		Size = ( Size << 7 ) + ( DSize[Position] & 0x7f );
	} while ( DSize[Position++] & 0x80 );

	return Size;
}

