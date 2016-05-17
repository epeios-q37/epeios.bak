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
		ogzdta::mData &Data )
	{
		ogzdta::sRow Datum = qNIL;

		if ( Value.Amount() ) {
			Datum = Data.New( qNIL );
			Data.Store( Value, qNIL, Datum );
		}

		return Datum;
	}
}

ogzbsc::sFRow ogzdtb::mDatabase::NewField(
	const ogzclm::rColumnBuffer &Column,
	ogzbsc::sRRow Record,
	ogzusr::sRow User )
{
	return Users.Add( Fields.New( Columns.New( Column.Type(), Column.Number(), Create_( Column.Label(), Data ), Create_( Column.Comment(), Data ) ) ), Record, User );
}

void ogzdtb::mDatabase::GetColumnFeatures_(
	ogzclm::sRow ColumnRow,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number,
	str::dString &Label,
	str::dString &Comment ) const
{
qRH
	ogzclm::sColumn Column;
qRB
	Column.Init();
	Columns.Recall( ColumnRow, Column );

	Type = Column.Type();
	Number = Column.Number();
	GetDatum_( Column.Label(), qNIL, Label );
	GetDatum_( Column.Comment(), qNIL, Comment );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetColumnFeatures_(
	const ogzfld::dRows &RawFields,
	cColumnFeatures &Callback ) const
{
qRH
	sdr::sRow Row = qNIL;
	ogzfld::wField Field;
	ogzclm::sRow Column = qNIL;
	ogztyp::sRow Type = qNIL;
	ogzclm::eNumber Number = ogzclm::n_Undefined;
	str::wString Label, Comment;
qRB
	Row = RawFields.First();

	while ( Row != qNIL ) {
		Field.Init();
		Fields.Recall( RawFields( Row ), Field );

		Column = Field.Column();

		Label.Init();
		Comment.Init();

		GetColumnFeatures_( Column, Type, Number, Label, Comment );

		Callback.ColumnFeatures( Column, Type, Number, Label, Comment );

		Row = RawFields.Next( Row );
	}
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetColumnFeatures_(
	const ogzrcd::dFields &Fields,
	ogzusr::sRow User,
	cColumnFeatures &Callback ) const
{
qRH
	ogzfld::wField Field;
	ogzfld::wRows Raws;
qRB
	Raws.Init();
	Users.GetRaws( Fields, User, Raws );

	GetColumnFeatures_( Raws, Callback );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetColumnFeatures(
	ogzbsc::sRRow RecordRow,
	ogzusr::sRow User,
	cColumnFeatures &Callback ) const
{
qRH
	ogzrcd::wRecord Record;
qRB
	Record.Init();
	Records.Recall( Users.GetRaw( RecordRow, User ), Record );

	GetColumnFeatures_( Record, User, Callback );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetEntries_(
	const ogzrcd::dFields &RegularFields,
	const ogzfld::dRows &RawFields,
	ogzusr::sRow User,
	cFieldEntries &Callback ) const
{
qRH
	sdr::sRow Row = qNIL;
	ogzfld::wField Field;
	ogzclm::sRow Column = qNIL;
	ogztyp::sRow Type = qNIL;
	str::wStrings Entries;
qRB
	Row = RawFields.First();

	while ( Row != qNIL ) {
		Field.Init();
		Fields.Recall( RawFields( Row ), Field );

		Column = Field.Column();

		Type = Columns.GetType( Column );

		Entries.Init();
		GetEntries_( RawFields( Row ), User, Entries );

		Callback.FieldEntries(RegularFields( Row ), Column, Type, Entries );

		Row = RawFields.Next( Row );
	}
qRR
qRT
qRE
}


void ogzdtb::mDatabase::GetEntries_(
	const ogzrcd::dFields &Fields,
	ogzusr::sRow User,
	cFieldEntries &Callback ) const
{
qRH
	ogzfld::wField Field;
	ogzfld::wRows Raws;
qRB
	Raws.Init();
	Users.GetRaws( Fields, User, Raws );

	GetEntries_( Fields, Raws, User, Callback );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetEntries(
	ogzbsc::sRRow RecordRow,
	ogzusr::sRow User,
	cFieldEntries &Callback	) const

{
qRH
	ogzrcd::wRecord Record;
qRB
	Record.Init();
	Records.Recall( Users.GetRaw( RecordRow, User ), Record );

	GetEntries_( Record, User, Callback );
qRR
qRT
qRE
}

namespace {
	void GetEntries_(
		const ogzdta::dRows &EntryRows,
		ogztyp::sRow Type,
		const ogzdta::mData &Data,
		ogzbsc::dData &Entries )
	{
	qRH
		sdr::sRow Row = qNIL;
		ogzbsc::wDatum Entry;
	qRB
		Row = EntryRows.First();

		while ( Row != qNIL ) {
			Entry.Init();
			Data.Recall( EntryRows( Row ), Type, Entry );

			Entries.Append( Entry );

			Row = EntryRows.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

ogztyp::sRow ogzdtb::mDatabase::GetEntries_(
	const ogzfld::dField &Field,
	ogzusr::sRow User,
	ogzbsc::dData &Entries ) const
{
	ogztyp::sRow Type = qNIL;
qRH
	ogzdta::wRows EntryRows;
qRB
	Type = Columns.GetType( Field.GetColumn() );

	EntryRows.Init();
	Users.GetRaws( Field, User, EntryRows );

	::GetEntries_( EntryRows, Type, Data, Entries );
qRR
qRT
qRE
	return Type;
}

ogztyp::sRow ogzdtb::mDatabase::GetEntries_(
	ogzfld::sRow FieldRow,
	ogzusr::sRow User,
	ogzbsc::dData &Entries	) const
{
	ogztyp::sRow Type = qNIL;
qRH
	ogzfld::wField Field;
qRB
	Field.Init();
	Fields.Recall( FieldRow, Field );

	Type = GetEntries_( Field, User, Entries );
qRR
qRT
qRE
	return Type;
}
