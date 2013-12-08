/*
	'bch.cpp' by Claude SIMON (http://zeusw.org/).

	'bch' is part of the Epeios framework.

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

#define BCH__COMPILATION

#include "bch.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "sdr.h"

using namespace bch;

void bch::_GetRelations(
	const uys::untyped_storage_ &Sorted,
	const uys::untyped_storage_ &Unsorted,
	sdr::size__ Size,
	sdr::row_t__ Limit,
	sdr::datum__ *Buffer,
	E_BUNCH_( sdr::row__ ) &Relations )
{
	sdr::row_t__ Row = 0;

	while ( Row < Limit ) {
		Sorted.Recall( Row, Size, Buffer );
		Relations.Append( Unsorted.Search( Buffer, Size, 0, Limit ) / Size );

		Row += Size;
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bchpersonnalization
{
public:
	bchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static bchpersonnalization Tutor;
