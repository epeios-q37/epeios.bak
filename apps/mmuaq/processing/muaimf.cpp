/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "muaimf.h"

using namespace muaimf;

namespace {
	qCDEF( char, CR_, '\r' ) ;
	qCDEF( char, LF_, '\n' ) ;

	typedef flw::sByte sByte_;

	inline bso::sBool IsUSACIIChar_( sByte_ Byte )
	{
		return ( Byte >= 33 ) && ( Byte <= 126 );
	}

	inline bso::sBool IsFieldNameChar_( sByte_ Byte )
	{
		return IsUSACIIChar_(Byte) && Byte != ':';
	}

	inline bso::sBool IsWSP_( sByte_ Byte )
	{
		return ( Byte == ' ' ) || ( Byte == '\t' );
	}

	// If returns 'false', there is no more header.
	bso::sBool GetField_(
		flw::sIFlow &Flow,
		str::dString &Name,
		str::dString &Body )
	{
		bso::sBool Continue = true;
		bso::sBool BodyPart = false;
		bso::sBool TestFolding = false;
		sByte_ Byte;

		if ( Flow.EndOfFlow() )
			return false;
		else if ( Flow.View() == CR_ ) {
			Flow.Skip();

			if ( !Flow.EndOfFlow() )
				if ( Flow.View() == LF_ )
					Flow.Skip();

			return false;
		} else if ( Flow.View() == LF_ ) {
			Flow.Skip();
			return false;
		}

		while ( Continue ) {
			Byte = Flow.Get();

			if ( Byte == CR_ ) {
				if ( Flow.EndOfFlow() )
					Continue = false;
				else
					if ( Flow.View() == LF_ ) {
						Flow.Skip();
						TestFolding = true;
					}
			} else if ( Byte == LF_ ) {
				TestFolding = true;
			} else if ( Byte == ':' ) {
				if ( BodyPart )
					Body.Append( Byte );
				else
					BodyPart = true;
			} else {
				if ( BodyPart )
					Body.Append( Byte );
				else
					Name.Append( Byte );
			}

			if ( TestFolding )
				if ( Flow.EndOfFlow() )
					Continue = false;
				else if ( IsWSP_(Flow.View() ) )
					TestFolding = false;
				else
					Continue = false;

		}

		return true;
	}
}

void muaimf::Fill(
	flw::sIFlow &Flow,
	dFields &Fields )
{
qRH
	str::wString Name, Body;
qRB
	Name.Init();
	Body.Init();

	while ( GetField_( Flow, Name, Body ) ) {
		Fields.Append( Name, Body );

		Name.Init();
		Body.Init();
	}
qRR
qRT
qRE
}

void muaimf::Fill(
	fdr::rIDriver &Driver,
	dFields &Fields )
{
qRH
	flw::sDressedIFlow<> Flow;
qRB
	Flow.Init( Driver );

	Fill( Flow, Fields );
qRR
qRT
qRE
}



void muaimf::Dump(
	const dFields &Fields,
	txf::rOFlow Flow )
{
qRH
	sFRow Row = qNIL;
	str::wString Entry;
qRB
	Row = Fields.First();

	while ( Row != qNIL ) {
		Entry.Init();
		Flow << Fields.GetName( Row, Entry ) << ": ";

		Entry.Init();
		Flow << Fields.GetBody( Row, Entry ) << txf::nl;

		Row = Fields.Next( Row );
	}
qRR
qRT
qRE
}

void muaimf::Dump(
	const dFields &Fields,
	fdr::rODriver &Driver )
{
qRH
	txf::rOFlow Flow;
qRB
	Flow.Init( Driver );

	Dump( Fields, Flow );
qRR
qRT
qRE
}






