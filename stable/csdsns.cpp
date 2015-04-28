/*
	'csdsns.cpp' by Claude SIMON (http://zeusw.org/).

	'csdsns' is part of the Epeios framework.

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

#define CSDSNS__COMPILATION

#include "csdsns.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace csdsns;

#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *csdsns::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( New );
		CASE( Store );
		CASE( TestAndGet );
		CASE( Delete );
	default:
		ERRPrm();
		return NULL;	// Pour viter un 'warning'.
		break;
	}
}

void csdsns::_callback___::_Clean( void )
{
	sdr::row__ Row = _Core.UPs.First();

	while ( Row != E_NIL ) {
		_Callback->PostProcess( _Core.UPs( Row ) );

		Row = _Core.UPs.Next( Row );
	}

	_Core.reset();
}

