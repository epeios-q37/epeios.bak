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
	ogzmta::sRow Create_(
		const str::dString &Value,
		ogzmta::eTarget Target,
		ogzmta::mMetas &Metas )
	{
		ogzmta::sRow Meta = qNIL;

		if ( Value.Amount() ) {
			Meta = Metas.New( qNIL );
			Metas.Store( Value, Target, Meta );
		}

		return Meta;
	}
}

ogzbsc::sFRow ogzdtb::mDatabase::NewField(
	const ogzclm::rColumnBuffer &Column,
	ogzbsc::sRRow Record,
	ogzusr::sRow User )
{
	ogzbsc::sFRow Field = Users.Add( Fields.New( Users.Add( Columns.New( Column.Type(), Column.Number(), Create_( Column.Label(), ogzmta::tColumnLabel, Metas ), Create_( Column.Comment(), ogzmta::tColumnComment, Metas ) ), User ) ), User );

	Records.AddField( Field, GetRawRecordRow_( Record, User ) );

	return Field;
}

void ogzdtb::mDatabase::GetColumnFeatures_(
	ogzclm::sRow ColumnRow,
	ogztyp::sRow *Type,
	ogzclm::eNumber *Number,
	str::dString *Label,
	str::dString *Comment ) const
{
qRH
	ogzclm::sColumn Column;
qRB
	Column.Init();
	Columns.Recall( ColumnRow, Column );

	if ( Type != NULL )
		*Type = Column.Type();

	if ( Number != NULL )
		*Number = Column.Number();

	if ( Label != NULL )
		GetMeta_( Column.Label(), *Label );

	if ( Comment != NULL )
		GetMeta_( Column.Comment(), *Comment );
qRR
qRT
qRE
}

ogztyp::sRow ogzdtb::mDatabase::GetType_(
	const ogzfld::dField &Field,
	ogzusr::sRow User) const
{
	ogztyp::sRow Type = qNIL;

	GetColumnFeatures_( GetRawColumnRow_( Field.GetColumn(), User ), &Type, NULL, NULL, NULL );

	return Type;
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

		tol::Init( Label, Comment );

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

void ogzdtb::mDatabase::Append_(
	const ogzbsc::dDatum &Entry,
	ogztyp::sRow Type,
	ogzusr::sRow User,
	ogzfld::dField &Field )
{
	ogzetr::sRow Row = Entries.New( Type );

	Entries.Store( Entry, Type, Row );

	Field.Add( Users.Add( Row, User ) );
}


void ogzdtb::mDatabase::Delete_(
	const ogzfld::dEntries &EntriesRows,
	ogzusr::sRow User )
{
qRH
	ogzetr::wRows RawEntriesRows;
qRB
	RawEntriesRows.Init();

	Users.GetRaws( EntriesRows, User, RawEntriesRows );

	Entries.Delete( RawEntriesRows );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::Append_(
	const ogzbsc::dData &Data,
	ogzusr::sRow User,
	ogzfld::dField &Field )
{
	ogztyp::sRow Type = GetType_( Field, User );
	sdr::sRow Row = Data.First();

	while ( Row != qNIL ) {
		Append_( Data( Row ), Type, User, Field );

		Row = Data.Next( Row );
	}
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
		const ogzetr::dRows &EntryRows,
		ogztyp::sRow Type,
		const ogzetr::mEntries &EntriesStorage,
		ogzbsc::dData &Entries )
	{
	qRH
		sdr::sRow Row = qNIL;
		ogzbsc::wDatum Entry;
	qRB
		Row = EntryRows.First();

		while ( Row != qNIL ) {
			Entry.Init();
			EntriesStorage.Recall( EntryRows( Row ), Type, Entry );

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
	ogzetr::wRows EntryRows;
qRB
	Columns.GetTypeAndNumber( GetRawColumnRow_( Field.GetColumn(), User ), Type, Number );

	EntryRows.Init();
	Users.GetRaws( Field, User, EntryRows );

	::GetEntries_( EntryRows, Type, this->Entries, Entries );
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

bso::sBool ogzdtb::mDatabase::UpdateField(
	ogzbsc::sFRow FieldRow,
	ogzusr::sRow User,
	ogzbsc::dData &Entries,
	qRPF )
{
	bso::sBool Exists = false;
qRH
	ogzfld::wField Field;
	ogzfld::sRow RawFieldRow = qNIL;
qRB
	RawFieldRow = GetRawFieldRow_( FieldRow, User );

	Field.Init();

	if ( !Fields.Recall(RawFieldRow, Field) ) {
		if ( qRPT )
			qRGnr();
		else
			qRReturn;
	}

	Delete_( Field, User );

	Field.RemoveEntries();

	Append_( Entries, User, Field );

	Fields.Store( Field, RawFieldRow );

	Exists = true;
qRR
qRT
qRE
	return Exists;
}

