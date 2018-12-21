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

#define XDHCMN__COMPILATION

#include "xdhcmn.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xdhcmn;

#define C( name )\
	case f##name:\
	return #name;\
	break


const char *xdhcmn::GetLabel( eFunction Function )
{
	switch ( Function ) {
	C( Execute );
	C( Log );
	C( Alert );
	C( Confirm );
	C( SetLayout );
	C( DressWidgets );
	C( SetContents );
	C( SetTimeout );
	/*
	C( InsertCSSRule );
	C( AppendCSSRule );
	C( RemoveCSSRule );
	*/
	C( Parent );
	C( FirstChild );
	C( LastChild );
	C( PreviousSibling );
	C( NextSibling );
	C( CreateElement );
	C( InsertChild );
	C( AppendChild );
	C( InsertBefore );
	C( InsertAfter );
	C( AddClasses );
	C( RemoveClasses );
	C( ToggleClasses );
	C( EnableElements );
	C( DisableElements );
	C( SetProperty );
	C( GetProperty );
	C( SetAttribute );
	C( GetAttribute );
	C( RemoveAttribute );
	C( GetResult );
	C( SetValue );
	C( GetValue );
	C( Focus );
	C( Select );
	C( Dummy );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

void xdhcmn::Escape(
    const str::string_ &Source,
    str::string_ &Target,
	bso::char__ Delimiter,
	bso::char__ EscapeChar )
{
    sdr::row__ Row = Source.First();
	bso::char__ C = 0;

    while ( Row != qNIL ) {
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
		default:	// The '\x' char sequence remains this 2 chars (hex number). Used essentially for end of string marker ('\x00': NUL char).
			if ( C == EscapeChar )
				if ( Source( Source.Next( Row ) ) != 'x' )
					Target.Append( EscapeChar );

			Target.Append( Source( Row ) );
			break;
		}

        Row = Source.Next( Row );
    }
}

const str::dString &xdhcmn:: Escape(
	str::string_ &String,
	bso::char__ Delimiter,	// Should be '\'', '"' or 0. If 0, escapes '\'' and '\"', otherwise escapes 'Delimiter'.
	bso::char__ EscapeChar )
{
qRH;
	str::wString Buffer;
qRB;
	Buffer.Init();
	Escape( String, Buffer, Delimiter, EscapeChar );
	String = Buffer;
qRR;
qRT;
qRE;
	return String;
}


void xdhcmn::FlatMerge(
	const str::dStrings &Splitted,
	str::dString &Merged,
	bso::sBool AsJSArray )
{
qRH
	sdr::row__ Row = qNIL;
	str::wString Escaped;
qRB
	Row = Splitted.First();

	if ( AsJSArray )
		Merged.Append( "[ " );

	while ( Row != qNIL ) {

		Merged.Append('"');

		Escaped.Init();
		xdhcmn::Escape( Splitted( Row ), Escaped, '"' );
		Merged.Append( Escaped );

		Merged.Append('"');

		Row = Splitted.Next( Row );

		if ( Row != qNIL )
			Merged.Append( ", " );
	}

	if ( AsJSArray )
		Merged.Append( " ]" );
qRR
qRT
qRE
}

void xdhcmn::FlatSplit(
	flw::sRFlow &Flow,
	str::dStrings &Splitted )
{
qRH;
	bso::sBool Escaped = false, Quoted = false;
	bso::sChar Char = 0;
	str::wString Item;
qRB;
	Item.Init();
	
	while ( !Flow.EndOfFlow() ) {
		Char = Flow.Get();

		if ( Escaped ) {
			switch ( Char ) {
			case 'b':
				Char = 8;
				break;
			case 't':
				Char = '9';
				break;
			case 'n':
				Char = 10;
				break;
			case 'f':
				Char = 12;
				break;
			case 'r':
				Char = 13;
				break;
			case '\'':
			case '"':
			case '\\':
				// 'Char' is put as is.
				break;
			default:
				qRFwk();
				break;
			}

			Item.Append( Char );
			Escaped = false;
		} else {
			switch ( Char ) {
			case '"':
				if ( Quoted ) {
					Splitted.Append( Item );
					Item.Init();
					Quoted = false;
				} else
					Quoted = true;
				break;
			case ' ':
				if ( Quoted )
					Item.Append( ' ' );
				break;
			case ',':
				if ( Quoted )
					Item.Append( ',' );
				break;
			case '\\':
				if ( !Quoted )
					qRFwk();
				Escaped = true;
			default:
				if ( !Quoted )
					qRFwk();
				else
					Item.Append( Char );
				break;
			}
		}
	}

	if ( Quoted )
		qRFwk();
	else if ( Escaped )
		qRFwk();
	else if ( Item.Amount() != 0 )
		Splitted.Append( Item );
qRR;
qRT;
qRE;
}

void xdhcmn::FlatSplit(
	const str::dString &Merged,
	str::dStrings &Splitted )
{
qRH;
	flx::sStringIFlow Flow;
qRB;
	Flow.Init( Merged );

	FlatSplit( Flow, Splitted );
qRR;
qRT;
qRE;
}


