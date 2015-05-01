/*
	'xdhcbk.cpp' by Claude SIMON (http://zeusw.org/).

	'xdhcbk' is part of the Epeios framework.

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

#define XDHCBK__COMPILATION

#include "xdhcbk.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xdhcbk;

#define C( name )\
	case f##name:\
	return #name;\
	break


const char *xdhcbk::GetLabel( function__ Function )
{
	switch ( Function ) {
	C( Alert );
	C( Confirm );
	C( SetChildren );
	C( SetCasting );
	C( SetProperty );
	C( GetProperty );
	C( SetAttribute );
	C( GetAttribute );
	C( RemoveAttribute );
	C( GetResult );
	C( SetContent );
	C( GetContent );
	C( Focus );
	default:
		ERRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}

void xdhcbk::Escape(
    const str::string_ &Source,
    str::string_ &Target,
	bso::char__ Delimiter,
	bso::char__ EscapeChar )
{
    sdr::row__ Row = Source.First();
	bso::char__ C = 0;

    while ( Row != E_NIL ) {
		switch ( C = Source( Row ) ) {
#if 0
		case 7:
			Target.Append( EscapeChar );
			Target.Append( 'a' );
			break;
#endif
		case 8:
			Target.Append( EscapeChar );
			Target.Append( 'b' );
			break;
		case 9:
			Target.Append( EscapeChar );
			Target.Append( 't' );
			break;
		case 10:
			Target.Append( EscapeChar );
			Target.Append( 'n' );
			break;
#if 0
		case 11:
			Target.Append( EscapeChar );
			Target.Append( 'v' );
			break;
#endif
		case 12:
			Target.Append( EscapeChar );
			Target.Append( 'f' );
			break;
		case 13:
			Target.Append( EscapeChar );
			Target.Append( 'r' );
			break;
#if 0
		case 127:
			Target.Append( EscapeChar );
			Target.Append( 'd' );
			break;
#endif
		case '\'':
		case '"':
			if ( ( Delimiter == C ) || ( Delimiter == 0 ) )
				Target.Append( EscapeChar );

			Target.Append( C );
			break;
		default:
			if ( C == EscapeChar )
				Target.Append( EscapeChar );

			Target.Append( Source( Row ) );
			break;
		}

        Row = Source.Next( Row );
    }
}


