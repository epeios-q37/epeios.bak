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

#if 0 // OLD

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

	while ( Row != E_NIL ) {
		Put_( String( Row ), Flow, Pattern );

		Row = Strings.Next( Row );

		Flow << Pattern[0];	// Séparateur d'entrées.
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

	while ( Row != E_NIL ) {
		Merge_( Strings( Row ), Flow, Pattern );

		Row = Table.Next( Row );

		Flow << FieldSeparator;
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
				if ( String.Amount() )
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
#else // NEW

row__ strmrg::table_::Append( const str::string_ &String )
{
	row__ Row = E_NIL;
ERRProlog
	_item Item;
ERRBegin
	Item.Init( String );

	Row = Main.Append( Items.Append( Item ) );
ERRErr
ERREnd
ERREpilog
	return Row;
}

// Prédéclaration.

static void Append_(
	const _irows_ &SourceRows,
	const _items_ &SourceItems,
	_irows_ &TargetRows,
	_items_ &TargetItems );

static _irow__ Append_(
	const _item_ &SourceItem,
	const _items_ &SourceItems,
	_items_ &TargetItems )
{
	_irow__ Row = E_NIL;
ERRProlog
	_item Item;
ERRBegin
	if ( SourceItem.ContainsString() ) {	// Positionne 'SourceItem()' pour la suite !
		Item.Init( SourceItem.String );
	} else {
		Item.Init();
		Append_( SourceItem.Items, SourceItems, Item.Items, TargetItems );
	}

	Row = TargetItems.Append( Item ) ;
ERRErr
ERREnd
ERREpilog
	return Row;
}

static _irow__ Append_(
	_irow__ SourceRow,
	const _items_ &SourceItems,
	_items_ &TargetItems )
{
	ctn::E_CITEMt( _item_, _irow__ ) SourceItem;

	SourceItem.Init( SourceItems );

	return Append_(SourceItem( SourceRow ), SourceItems, TargetItems );
}

static void Append_(
	const _irows_ &SourceRows,
	const _items_ &SourceItems,
	_irows_ &TargetRows,
	_items_ &TargetItems )	// Récursif !
{
	ctn::E_CITEMt( _item_, _irow__ ) SourceItem;
	row__ Row = SourceRows.First();

	SourceItem.Init( SourceItems );

	while ( Row != E_NIL ) {
		TargetRows.Append( Append_( SourceRows( Row ), SourceItems, TargetItems ) );

		Row = SourceRows.Next( Row );
	}
}

row__ strmrg::table_::Append( const table_ &Table )
{
	row__ Row = E_NIL;
ERRProlog
	_item Item;
	ctn::E_CITEMt( _item_, _irow__ ) SourceItem;
ERRBegin
	SourceItem.Init( Table.Items );

	if ( (Table.Amount() == 1) && ( SourceItem( Table.Main( Table.Main.First() ) ).ContainsString() ) )
		Row = Append( SourceItem().String );
	else {
		Item.Init();

		Append_( Table.Main, Table.Items, Item.Items, Items );

		Row = Main.Append( Items.Append( Item ) );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ strmrg::table_::Append( const str::strings_ &Strings )
{
	row__ Row = E_NIL;
ERRProlog
	table Table;
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ SRow = E_NIL;
ERRBegin
	Table.Init();
	SRow = Strings.First();

	String.Init( Strings );

	while ( SRow != E_NIL ) {
		Table.Append( String( SRow ) );

		SRow = Strings.Next( SRow );
	}

	Row = Append( Table ); 
ERRErr
ERREnd
ERREpilog
	return Row;
}

void strmrg::table_::GetTable(
	row__ Row,
	table_ &Table ) const
{
	ctn::E_CITEMt( _item_, _irow__ ) Item;
	Item.Init( Items );

	Append_( Item( Main( Row ) ).Items, Items, Table.Main, Table.Items );
}

void strmrg::GetTable(
	const _item_ &Item,
	const _items_ &Items,
	table_ &Table )
{
	if ( Item.ContainsString() )
		Table.Append( Item.String );
	else
		Append_( Item.Items, Items, Table.Main, Table.Items );
}

static void Put_(
	const str::string_ &String,
	const tokens__ &Tokens,
	flw::oflow__ &Flow )
{
	const char Pattern[5] = { Tokens.Separator, Tokens.Begin, Tokens.End, Tokens.Escape, 0 };
	sdr::row__ Row = String.First();
	bso::char__ Char = 0;

	while ( Row != E_NIL ) {
		if ( strrchr( Pattern, Char = String( Row ) ) )
			Flow << Tokens.Escape;

		Flow << Char;

		Row = String.Next( Row );
	}
}

static void Merge_(
	const _irows_ &Rows,
	const _items_ &Items,
	const tokens__ &Tokens,
	flw::oflow__ &Flow )	// Récursif !
{
	ctn::E_CITEMt( _item_, _irow__ ) Item;
	row__ Row = Rows.First();
	Item.Init( Items );

	while ( Row != E_NIL ) {
		if ( Item( Rows ( Row ) ).ContainsString() ) {	// Positionne 'Item' pour la suite !
			Put_( Item().String, Tokens, Flow );

			Flow << Tokens.Separator;
		} else {
			Flow << Tokens.Begin;
			Merge_( Item().Items, Items, Tokens, Flow );
			Flow << Tokens.End;
		}

		Row = Rows.Next( Row );
	}
}

void strmrg::Merge(
	const table_ &Table,
	flw::oflow__ &Flow,
	const tokens__ &Tokens )
{
	Merge_( Table.Main, Table.Items, Tokens, Flow );
}

void strmrg::Merge(
	const table_ &Table,
	str::string_ &String,
	const tokens__ &Tokens )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
ERRBegin
	Flow.Init( String );

	Merge( Table, Flow, Tokens );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ strmrg::Split(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	const tokens__ &Tokens )	// Récursif !
{
	bso::bool__ Success = true;
ERRProlog
	str::string String;
	table SubTable;
	bso::bool__ WasEscape = false;
	xtf::utf__ UTF;
	bso::char__ C = 0;
	xtf::error__ Error = xtf::e_Undefined;
ERRBegin
	String.Init();

	while ( !Flow.EndOfFlow( Error ) ) {
		UTF.Init();
		C = Flow.Get( UTF );
		if ( WasEscape ) {
			String.Append( (bso::char__ *)UTF.Data, UTF.Size );
			WasEscape = false;
		} else {
			if ( C == Tokens.Escape ) {
				WasEscape = true;
			} else if ( C == Tokens.Separator ) {
				Table.Append( String );
				String.Init();
			} else if ( C == Tokens.Begin ) {
				if ( String.Amount() != 0 ) {
					Table.Append( String );
					String.Init();
				}

				SubTable.Init();
				if ( !Split( Flow, SubTable, Tokens ) ) {
					Success = false;
					ERRFwk();
				}
				Table.Append( SubTable );
			} else if ( C == Tokens.End ) {
				if ( String.Amount() != 0 )
					Table.Append( String );
				ERRReturn;
			} else {
				String.Append( (bso::char__ *)UTF.Data, UTF.Size );
			}
		}
	}

	if ( ( Error == xtf::e_NoError ) && ( !WasEscape ) ) {
		if ( String.Amount() != 0 )
			Table.Append( String );
	} else
		Success = false;

ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ strmrg::Split(
	const str::string_ &String,
	table_ &Table,
	const tokens__ &Tokens )
{
	flx::E_STRING_IFLOW__ SFlow;
	xtf::extended_text_iflow__ XFlow;

	SFlow.Init( String );
	XFlow.Init( SFlow, utf::f_Guess );

	return Split( XFlow, Table, Tokens );
}

static void GetStrings_(
	const _irows_ &Rows,
	const _items_ &Items,
	str::strings_ &Strings )
{
	ctn::E_CITEMt( _item_, _irow__ ) Item;
	row__ Row = Rows.First();

	Item.Init( Items );

	while ( Row != E_NIL ) {
		if ( !Item(Rows( Row ) ).ContainsString() )
			ERRFwk();

		Strings.Append(Item().String );

		Row = Rows.Next( Row );
	}
}

static void GetStrings_(
	const _item_ &Item,
	const _items_ &Items,
	str::strings_ &Strings )
{
	if ( Item.ContainsString() )
		Strings.Append( Item.String );
	else {
		GetStrings_( Item.Items, Items, Strings );
	}
}

void strmrg::retriever__::GetStrings( str::strings_ &Strings )
{
	ctn::E_CITEMt( _item_, _irow__ ) Item;

	Item.Init( _I() );

	GetStrings_( Item(_R()( _Row ) ), _I(), Strings );

	_Row = _R().Next( _Row );
}


#endif





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
