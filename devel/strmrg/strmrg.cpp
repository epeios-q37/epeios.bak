/*
	'strmrg.cpp' by Claude SIMON (http://zeusw.org/).

	'strmrg' is part of the Epeios framework.

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

#define STRMRG__COMPILATION

#include "strmrg.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "flx.h"

using namespace strmrg;

typedef bso::char__ pattern__[4];

static void Put_(
	const str::string_ &String,
	flw::oflow__ &Flow,
	const pattern__ &Pattern )
{
	bso::char__ C = 0;
	sdr::row__ Row = String.First();

	while ( Row != E_NIL ) {
		if ( strchr( Pattern, C = String( Row ) ) != NULL )
			Flow << Pattern[2];	// Caractère d'échappement.

		Flow << C;

		Row = String.Next( Row );
	}
}

static void Merge_(
	const str::strings_ &Strings,
	flw::oflow__ &Flow,
	const pattern__ &Pattern )
{
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ Row = Strings.First();

	String.Init( Strings );

	if ( Row != E_NIL ) {
		Put_( String( Row ), Flow, Pattern );

		Row = Strings.Next( Row );
	}

	while ( Row != E_NIL ) {
		Flow << Pattern[0];	// Séparateur d'entrées.

		Put_( String( Row ), Flow, Pattern );

		Row = Strings.Next( Row );
	}
}

static void Fill_(
	pattern__ &Pattern,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	Pattern[0] = EntrySeparator;
	Pattern[1] = FieldSeparator;
	Pattern[2] = EscapeChar;
	Pattern[3] = 0;
}

void strmrg::Merge(
	const table_ &Table,
	flw::oflow__ &Flow,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	ctn::E_CITEM( str::strings_ ) Strings;
	sdr::row__ Row = Table.First();
	pattern__ Pattern;

	Fill_( Pattern, EntrySeparator, FieldSeparator, EscapeChar );

	Strings.Init( Table );

	if ( Row != E_NIL ) {
		Merge_( Strings( Row ), Flow, Pattern );

		Row = Table.Next( Row );
	}

	while ( Row != E_NIL ) {
		Flow << FieldSeparator;

		Merge_( Strings( Row ), Flow, Pattern );

		Row = Table.Next( Row );
	}
}

void strmrg::Merge(
	const table_ &Table,
	str::string_ &String,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
ERRBegin
	Flow.Init( String );

	Merge( Table, Flow, EntrySeparator, FieldSeparator, EscapeChar );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ strmrg::Split(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
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

bso::bool__ strmrg::Split(
	const str::string_ &String,
	table_ &Table,
	char EntrySeparator,
	char FieldSeparator,
	char EscapeChar )
{
	flx::E_STRING_IFLOW__ SFlow;
	xtf::extended_text_iflow__ XFlow;

	SFlow.Init( String );
	XFlow.Init( SFlow, utf::f_Guess );

	return Split( XFlow, Table, EntrySeparator, FieldSeparator, EscapeChar );
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class strmrgpersonnalization
{
public:
	strmrgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~strmrgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static strmrgpersonnalization Tutor;
