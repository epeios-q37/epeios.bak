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

#define TXMTBL_COMPILATION_

#include "txmtbl.h"

using namespace txmtbl;

static inline bso::bool__ HandleEscape_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape,
	bso::bool__ &EOX,
	bso::char__ &C )
{
	bso::bool__ Retry = false;
	xtf::error__ Error = xtf::e_NoError;

	if ( Flow.EndOfFlow( Error ) )
		qRFwk();

	switch( C = Flow.Get() ) {
	case 'n':
		C = '\n';
		break;
	case 'r':
		C = '\r';
		break;
	case 's':
		C = Separator;
		break;
	case 't':
		C = '\t';
		break;
	case '\n':
		if ( !( EOX = Flow.EndOfFlow( Error ) ) ) {
			if ( Flow.View() == '\r' )
				Flow.Get();
			Retry = true;
		}
		break;
	case '\r':
		if ( !( EOX = Flow.EndOfFlow( Error ) ) ) {
			if ( Flow.View() == '\n' )
				Flow.Get();
			Retry = true;
		}
		break;
	case '\t':
		Retry = true;
		break;
	default:
		if ( C != Escape )
			qRFwk();
		break;
	}

	if ( Error != xtf::e_NoError )
		qRFwk();

	return Retry;
}

static inline bso::bool__ IsNotEndOfCell_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape,
	bso::bool__ &EOX,
	bso::char__ &C,
	xtf::sUTF *UTF = NULL)
{
	bso::bool__ Loop = false;
	xtf::error__ Error = xtf::e_NoError;
	xtf::sUTF TempUTF;

	if ( !( EOX = Flow.EndOfFlow( Error ) ) ) {
		do {
      TempUTF.Init();
			if ( ( C = Flow.Get( TempUTF ) ) == Escape )
				Loop = HandleEscape_( Flow, Separator, Escape, EOX, ((bso::sChar *)TempUTF.Data)[0] );
			else
				Loop = false;
		} while ( Loop );

		if ( UTF != NULL )
      *UTF = TempUTF;
	}

	if ( Error != xtf::e_NoError )
		qRFwk();

	return !EOX && ( C != Separator ) && ( C != '\n' ) && ( C != '\r' );
}

static inline txmtbl::delimiter GetDelimiter_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	bso::bool__ EOX,
	bso::char__ C )
{
	xtf::error__ Error = xtf::e_NoError;

	if ( EOX )
		return txmtbl::dEOF;
	else if ( C == '\n' )
	{
		if ( !Flow.EndOfFlow( Error ) && ( Flow.View() == '\r' ) )
			Flow.Get();

		if ( Error != xtf::e_NoError )
			qRFwk();

		return txmtbl::dEOL;
	}
	else if ( C == '\r' )
	{
		if ( !Flow.EndOfFlow( Error ) && ( Flow.View() == '\n' ) )
			Flow.Get();

		if ( Error != xtf::e_NoError )
			qRFwk();

		return txmtbl::dEOL;
	}
	else if ( C == Separator )
		return txmtbl::dSeparator;
	else
	{
		qRFwk();
		return txmtbl::d_Undefined;
	}
}

txmtbl::delimiter txmtbl::GetCell(
	xtf::extended_text_iflow__ &Flow,
	cell_ &Cell,
	separator__ Separator,
	escape__ Escape )
{
	bso::bool__ EOX = false;
	bso::char__ C = 0;
	xtf::sUTF UTF;

	Cell.Location( Flow.Position().Column );

	UTF.Init();

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX, C, &UTF ) ) {
		Cell.Append( (const bso::sChar *)UTF.Data, UTF.Size );
		UTF.Init();
	}

	return GetDelimiter_( Flow, Separator, EOX, C );
}

txmtbl::delimiter txmtbl::SkipCell(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape )
{
	bso::bool__ EOX = false;
	bso::sChar C = 0;

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX, C ) );

	return GetDelimiter_( Flow, Separator, EOX, C );
}

bso::bool__ txmtbl::GetLine(
	xtf::extended_text_iflow__ &Flow,
	line_ &Line,
	separator__ Separator,
	escape__ Escape )
{
	bso::bool__ Result = false;
qRH
	cell Cell;
	bso::bool__ Loop = false, ThereIsCell = false;
	xtf::error__ Error = xtf::e_NoError;
qRB
	Cell.Init();

	Line.Location( Flow.Position().Line );

	do {
		Cell.Init();

		Loop = ( GetCell( Flow, Cell, Separator, Escape ) == txmtbl::dSeparator ) && !Flow.EndOfFlow( Error );

		if ( Loop || Cell.Amount() || Line.Amount() ) {
			Line.Add( Cell );
			ThereIsCell = true;
		}

	} while ( Loop );

	Result = ThereIsCell || !Flow.EndOfFlow( Error );

	if ( Error != xtf::e_NoError )
		qRFwk();
qRR
qRT
qRE

	return Result;
}

void line_::Erase_( stack_ &Stack )
{
	while( Stack.Amount() != 0 )
		cells_::Remove( Stack.Pop() );
}

amount__ line_::RemoveEmptyCells( void )
{
	amount__ Amount = 0;
qRH
	sdr::row__ Current = First();
	stack Stack;
qRB
	Stack.Init();

	while( Current != qNIL )
	{
		if ( !Get( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}

	Erase_( Stack );
qRR
qRT
qRE
	return Amount;
}

sdr::row__ line_::FirstNonEmptyCell( void ) const
{
	sdr::row__ Current = First();

	while( ( Current != qNIL ) && !Get( Current ).Amount() )
		Current = Next( Current );

	return Current;
}

sdr::row__ line_::LastNonEmptyCell( void ) const
{
	sdr::row__ Current = Last();

	while( ( Current != qNIL ) && !Get( Current ).Amount() )
		Current = Previous( Current );

	return Current;
}

amount__ line_::RemoveHeadingEmptyCells( void )
{
	amount__ Amount = 0;
qRH
	sdr::row__ Current = FirstNonEmptyCell();
	stack Stack;
qRB
	Stack.Init();

	if ( Current != qNIL )
	{
		Current = Previous( Current );

		while( Current != qNIL )
		{
			Stack.Push( Current );

			Amount++;

			Current = Previous( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
qRR
qRT
qRE
	return Amount;
}

amount__ line_::RemoveTailingEmptyCells( void )
{
	amount__ Amount = 0;
qRH
	sdr::row__ Current = LastNonEmptyCell();
	stack Stack;
qRB

	Stack.Init();

	if ( Current != qNIL )
	{
		Current = Next( Current );

		while( Current != qNIL )
		{

			Stack.Push( Current );

			Amount++;

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
qRR
qRT
qRE
	return Amount;
}

amount__ line_::RemoveCentralEmptyCells( void )
{
	amount__ Amount = 0;
qRH
	sdr::row__
		Current = FirstNonEmptyCell(),
		Last = LastNonEmptyCell();
	stack Stack;
qRB
	Stack.Init();

	if ( Current != Last )
	{
		Current = Next( Current );

		while( Current != Last )
		{
			if ( !Get( Current ).Amount() )
			{
				Stack.Push( Current );
				Amount++;
			}

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() && ( Current == qNIL ) )
		RemoveAllCells();
qRR
qRT
qRE
	return Amount;
}

amount__ line_::RemoveCellsAt( sdr::row__ Position )
{
	amount__ Amount = 0;
qRH
	stack Stack;
qRB
	Stack.Init();

	while( Position != qNIL )
	{
		Stack.Push( Position );

		Amount++;

		Position = Next( Position );
	}

	Erase_( Stack );
qRR
qRT
qRE
	return Amount;
}

static inline bool IsComment_(
	const cell_ &Cell,
	bso::char__ Marker )
{
	return Cell.Amount() && ( Cell( 0 ) == Marker );
}

amount__ line_::RemoveComment( bso::char__ Marker )
{
	sdr::row__ Position = First();

	while( ( Position != qNIL ) && !IsComment_( Get( Position ), Marker ) )
		Position = Next( Position );

	if ( Position != qNIL )
		return RemoveCellsAt( Position );
	else
		return 0;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const line_ &Line )
{
	sdr::row__ Current;

	Current = Line.First();

	if ( Current != qNIL )
	{
		Flow << Line( Current );
		Current = Line.Next( Current );
	}

	while( Current != qNIL )
	{
		Flow << txf::tab << Line( Current );
		Current = Line.Next( Current );
	}

	return Flow;
}

bso::bool__ txmtbl::GetFirstNonEmptyLine(
	xtf::extended_text_iflow__ &Flow,
	line_ &Line,
	separator__ Separator,
	escape__ Escape )
{
	xtf::error__ Error = xtf::e_NoError;

	if ( Flow.EndOfFlow( Error ) ) {
		if ( Error != xtf::e_NoError )
			qRFwk();

		return false;
	} else
	{
		do
		{
			Line.Init();
			GetLine( Flow, Line, Separator, Escape );

			Line.RemoveEmptyCells();

		} while( !Line.Amount() && !Flow.EndOfFlow( Error ) );

		if ( Error != xtf::e_NoError )
			qRFwk();

		return Line.Amount() != 0;
	}
}


void txmtbl::GetTable(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	separator__ Separator,
	escape__ Escape )
{
qRH
	line Line;
	xtf::error__ Error = xtf::e_NoError;
qRB
	while( !Flow.EndOfFlow( Error ) )
	{
		Line.Init();

		Line.Location( Flow.Position().Line );

		GetLine( Flow, Line, Separator, Escape );

		Table.AddLine( Line );
	}

	if ( Error != xtf::e_NoError )
		qRFwk();
qRR
qRT
qRE
}

void table_::Erase_( stack_ &Stack )
{
	while( Stack.Amount() != 0 )
		lines_::Remove( Stack.Pop() );
}

void table_::RemoveEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != qNIL )
	{
		lines_::operator ()( Current ).RemoveEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveHeadingEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != qNIL )
	{
		lines_::operator ()( Current ).RemoveHeadingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveTailingEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != qNIL )
	{
		lines_::operator ()( Current ).RemoveTailingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveCentralEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != qNIL )
	{
		lines_::operator()( Current ).RemoveCentralEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveComments( bso::char__ Marker )
{
	sdr::row__ Current = First();

	while( Current != qNIL )
	{
		lines_::operator()( Current ).RemoveComment( Marker );

		Current = Next( Current );
	}

	lines_::Flush();
}

amount__ table_::RemoveEmptyLines( void )
{
	amount__ Amount = 0;
qRH
	sdr::row__ Current = First();
	stack Stack;
qRB
	Stack.Init();

	while( Current != qNIL )
	{
		if ( !Get( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}

	Erase_( Stack );
qRR
qRT
qRE
	return Amount;

}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const table_ &Table )
{
	sdr::row__ Current;

	Current = Table.First();

	if ( Current != qNIL )
	{
		Flow << Table( Current );
		Current = Table.Next( Current );
	}

	while( Current != qNIL )
	{
		Flow << txf::nl << Table( Current );
		Current = Table.Next( Current );
	}

	return Flow;
}

