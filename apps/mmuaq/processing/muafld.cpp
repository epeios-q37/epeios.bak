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

#include "muafld.h"

using namespace muafld;

sRow muafld::dFoldersTree::Search(
	sRow Folder,
	const str::dString &Name ) const
{
	sRow Row = FirstChild( Folder );

	while ( ( Row != qNIL ) && ( Folders_( Row ).Name != Name ) )
		Row = NextSibling( Row );

	return Row;
}

bso::sBool muafld::dFoldersTree::IsDescendant(
	sRow Folder,
	sRow Ascendant ) const
{
	while ( ( Folder != qNIL ) && ( Folder != Ascendant ) )
		Folder = Parent( Folder );

	return Folder != qNIL;
}

const dRows &muafld::dFoldersTree::GetChildren(
	sRow Row,
	dRows &Rows ) const
{
	if ( Row == qNIL )
		Rows.Append( S_.Root );
	else {
		Row = FirstChild( Row );

		while ( Row != qNIL ) {
			Rows.Add( Row );

			Row = NextSibling( Row );
		}
	}

	return Rows;
}

void muafld::dFoldersTree::GetNames(
	const muafld::dRows &Folders,
	str::dStrings &Names ) const
{
	sdr::sRow Row = Folders.First(), NRow = qNIL;

	while ( Row != qNIL ) {
		NRow = Names.New();
		Names( NRow ).Init();

		Names( NRow ) = GetName( Folders( Row ), Names( NRow ) );

		Row = Folders.Next( Row);
	}
}






