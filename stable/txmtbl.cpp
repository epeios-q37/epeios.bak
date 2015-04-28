/*
	'txmtbl' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'txmtbl' header file ('txmtbl.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define TXMTBL__COMPILATION

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
	xtf::utf__ UTF;

	UTF.Init();

	if ( Flow.EndOfFlow( Error ) )
		ERRDta();
		
	switch( C = Flow.Get( UTF ) ) {
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
			if ( Flow.View( UTF ) == '\r' )
				Flow.Get( UTF );
			Retry = true;
		}
		break;
	case '\r':
		if ( !( EOX = Flow.EndOfFlow( Error ) ) ) {
			if ( Flow.View( UTF ) == '\n' )
				Flow.Get( UTF );
			Retry = true;
		}
		break;
	case '\t':
		Retry = true;
		break;
	default:
		if ( C != Escape )
			ERRDta();
		break;
	}

	if ( Error != xtf::e_NoError )
		ERRDta();
	
	return Retry;
}

static inline bso::bool__ IsNotEndOfCell_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape,
	bso::bool__ &EOX,
	bso::char__ &C )
{
	bso::bool__ Loop = false;
	xtf::error__ Error = xtf::e_NoError;
	xtf::utf__ UTF;

	UTF.Init();

	if ( !( EOX = Flow.EndOfFlow( Error ) ) )
		do {		
			if ( ( C = Flow.Get( UTF ) ) == Escape )
				Loop = HandleEscape_( Flow, Separator, Escape, EOX, C );
			else
				Loop = false;
		} while ( Loop );
		
	if ( Error != xtf::e_NoError )
		ERRDta();

	return !EOX && ( C != Separator ) && ( C != '\n' ) && ( C != '\r' );
}

static inline txmtbl::delimiter GetDelimiter_( 
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	bso::bool__ EOX,
	bso::char__ C )
{
	xtf::error__ Error = xtf::e_NoError;
	xtf::utf__ UTF;

	UTF.Init();

	if ( EOX )
		return txmtbl::dEOF;
	else if ( C == '\n' )
	{
		if ( !Flow.EndOfFlow( Error ) && ( Flow.View( UTF ) == '\r' ) )
			Flow.Get( UTF );

		if ( Error != xtf::e_NoError )
			ERRDta();

		return txmtbl::dEOL;
	}
	else if ( C == '\r' )
	{
		if ( !Flow.EndOfFlow( Error ) && ( Flow.View( UTF ) == '\n' ) )
			Flow.Get( UTF );

		if ( Error != xtf::e_NoError )
			ERRDta();

		return txmtbl::dEOL;
	}
	else if ( C == Separator )
		return txmtbl::dSeparator;
	else
	{
		ERRFwk();
		return txmtbl::dUnknow;
	}
}

txmtbl::delimiter txmtbl::GetCell(
	xtf::extended_text_iflow__ &Flow,
	cell_ &Cell,
	separator__ Separator,
	escape__ Escape )
{
	bso::char__ C = 0;
	bso::bool__ EOX = false;

	Cell.Init();

	Cell.Location( Flow.Position().Column );

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX, C ) ) 
		Cell.Append( C );
	
	return GetDelimiter_( Flow, Separator, EOX, C );
}

txmtbl::delimiter txmtbl::SkipCell(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape )
{
	bso::char__ C = 0;
	bso::bool__ EOX = false;

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX,C ) );

	return GetDelimiter_( Flow, Separator, EOX, C );
}

bso::bool__ txmtbl::GetLine(
	xtf::extended_text_iflow__ &Flow,
	line_ &Line,
	separator__ Separator,
	escape__ Escape )
{
	bso::bool__ Result = false;
ERRProlog
	cell Cell;
	bso::bool__ Loop;
	xtf::error__ Error = xtf::e_NoError;
ERRBegin
	Cell.Init();

	Line.Location( Flow.Position().Line );

	do {
		Loop = ( GetCell( Flow, Cell, Separator, Escape ) == txmtbl::dSeparator ) && !Flow.EndOfFlow( Error );

		if ( Loop || Cell.Amount() || Line.Amount() )
			Line.Add( Cell );

	} while ( Loop );

	Result = !Flow.EndOfFlow( Error );

	if ( Error != xtf::e_NoError )
		ERRDta();
ERRErr
ERREnd
ERREpilog

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
ERRProlog
	ctn::E_CMITEM( cell_ ) Cell;
	sdr::row__ Current = First();
	stack Stack;
ERRBegin
	Cell.Init( *this );

	Stack.Init();

	while( Current != E_NIL )
	{
		if ( !Cell( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}
	
	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;
}

sdr::row__ line_::FirstNonEmptyCell( void ) const
{
	ctn::E_CMITEM( cell_ ) Cell;
	sdr::row__ Current = First();

	Cell.Init( *this );

	while( ( Current != E_NIL ) && !Cell( Current ).Amount() )
		Current = Next( Current );

	return Current;
}

sdr::row__ line_::LastNonEmptyCell( void ) const
{
	ctn::E_CMITEM( cell_ ) Cell;
	sdr::row__ Current = Last();

	Cell.Init( *this );

	while( ( Current != E_NIL ) && !Cell( Current ).Amount() )
		Current = Previous( Current );

	return Current;
}

amount__ line_::RemoveHeadingEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	sdr::row__ Current = FirstNonEmptyCell();
	stack Stack;
ERRBegin
	Stack.Init();

	if ( Current != E_NIL )
	{
		Current = Previous( Current );

		while( Current != E_NIL )
		{
			Stack.Push( Current );

			Amount++;

			Current = Previous( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveTailingEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	sdr::row__ Current = LastNonEmptyCell();
	stack Stack;
ERRBegin

	Stack.Init();

	if ( Current != E_NIL )
	{
		Current = Next( Current );

		while( Current != E_NIL )
		{

			Stack.Push( Current );

			Amount++;

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveCentralEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	ctn::E_CMITEM( cell_ ) Cell;
	sdr::row__
		Current = FirstNonEmptyCell(),
		Last = LastNonEmptyCell();
	stack Stack;
ERRBegin
	Stack.Init();

	Cell.Init( *this );

	if ( Current != Last )
	{
		Current = Next( Current );

		while( Current != Last )
		{
			if ( !Cell( Current ).Amount() )
			{
				Stack.Push( Current );
				Amount++;
			}

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() && ( Current == E_NIL ) )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveCellsAt( sdr::row__ Position )
{
	amount__ Amount = 0;
ERRProlog
	stack Stack;
ERRBegin
	Stack.Init();

	while( Position != E_NIL )
	{
		Stack.Push( Position );

		Amount++;

		Position = Next( Position );
	}

	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;
}

static inline bool IsCommentary_(
	const cell_ &Cell,
	bso::char__ Marker )
{
	return Cell.Amount() && ( Cell( 0 ) == Marker );
}

amount__ line_::RemoveComment( bso::char__ Marker )
{
	sdr::row__ Position = First();
	ctn::E_CMITEM( cell_ ) Cell;

	Cell.Init( *this );

	while( ( Position != E_NIL ) && !IsCommentary_( Cell( Position ), Marker ) )
		Position = Next( Position );

	if ( Position != E_NIL )
		return RemoveCellsAt( Position );
	else
		return 0;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const line_ &Line )
{
	sdr::row__ Current;
	ctn::E_CMITEM( cell_ ) Cell;

	Cell.Init( Line );

	Current = Line.First();

	if ( Current != E_NIL )
	{
		Flow << Cell( Current );
		Current = Line.Next( Current );
	}

	while( Current != E_NIL )
	{
		Flow << txf::tab << Cell( Current );
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
			ERRDta();

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
			ERRDta();

		return Line.Amount() != 0;
	}
}


void txmtbl::GetTable(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	separator__ Separator,
	escape__ Escape )
{
ERRProlog
	line Line;
	xtf::error__ Error = xtf::e_NoError;
ERRBegin
	while( !Flow.EndOfFlow( Error ) )
	{
		Line.Init();

		Line.Location( Flow.Position().Line );

		GetLine( Flow, Line, Separator, Escape );

		Table.AddLine( Line );
	}

	if ( Error != xtf::e_NoError )
		ERRDta();
ERRErr
ERREnd
ERREpilog
}

void table_::Erase_( stack_ &Stack )
{
	while( Stack.Amount() != 0 )
		lines_::Remove( Stack.Pop() );
}

void table_::RemoveEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != E_NIL )
	{
		lines_::operator ()( Current ).RemoveEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveHeadingEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != E_NIL )
	{
		lines_::operator ()( Current ).RemoveHeadingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveTailingEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != E_NIL )
	{
		lines_::operator ()( Current ).RemoveTailingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveCentralEmptyCells( void )
{
	sdr::row__ Current = First();

	while( Current != E_NIL )
	{
		lines_::operator()( Current ).RemoveCentralEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveComments( bso::char__ Marker )
{
	sdr::row__ Current = First();

	while( Current != E_NIL )
	{
		lines_::operator()( Current ).RemoveComment( Marker );

		Current = Next( Current );
	}

	lines_::Flush();
}

amount__ table_::RemoveEmptyLines( void )
{
	amount__ Amount = 0;
ERRProlog
	ctn::E_CITEM( line_ ) Line;
	sdr::row__ Current = First();
	stack Stack;
ERRBegin
	Line.Init( *this );

	Stack.Init();

	while( Current != E_NIL )
	{
		if ( !Line( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}

	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;

}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const table_ &Table )
{
	ctn::E_CITEM( line_ ) Line;
	sdr::row__ Current;

	Line.Init( Table );

	Current = Table.First();

	if ( Current != E_NIL )
	{
		Flow << Line( Current );
		Current = Table.Next( Current );
	}

	while( Current != E_NIL )
	{
		Flow << txf::nl << Line( Current );
		Current = Table.Next( Current );
	}

	return Flow;
}

