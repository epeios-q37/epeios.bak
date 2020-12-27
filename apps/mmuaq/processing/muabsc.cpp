/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "muabsc.h"

#include "fnm.h"

using namespace muabsc;

namespace get_next_imap_tag_ {
	void GetNext( str::dString &Tag )
	{
		sdr::sRow Row = Tag.Last();
		bso::sChar Char = 0;
		bso::sBool Cont = false;;

		while ( Row != qNIL ) {
			Cont = false;
			Char = Tag( Row );

			if ( !isalnum( Char ) )
				qRGnr();

			if ( tolower( Char ) != Char )
				qRGnr();

			if ( Char < '9' )
				Tag.Store( Char + 1, Row );
			else if ( Char == '9' )
				Tag.Store('a', Row );
			else if ( Char < 'z' )
				Tag.Store( Char + 1, Row );
			else if ( Char == 'z' ) {
				Tag.Store( '0', Row );
				Cont = true;
			}

			Row = Tag.Previous( Row );

			if ( Cont ) {
				if ( Row == qNIL )
					Tag.InsertAt( '1' );
			} else
				Row = qNIL;
		}
	}
}

const str::dString &muabsc::GetNextIMAPTag( str::dString &Tag )
{
	if ( Tag.Amount() == 0 )
		Tag.Append( '0' );
	else
		get_next_imap_tag_::GetNext( Tag );

	return Tag;
}



void muabsc::Dump(
	const dIndexes &Indexes,
	txf::sWFlow &Flow )
{
	sdr::sRow Row = Indexes.First();

	while ( Row != qNIL ) {
		Flow << Indexes( Row ) << txf::nl;

		Row = Indexes.Next( Row );
	}
}

