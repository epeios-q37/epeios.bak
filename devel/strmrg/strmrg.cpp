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

row__ strmrg::table_::Append( const str::string_ &String )
{
	row__ Row = E_NIL;
ERRProlog
	_item Item;
ERRBegin
	Item.Init( Strings.Append( String ) );

	Row = Main.Append( Items.Append( Item ) );
ERRErr
ERREnd
ERREpilog
	return Row;
}

// Prdclaration.
static void Append_(
	const _irows_ &SourceRows,
	const _items_ &SourceItems,
	const _strings_ &SourceStrings,
	_irows_ &TargetRows,
	_items_ &TargetItems,
	_strings_ &TargetStrings );

static _irow__ Append_(
	const _item_ &SourceItem,
	const _items_ &SourceItems,
	const _strings_ &SourceStrings,
	_items_ &TargetItems,
	_strings_ &TargetStrings )
{
	_irow__ Row = E_NIL;
ERRProlog
	_item Item;
	ctn::E_CMITEMt( _string_ , _srow__ ) SourceString;
ERRBegin
	if ( SourceItem.ContainsString() ) {	// Positionne 'SourceItem()' pour la suite !
		SourceString.Init( SourceStrings );
		Item.Init( TargetStrings.Append( SourceString( SourceItem.String() ) ) );
	} else {
		Item.Init();
		Append_( SourceItem.Items, SourceItems, SourceStrings, Item.Items, TargetItems, TargetStrings );
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
	const _strings_ &SourceStrings,
	_items_ &TargetItems,
	_strings_ &TargetStrings )
{
	ctn::E_CMITEMt( _item_, _irow__ ) SourceItem;

	SourceItem.Init( SourceItems );

	return Append_( SourceItem( SourceRow ), SourceItems, SourceStrings, TargetItems, TargetStrings );
}

static void Append_(
	const _irows_ &SourceRows,
	const _items_ &SourceItems,
	const _strings_ &SourceStrings,
	_irows_ &TargetRows,
	_items_ &TargetItems,
	_strings_ &TargetStrings )	// Rcursif (paaaaas bien) !
{
	ctn::E_CMITEMt( _item_, _irow__ ) SourceItem;
	row__ Row = SourceRows.First();

	SourceItem.Init( SourceItems );

	while ( Row != E_NIL ) {
		TargetRows.Append( Append_( SourceRows( Row ), SourceItems, SourceStrings, TargetItems, TargetStrings ) );

		Row = SourceRows.Next( Row );
	}
}

row__ strmrg::table_::Append( const table_ &Table )
{
	row__ Row = E_NIL;
ERRProlog
	_item Item;
	ctn::E_CMITEMt( _item_, _irow__ ) SourceItem;
	ctn::E_CMITEMt( _string_, _srow__ ) SourceString;
ERRBegin
	SourceItem.Init( Table.Items );

	if ( (Table.Amount() == 1) && (SourceItem(Table.Main(Table.Main.First())).ContainsString()) ) {
		SourceString.Init( Table.Strings );
		Row = Append( SourceString( SourceItem().String() ) );
	} else {
		Item.Init();

		Append_( Table.Main, Table.Items, Table.Strings, Item.Items, Items, Strings );

		Row = Main.Append( Items.Append( Item ) );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}

row__ strmrg::table_::AppendMono( const str::strings_ &Strings )
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

void strmrg::table_::AppendMulti( const str::strings_ &Strings )
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) String;
	sdr::row__ Row = Strings.First();
ERRBegin
	Row = Strings.First();

	String.Init( Strings );

	while ( Row != E_NIL ) {
		Append( String( Row ) );

		Row = Strings.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void strmrg::table_::GetTable(
	row__ Row,
	table_ &Table ) const
{
	ctn::E_CMITEMt( _item_, _irow__ ) Item;
	Item.Init( Items );

	Append_( Item( Main( Row ) ).Items, Items, Strings, Table.Main, Table.Items, Table.Strings );
}

void strmrg::GetTable(
	const _item_ &Item,
	const _items_ &Items,
	const _strings_ &Strings,
	table_ &Table )
{
	ctn::E_CMITEMt( _string_, _srow__ ) String;

	if ( Item.ContainsString() ) {
		String.Init( Strings );
		Table.Append( String( Item.String() ) );
	} else
		Append_( Item.Items, Items, Strings, Table.Main, Table.Items, Table.Strings );
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
	const _strings_ &Strings,
	const tokens__ &Tokens,
	flw::oflow__ &Flow )	// Rcursif (paaas bien !) !
{
	ctn::E_CMITEMt( _item_, _irow__ ) Item;
	ctn::E_CMITEMt( _string_, _srow__ ) String;

	row__ Row = Rows.First();
	Item.Init( Items );
	String.Init( Strings );

	while ( Row != E_NIL ) {
		if ( Item( Rows ( Row ) ).ContainsString() ) {	// Positionne 'Item' pour la suite !

			Put_( String( Item().String() ), Tokens, Flow );

			Flow << Tokens.Separator;
		} else {
			Flow << Tokens.Begin;
			Merge_( Item().Items, Items, Strings, Tokens, Flow );
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
	Merge_( Table.Main, Table.Items, Table.Strings, Tokens, Flow );
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

enum trace__ {
	tOther,
	tTable,
	tEscape,
	t_amount,
	t_Undefined
};

bso::bool__ strmrg::Split(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	const tokens__ &Tokens )	// Rcursif !
{
	bso::bool__ Success = true;
ERRProlog
	str::string String;
	table SubTable;
	trace__ Trace = t_Undefined;
	xtf::utf__ UTF;
	bso::char__ C = 0;
	xtf::error__ Error = xtf::e_Undefined;
ERRBegin
	String.Init();

	while ( !Flow.EndOfFlow( Error ) ) {
		UTF.Init();
		C = Flow.Get( UTF );
		if ( Trace == tEscape ) {
			String.Append( (bso::char__ *)UTF.Data, UTF.Size );
			Trace = tOther;
		} else {
			if ( C == Tokens.Escape ) {
				Trace = tEscape;
			} else if ( C == Tokens.Separator ) {
				if ( ( String.Amount() != 0 ) || ( Trace != tTable ) )
					Table.Append( String );
				Trace = tOther;
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
				Trace = tTable;
			} else if ( C == Tokens.End ) {
				if ( String.Amount() != 0 )
					Table.Append( String );
				ERRReturn;
			} else {
				String.Append( (bso::char__ *)UTF.Data, UTF.Size );
				Trace = tOther;
			}
		}
	}

	if ( ( Error == xtf::e_NoError ) && ( Trace != tEscape ) ) {
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

void strmrg::retriever__::GetString( str::string_ &String )
{
ERRProlog
	str::strings Strings;
ERRBegin
	Strings.Init();

	GetStrings( Strings );

	if ( Strings.Amount() != 1 )
		ERRFwk();

	Strings.Recall(Strings.First(), String );
ERRErr
ERREnd
ERREpilog
}

static void GetStrings_(
	const _irows_ &Rows,
	const _items_ &Items,
	const _strings_ &Strings,
	str::strings_ &Result )
{
	ctn::E_CMITEMt( _item_, _irow__ ) Item;
	ctn::E_CMITEMt( _string_, _srow__ ) String;
	row__ Row = Rows.First();

	Item.Init( Items );
	String.Init( Strings );

	while ( Row != E_NIL ) {
		if ( !Item(Rows( Row ) ).ContainsString() )
			ERRFwk();

		Result.Append( String( Item().String() ) );

		Row = Rows.Next( Row );
	}
}

static void GetStrings_(
	const _item_ &Item,
	const _items_ &Items,
	const _strings_ &Strings,
	str::strings_ &Result )
{
	ctn::E_CMITEMt( _string_, _srow__ ) String;
	String.Init( Strings );

	if ( Item.ContainsString() )
		Result.Append( String( Item.String() ) );
	else {
		GetStrings_( Item.Items, Items, Strings, Result );
	}
}

void strmrg::retriever__::GetStrings( str::strings_ &Result )
{
	ctn::E_CMITEMt( _item_, _irow__ ) Item;
	ctn::E_CMITEMt( _string_, _srow__ ) String;

	Item.Init( _I() );
	String.Init( _S() );

	if ( Item( _R()( _Row ) ).ContainsString() )	// Positionne 'Item()'.
		Result.Append( String( Item().String() ) );
	else
		GetStrings_( Item(_R()( _Row ) ), _I(), _S(), Result );

	_Row = _R().Next( _Row );
}
