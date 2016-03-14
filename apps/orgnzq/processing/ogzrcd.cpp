/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogzrcd.h"

using namespace ogzrcd;

ogzfld::sRow ogzrcd::rRecordBuffer::CreateField( ogzclm::sRow Column )
{
	ogzfld::sRow Row = qNIL;
qRH
	ogzfld::wField Field;
qRB
	Field.Init( Column );

	Row = Fields.New();

	Fields.Store( Field, Row );

	Append( Row );
qRR
qRT
qRE
	return Row;
}

ogzclm::sRow ogzrcd::rRecordBuffer::CreateColumn(
	ogztyp::sRow Type,
	ogzclm::eNumber Number )
{
	ogzclm::sRow Row = qNIL;	
	ogzclm::sColumn Column;

	Column.Init( Type, Number );

	Row = Columns.New();

	Columns.Store( Column, Row );

	return Row;
}

ogzfld::sRow ogzrcd::rRecordBuffer::CreateField(
	ogztyp::sRow Type,
	ogzclm::eNumber Number )
{
	return CreateField(CreateColumn( Type, Number ) );
}
