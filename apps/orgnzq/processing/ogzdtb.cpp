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

#include "ogzdtb.h"

using namespace ogzdtb;

namespace {
	ogzdta::sRow Create_(
		const str::dString &Value,
		ogzdta::sData &Data )
	{
		ogzdta::sRow Datum = qNIL;

		if ( Value.Amount() ) {
			Datum = Data.New( qNIL );
			Data.Store( Value, qNIL, Datum );
		}

		return Datum;
	}
}

ogzrcd::sFRow ogzdtb::rDatabase::NewField_(
	ogzrcd::sRow Record,
	const ogzclm::rColumnBuffer &Column )
{
	return Records.AddField( Fields.New( Columns.New( Column.Type(), Column.Number(), Create_( Column.Label(), Data ), Create_( Column.Comment(), Data ) ) ), Record );
}

namespace {
	void GetEntries_(
		const ogzfld::dField &Field,
		ogztyp::sRow Type,
		const ogzdta::sData &Data,
		ogzbsc::dData &Entries )
	{
	qRH
		ogzfld::sDRow Row = qNIL;
		ogzbsc::wDatum Entry;
	qRB
		Row = Field.First();

		while ( Row != qNIL ) {
			Entry.Init();
			Data.Recall( Field( Row ), Type, Entry );

			Entries.Append( Entry );

			Row = Field.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

void ogzdtb::rDatabase::GetEntries_(
	const ogzfld::dField &Field,
	ogzbsc::dData &Entries ) const
{
qRH
	ogzclm::sColumn Column;
qRB
	Column.Init();
	Columns.Recall( Field.Column(), Column );

	::GetEntries_( Field, Column.GetType(), Data, Entries );
qRR
qRT
qRE
}

void ogzdtb::rDatabase::GetEntries_(
	ogzfld::sRow FieldRow,
	ogzbsc::dData &Entries	) const
{
qRH
	ogzfld::wField Field;
qRB
	Field.Init();
	Fields.Recall( FieldRow, Field );

	GetEntries_( Field, Entries );
qRR
qRT
qRE
}
