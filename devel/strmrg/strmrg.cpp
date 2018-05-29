/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define STRMRG__COMPILATION

#include "strmrg.h"

#include "flx.h"

using namespace strmrg;

row__ strmrg::table_::Append( const str::string_ &String )
{
	row__ Row = qNIL;
qRH
	_item Item;
qRB
	Item.Init( Strings.Append( String ) );

	Row = Main.Append( Items.Append( Item ) );
qRR
qRT
qRE
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
	_irow__ Row = qNIL;
qRH
	_item Item;
qRB
	if ( SourceItem.ContainsString() ) {	// Positionne 'SourceItem()' pour la suite !
		Item.Init( TargetStrings.Append( SourceStrings( SourceItem.String() ) ) );
	} else {
		Item.Init();
		Append_( SourceItem.Items, SourceItems, SourceStrings, Item.Items, TargetItems, TargetStrings );
	}

	Row = TargetItems.Append( Item ) ;
qRR
qRT
qRE
	return Row;
}

static _irow__ Append_(
	_irow__ SourceRow,
	const _items_ &SourceItems,
	const _strings_ &SourceStrings,
	_items_ &TargetItems,
	_strings_ &TargetStrings )
{
	ctn::qCMITEMs( _item_, _irow__ ) SourceItem;

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
	row__ Row = SourceRows.First();

	while ( Row != qNIL ) {
		TargetRows.Append( Append_( SourceRows( Row ), SourceItems, SourceStrings, TargetItems, TargetStrings ) );

		Row = SourceRows.Next( Row );
	}
}

row__ strmrg::table_::AppendMono( const table_ &Table )
{
	row__ Row = qNIL;
qRH
	_item Item;
qRB
	if ( (Table.Amount() == 1) && (Table.Items(Table.Main(Table.Main.First())).ContainsString()) ) {
		Row = Append( Table.Strings( Table.Items().String() ) );
	} else {
		Item.Init();

		Append_( Table.Main, Table.Items, Table.Strings, Item.Items, Items, Strings );

		Row = Main.Append( Items.Append( Item ) );
	}
qRR
qRT
qRE
	return Row;
}

void strmrg::table_::AppendMulti( const table_ &Table )
{
qRH
	retriever__ Retriever;
	table SubTable;
qRB
	Retriever.Init( Table );

	while ( Retriever.Availability() != aNone ) {
		SubTable.Init();
		Retriever.GetTable( SubTable );
		AppendMono( SubTable );
	}
qRR
qRT
qRE
}

row__ strmrg::table_::AppendMono( const str::strings_ &Strings )
{
	row__ Row = qNIL;
qRH
	table Table;
	sdr::row__ SRow = qNIL;
qRB
	Table.Init();
	SRow = Strings.First();

	while ( SRow != qNIL ) {
		Table.Append( Strings( SRow ) );

		SRow = Strings.Next( SRow );
	}

	Row = AppendMono( Table ); 
qRR
qRT
qRE
	return Row;
}

void strmrg::table_::AppendMulti( const str::strings_ &Strings )
{

	sdr::row__ Row = Strings.First();

	Row = Strings.First();

	while ( Row != qNIL ) {
		Append( Strings( Row ) );

		Row = Strings.Next( Row );
	}
}

void strmrg::table_::GetTable(
	row__ Row,
	table_ &Table ) const
{
	ctn::qCMITEMs( _item_, _irow__ ) Item;

	Item.Init( Items );

	Append_( Item( Main( Row ) ).Items, Items, Strings, Table.Main, Table.Items, Table.Strings );
}

void strmrg::GetTable(
	const _item_ &Item,
	const _items_ &Items,
	const _strings_ &Strings,
	table_ &Table )
{
	if ( Item.ContainsString() ) {
		Table.Append( Strings( Item.String() ) );
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

	while ( Row != qNIL ) {
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
	ctn::qCMITEMs( _item_, _irow__ ) Item;

	row__ Row = Rows.First();

	Item.Init( Items );

	while ( Row != qNIL ) {
		if ( Item( Rows ( Row ) ).ContainsString() ) {	// Positionne 'Item' pour la suite !

			Put_( Strings( Item().String() ), Tokens, Flow );

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
qRH
	flx::E_STRING_OFLOW___ Flow;
qRB
	Flow.Init( String );

	Merge( Table, Flow, Tokens );
qRR
qRT
qRE
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
qRH
	str::string String;
	table SubTable;
	trace__ Trace = t_Undefined;
	xtf::utf__ UTF;
	bso::char__ C = 0;
	xtf::error__ Error = xtf::e_Undefined;
qRB
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
					qRFwk();
				}
				Table.AppendMono( SubTable );
				Trace = tTable;
			} else if ( C == Tokens.End ) {
				if ( String.Amount() != 0 )
					Table.Append( String );
				qRReturn;
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

qRR
qRT
qRE
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
qRH
	str::strings Strings;
qRB
	Strings.Init();

	GetStrings( Strings );

	if ( Strings.Amount() == 1 )
			Strings.Recall(Strings.First(), String );
	else if ( Strings.Amount() != 0 )
		qRFwk();

qRR
qRT
qRE
}

static void GetStrings_(
	const _irows_ &Rows,
	const _items_ &Items,
	const _strings_ &Strings,
	str::strings_ &Result )
{
	row__ Row = Rows.First();

	while ( Row != qNIL ) {
		if ( !Items(Rows( Row ) ).ContainsString() )
			qRFwk();

		Result.Append( Strings( Items().String() ) );

		Row = Rows.Next( Row );
	}
}

static void GetStrings_(
	const _item_ &Item,
	const _items_ &Items,
	const _strings_ &Strings,
	str::strings_ &Result )
{
	if ( Item.ContainsString() )
		Result.Append( Strings( Item.String() ) );
	else {
		GetStrings_( Item.Items, Items, Strings, Result );
	}
}

void strmrg::retriever__::GetStrings( str::strings_ &Result )
{
	ctn::qCMITEMs( _item_, _irow__ ) Item;

	Item.Init( _I() );

	if ( Item( _R()( _Row ) ).ContainsString() )	// Positionne 'Item()'.
		Result.Append( s_()( Item().String() ) );
	else
		GetStrings_( Item(_R()( _Row ) ), _I(), s_(), Result );

	_Row = _R().Next( _Row );
}
