/*
	'bso.cpp' by Claude SIMON (http://zeusw.org/).

	'bso' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define BSO__COMPILATION

#include "bso.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

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
			ERRDta();

		XInt._Int[Position] = ( UInt & 0x7f ) | 0x80; 
		UInt >>= 7;
	}

	XInt._Length = BSO_DINT_SIZE_MAX - Position;

	return XInt;
}

#define LIMIT ( BSO_INT_MAX >> 7 )

int__ bso::ConvertToInt(
	const raw__ *DInt,
	size__ &Length )
{
	length__ Position = 0;
	int__ Int = 0;

	do {
		if ( Int > LIMIT )
			ERRDta();

		Int = ( Int << 7 ) + ( DInt[Position] & 0x7f );
	} while ( DInt[Position++] & 0x80 );

	if ( &Length != NULL )
		Length = Position;

	return Int;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bsopersonnalization
{
public:
	bsopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
		if ( sizeof( size__ ) != sizeof( int__ ) )
			ERRChk();

		if ( sizeof( sint__ ) != sizeof( uint__ ) )
			ERRChk();

		if ( sizeof( int__ ) != sizeof( uint__ ) )
			ERRChk();
	}
	~bsopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static bsopersonnalization Tutor;
