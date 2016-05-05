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

namespace {
	inline void GetDatum_(
		ogzdta::sRow Row,
		ogztyp::sRow TextType,
		const ogzdta::sData &Data,
		str::dString &Datum )
	{
		if ( Row != qNIL )
			Data.Recall( Row, TextType, Datum );
	}
}

#if 0
void ogzrcd::rRecordBuffer::GetColumnContent(
	ogzfld::sLRow Row,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number,
	str::dString &Label,
	str::dString &Comment ) const
{
qRH
	ogzclm::sColumn Column;
qRB
	Column.Init();
	GetColumn( Row, Column );

	Type = Column.Type();
	Number = Column.Number();

	GetDatum_( Column.Label(), Columns_.TextType(), Data_, Label );
	GetDatum_( Column.Comment(), Columns_.TextType(), Data_, Comment );
qRR
qRT
qRE
}
# endif
