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

#define BSO__COMPILATION

#include "bso.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

# include "tol.h"

using namespace bso;

const xint__ &bso::_ConvertToDInt(
	int__ UInt,
	xint__ &XInt )
{
	length__ Position = BSO_DINT_SIZE_MAX - 1;

	XInt._Int[Position] = UInt & 0x7f;
	UInt >>= 7;

	while ( UInt != 0 ) {
		if ( Position-- == 0 )
			qRFwk();

		XInt._Int[Position] = ( UInt & 0x7f ) | 0x80; 
		UInt >>= 7;
	}

	XInt._Length = BSO_DINT_SIZE_MAX - Position;

	return XInt;
}

#define LIMIT ( BSO_INT_MAX >> 7 )

int__ bso::ConvertToInt(
	const byte__ *DInt,
	size__ *Length )
{
	length__ Position = 0;
	int__ Int = 0;

	do {
		if ( Int > LIMIT )
			qRFwk();

		Int = ( Int << 7 ) + ( DInt[Position] & 0x7f );
	} while ( DInt[Position++] & 0x80 );

	if ( Length != NULL )
		*Length = Position;

	return Int;
}

Q37_GCTOR( bso )
{
	if ( sizeof( size__ ) != sizeof( int__ ) )
		qRChk();

	if ( sizeof( sint__ ) != sizeof( uint__ ) )
		qRChk();

	if ( sizeof( int__ ) != sizeof( uint__ ) )
		qRChk();
}

