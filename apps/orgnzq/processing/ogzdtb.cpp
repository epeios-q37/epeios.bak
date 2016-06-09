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
	ogzbsc::sFRow Field = Users.Add( Fields.New( Users.Add( Columns.New( Column.Type(), Column.Number(), Create_( Column.Label(), Data ), Create_( Column.Comment(), Data ) ), User ) ), User );

	Records.AddField( Field, GetRawRecordRow_( Record, User ) );

	return Field;
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
	ogzusr::sRow User,
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

		Column = GetRawColumnRow_( Field.Column(), User );

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

	GetColumnFeatures_( Raws, User, Callback );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetColumnsFeatures(
	ogzbsc::sRRow RecordRow,
	ogzusr::sRow User,
	cColumnFeatures &Callback ) const
{
qRH
	ogzrcd::wRecord Record;
qRB
	Record.Init();
	Records.Recall( GetRawRecordRow_( RecordRow, User ), Record );

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
	ogzclm::eNumber Number = ogzclm::n_Undefined;
	str::wStrings Entries;
qRB
	Row = RawFields.First();

	while ( Row != qNIL ) {
		Field.Init();
		Fields.Recall( RawFields( Row ), Field );

		Column = GetRawColumnRow_( Field.Column(), User );

		Columns.GetTypeAndNumber( Column, Type, Number );

		Entries.Init();
		GetEntries_( RawFields( Row ), User, Entries, Type, Number );

		Callback.FieldEntries( RegularFields( Row ), Column, Type, Number, Entries );

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
	Records.Recall( GetRawRecordRow_( RecordRow, User ), Record );

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

void ogzdtb::mDatabase::GetEntries_(
	const ogzfld::dField &Field,
	ogzusr::sRow User,
	ogzbsc::dData &Entries,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number ) const
{
qRH
	ogzdta::wRows EntryRows;
qRB
	Columns.GetTypeAndNumber( GetRawColumnRow_( Field.GetColumn(), User ), Type, Number );

	EntryRows.Init();
	Users.GetRaws( Field, User, EntryRows );

	::GetEntries_( EntryRows, Type, Data, Entries );
qRR
qRT
qRE
}

bso::sBool ogzdtb::mDatabase::GetEntries_(
	ogzfld::sRow FieldRow,
	ogzusr::sRow User,
	ogzbsc::dData &Entries,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number,
	qRPF ) const
{
	bso::sBool Exists = false;
qRH
	ogzfld::wField Field;
qRB
	Field.Init();
	Exists = Fields.Recall( FieldRow, Field );

	if ( !Exists ) {
		if ( qRPT )
			qRFwk();
		else
			qRReturn;
	}

	GetEntries_( Field, User, Entries, Type, Number );
qRR
qRT
qRE
	return Exists;
}
