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

void xhtcllbk::EscapeQuotes(
    const str::string_ &Source,
    str::string_ &Target )
{
    sdr::row__ Row = Source.First();

    while ( Row != E_NIL ) {
		switch ( Source( Row ) ) {
		case '\\':
			Target.Append("\\\\");
			break;
		case '"':
			Target.Append("\\\"");
			break;
		case 10:
			Target.Append( "\\n" );
			break;
		case 13:
			Target.Append( "\\r" );
			break;
		default:
	        Target.Append( Source( Row ) );
			break;
		}

        Row = Source.Next( Row );
    }
}

bso::bool__ xhtcllbk::Fill(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	bso::char__ EntrySeparator,
	bso::char__ FieldSeparator,
	bso::char__ EscapeChar )
{
	bso::bool__ Success = false;
ERRProlog
	str::string String;
	str::strings Strings;
	xtf::error__ Error = xtf::e_Undefined;
	bso::char__ C;
	xtf::utf__ UTF;
	bso::bool__ Escaping = false;
ERRBegin
	String.Init();
	Strings.Init();

	while ( !Flow.EndOfFlow( Error ) ) {
		UTF.Init();
		C = Flow.Get( UTF );

		if ( C == EscapeChar ) {
			if ( Escaping )
				String.Append( EscapeChar );
			Escaping = !Escaping;
		} else if ( C == EntrySeparator ) {
			if ( Escaping ) {
				String.Append( EntrySeparator );
				Escaping = false;
			} else {
				Strings.Append( String );
				String.Init();
			}
		} else if ( C == FieldSeparator ) {
			if ( Escaping ) {
				String.Append( FieldSeparator );
				Escaping = false;
			} else {
				Strings.Append( String );
				String.Init();
				Table.Append( Strings );
				Strings.Init();
			}
		} else if ( Escaping )
			ERRReturn;
		else
			String.Append( (bso::char__ *)UTF.Data, UTF.Size );
	}

	if ( !Escaping )
		if ( Error == xtf::e_NoError ) {
			if ( String.Amount() )
				Strings.Append( String );

			if ( Strings.Amount() )
				Table.Append( Strings );

			Success = true;
		}
ERRErr
ERREnd
ERREpilog
	return Success;
}

void xhtcllbk::EscapeEscapeChar(
	const str::string_ &Source,
	str::string_ &Target,
	bso::char__ EscapeChar )
{
    sdr::row__ Row = Source.First();

    while ( Row != E_NIL ) {
		if ( Source(Row) == '\\' )
			Target.Append('\\' );

        Target.Append( Source( Row ) );

		Row = Source.Next( Row );
    }
}



