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

#define PRSHCT_COMPILATION_
#include "prshct.h"

enum type {
	tUnknow,
	// Title,
	tTitle,
	// Begin of list mark.
	tBeginList,
	// End of list mark.
	tEndList,
	// Only the marker.
	tOrphan,
	// Subtitle.
	tSubtitle,
	tComment,
	tReference,
	tNone,
};


typedef bso__sbyte level__;

typedef bso__char marker__;

//#define issign( s ) ( strchr( "+-", s ) )


static void GetLine_(
	xtf::extended_text_iflow___ &IFlow,
	str_string_ &Line )
{
qRH
	txmpgh_paragraph Paragraph;
qRB
	Paragraph.Init();
	Paragraph.Parse( IFlow );

	Paragraph.Merge( Line );
qRR
qRT
qRE
}

static void Trim_( str_string_ &Line )
{
	SIZE__ Amount = 0;
	SIZE__ Size = Line.Amount();

	while( ( Size > Amount ) && isspace( Line( Amount ) ) )
		Amount++;

	Line.Delete( 0, Amount );
}

static inline type PurgeAndAnalyze_(
	str_string_ &Line,
	level__ &Level,
	marker__ Marker )
{
	type Type = tUnknow;
	bso__char M = 0, Q = 0, L = 0;
	SIZE__ Amount = 0;

	Trim_( Line );

	switch( Line.Amount() ) {
	default:
		L = Line( 2 );
		Amount++;
	case 2:
		Q = Line( 1 );
		Amount++;
	case 1:
		M = Line( 0 );
		Amount++;
	case 0:
		break;
	}

	if ( isspace( Q ) )
	{
		L = Q;
		Amount--;
	}

	if ( M == Marker ) {
		switch( Q ) {
		case 'l':
			Type = tBeginList;
			break;
		case 'L':
			Type = tEndList;
			break;
		case 's':
			Type = tSubtitle;
			break;
		case 'c':
			Type = tComment;
			break;
		case 'r':
			Type = tReference;
			break;
		case 0:
			Type = tOrphan;
			break;
		case '+':
			Type = tTitle;
			Level = 1;
			break;
		case '-':
			Type = tTitle;
			Level = -1;
			break;
		default:
			if ( isdigit( Q ) ) {
				Type = tTitle;
				Level = '0' - Q;
			} else if ( Q == Marker ) {
				Type = tNone;
				Amount = 1;
			} else if ( !isspace( Q ) ) 
				return tUnknow;
			else {
				Type = tTitle;
				Level = 0;
			}
			break;
		}

		if ( !isspace( L )
			 && ( L != 0 )
			 && ( Type != tNone ) )
			Type = tUnknow;
		else
			Line.Delete( 0, Amount );
	} else 
		Type = tNone;

	Trim_( Line );

	return Type;
}

static POSITION__ Rewind_(
	const txmhch_hierarchical_text_ &Text,
	POSITION__ P,
	level__ Level )
{
	while( Level-- && ( P != NONE ) )
		P = Text.Parent( P );

	return P;
}

#define RL	{\
				if ( ErrH != err::hSkip )\
					ERRf();\
				Error = prshct::rLevel;\
			}

#define RS	{\
				if ( ErrH != err::hSkip )\
					ERRf();\
				Error = prshct::rSyntax;\
			}


POSITION__ PRSHCTParse(
	xtf::extended_text_iflow___ &IFlow,
	txmhch_hierarchical_text_ &Text,
	bso__char Marker,
	err::handle ErrH,
	prshct::error &UError )
{
	POSITION__ Root = NONE;
qRH
	str_string Paragraph;
	POSITION__ P = NONE;
	level__ Level = 0;
	txmhch::type Type = txmhch::tNormal;
	bso__bool ToCreate = false;
	prshct::error Error = prshct::rOK;
qRB

	while ( !IFlow.EOX() && ( P == NONE ) ) {
		Paragraph.Init();
		IFlow.GetLine( Paragraph );

		switch( PurgeAndAnalyze_( Paragraph, Level, Marker ) ) {
		case txmhch::tTitle:
			if ( Level != 0 ) {
				RL;
				break;
			}
			Root = P = Text.Create( Paragraph, Type );
			break;
		case txmhch::tUnknow:
			RS;
			break;
		default:
			if ( Paragraph.Amount() )
				RL;
			break;
		}
	}

	while ( !IFlow.EOX() && ( Error == prshct::rOK) ) {
		Paragraph.Init();
		IFlow.GetLine( Paragraph );

		switch( PurgeAndAnalyze_( Paragraph, Level, Marker ) ) {
		case tBeginList:
			P = Text.BecomeLast( Paragraph, Type, P );
			break;
		case tEndList:
			if ( ( P = Rewind_( Text, P, 1 ) ) == NONE )
				RL
			break;
		case tSubtitle:
			if ( Paragraph.Amount() == 0 )
				Type = txmhch::tSubtitle;
			else
				Text.BecomeLast( Paragraph, txmhch::tSubtitle, P );
			break;
		case tComment:
			if ( Paragraph.Amount() == 0 )
				Type = txmhch::tComment;
			else
				Text.BecomeLast( Paragraph, txmhch::tComment, P );
			break;
		case tReference:
			if ( Paragraph.Amount() == 0 )
				Type = txmhch::tReference;
			else
				Text.BecomeLast( Paragraph, txmhch::tReference, P );
			break;
		case tNone:
			if ( Paragraph.Amount() )
				Text.BecomeLast( Paragraph, Type, P );
			break;
		case tOrphan:
			if ( !Paragraph.Amount() ) {
				Type = txmhch::tNormal;
				break;
			}
		case tTitle:
			if ( Level < 0 ) {
				if ( ToCreate ) {
					Level++;
					ToCreate = false;
				}
				if ( ( P = Rewind_( Text, P, -Level ) ) == NONE )
					RL
				else if ( Paragraph.Amount() )
				P = Text.BecomeNext( Paragraph, txmhch::tTitle, P );
			} else if ( Level == 0 ) {
				if ( ToCreate ) {
					P = Text.BecomeLast( Paragraph, txmhch::tTitle, P );
					ToCreate = false;
				} else
					P = Text.BecomeNext( Paragraph, txmhch::tTitle, P );
			} else if ( Level == 1 ) {
				if ( ToCreate )
					RL
				else if ( Paragraph.Amount() )
					P = Text.BecomeLast( Paragraph, txmhch::tTitle, P );
				else
					ToCreate = true;
			} else
				ERRc();
			break;
		case txmhch::tUnknow:
			RS
			break;
		default:
			ERRc();
			break;
		}
	}

	Text.Sync();

	if ( &UError ) 
		UError = Error;
qRR
qRT
qRE
	return Root;
}
