/*
	'xhtcllbk.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtcllbk' is part of the Epeios framework.

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

#define XHTCLLBK__COMPILATION

#include "xhtcllbk.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtcllbk;


void xhtcllbk::Escape(
    const str::string_ &Source,
    str::string_ &Target,
	char EscapeChar )
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
			Target.Append( EscapeChar );
			Target.Append( '\'' );
			break;
		case '"':
			Target.Append( EscapeChar );
			Target.Append( '"' );
			break;
		default:
			if ( C == EscapeChar ) {
				Target.Append( EscapeChar );
				Target.Append( EscapeChar );
			} else
				Target.Append( Source( Row ) );
			break;
		}

        Row = Source.Next( Row );
    }
}
#if 0
void xhtcllbk::Unescape(
    const str::string_ &Source,
    str::string_ &Target,
	char EscapeChar )
{
    sdr::row__ Row = Source.First();
	bso::char__ C = 0;
	bso::bool__ Skipped = false;

    while ( Row != E_NIL ) {
		if ( ( C = Source( Row ) ) == EscapeChar ) {
			if ( Skipped )
				Target.Append( C );

			Skipped = !Skipped;
		} else {
			Target.Append( C );
			Skipped = false;
		}

        Row = Source.Next( Row );
	}
}
#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtcllbkpersonnalization
{
public:
	xhtcllbkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtcllbkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtcllbkpersonnalization Tutor;
