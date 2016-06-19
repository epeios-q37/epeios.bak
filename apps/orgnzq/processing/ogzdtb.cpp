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
		sURow User,
		const str::dString &Value,
		ogzmta::eTarget Target,
		const ogzmta::mMetas &Metas )
	{
		ogzmta::sRow Meta = qNIL;

		if ( Value.Amount() ) {
			Meta = Metas.New( qNIL );
			Metas.Store( User, Value, Target, Meta );
		}

		return Meta;
	}
}

sFRow ogzdtb::mDatabase::Create_(
	sURow User,
	const ogzclm::rColumnBuffer &Column,
	sRRow Record,
	qRPN ) const
{
	ogzrcd::sRow RawRecord = GetRaw_( User, Record, qRP );

	if ( RawRecord == qNIL )
		return qNIL;

	sFRow Field = Users.Add( User,
									 Fields.New( Users.Add( User,
														    Columns.New( Column.Type(),
															             Column.Number(),
																		 ::Create_( User, Column.Label(), ogzmta::tColumnLabel, Metas ),
																		 ::Create_( User, Column.Comment(), ogzmta::tColumnComment, Metas ) ) ),
												  Record ) );

	Records.Add( Field, RawRecord );

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
	sURow User,
	const ogzfld::dField &Field ) const
{
	ogztyp::sRow Type = qNIL;

	GetColumnFeatures_( GetRaw_( User, Field.GetColumn() ), &Type, NULL, NULL, NULL );

	return Type;
}


void ogzdtb::mDatabase::GetColumnFeatures_(
	sURow User,
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

		Column = GetRaw_( User, Field.Column() );

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
	sURow User,
	const ogzrcd::dFields &Fields,
	cColumnFeatures &Callback ) const
{
qRH
	ogzfld::wField Field;
	ogzfld::wRows Raws;
qRB
	Raws.Init();
	Users.GetRaws( User, Fields, Raws );

	GetColumnFeatures_( User, Raws, Callback );
qRR
qRT
qRE
}

bso::sBool ogzdtb::mDatabase::GetColumnsFeatures(
	sURow User,
	sRRow RecordRow,
	cColumnFeatures &Callback,
	qRPN ) const
{
	bso::sBool Exists = false;
qRH
	ogzrcd::wRecord Record;
	ogzrcd::sRow RawRecord = qNIL;
qRB
	RawRecord = GetRaw_( User, RecordRow, qRP );

	if ( RawRecord == qNIL )
		qRReturn;

	Exists = true;

	Record.Init();
	Records.Recall( RawRecord, Record );

	GetColumnFeatures_( User, Record, Callback );
qRR
qRT
qRE
	return Exists;
}

void ogzdtb::mDatabase::GetEntries_(
	sURow User,
	const ogzrcd::dFields &RegularFields,
	const ogzfld::dRows &RawFields,
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

		Column = GetRaw_( User, Field.Column() );

		Columns.GetTypeAndNumber( Column, Type, Number );

		Entries.Init();
		GetEntries_( User, RawFields( Row ), Entries, Type, Number );

		Callback.FieldEntries( RegularFields( Row ), Column, Type, Number, Entries );

		Row = RawFields.Next( Row );
	}
qRR
qRT
qRE
}


void ogzdtb::mDatabase::GetEntries_(
	sURow User,
	const ogzrcd::dFields &Fields,
	cFieldEntries &Callback ) const
{
qRH
	ogzfld::wField Field;
	ogzfld::wRows Raws;
qRB
	Raws.Init();
	Users.GetRaws( User, Fields, Raws );

	GetEntries_( User, Fields, Raws, Callback );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::Append_(
	sURow User,
	const ogzbsc::dDatum &Entry,
	ogztyp::sRow Type,
	ogzfld::dField &Field ) const
{
	ogzetr::sRow Row = Entries.New( Type );

	Entries.Store( Entry, Type, Row );

	Field.Add( Users.Add( User, Row ) );
}


void ogzdtb::mDatabase::Delete_(
	sURow User,
	const ogzfld::dEntries &EntriesRows ) const
{
qRH
	ogzetr::wRows RawEntriesRows;
qRB
	RawEntriesRows.Init();

	Users.GetRaws( User, EntriesRows, RawEntriesRows );

	Entries.Delete( RawEntriesRows );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::Append_(
	sURow User,
	const ogzbsc::dData &Data,
	ogzfld::dField &Field ) const
{
	ogztyp::sRow Type = GetType_( User, Field );
	sdr::sRow Row = Data.First();

	while ( Row != qNIL ) {
		Append_( User, Data( Row ), Type, Field );

		Row = Data.Next( Row );
	}
}

bso::sBool ogzdtb::mDatabase::GetEntries(
	sURow User,
	sRRow RecordRow,
	cFieldEntries &Callback,
	qRPN ) const

{
	bso::sBool Exists = false;
qRH
	ogzrcd::wRecord Record;
	ogzrcd::sRow RawRecord = qNIL;
qRB
	if ( ( RawRecord = GetRaw_( User, RecordRow, qRPU  ) ) == qNIL )
		qRReturn;

	Exists = true;

	Record.Init();
	Records.Recall( RawRecord, Record );

	GetEntries_( User, Record, Callback );
qRR
qRT
qRE
	return Exists;
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
	sURow User,
	const ogzfld::dField &Field,
	ogzbsc::dData &Entries,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number ) const
{
qRH
	ogzetr::wRows EntryRows;
qRB
	Columns.GetTypeAndNumber( GetRaw_( User, Field.GetColumn() ), Type, Number );

	EntryRows.Init();
	Users.GetRaws( User, Field, EntryRows );

	::GetEntries_( EntryRows, Type, this->Entries, Entries );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetEntries_(
	sURow User,
	ogzfld::sRow FieldRow,
	ogzbsc::dData &Entries,
	ogztyp::sRow &Type,
	ogzclm::eNumber &Number ) const
{
qRH
	ogzfld::wField Field;
qRB
	Field.Init();

	if ( !Fields.Recall( FieldRow, Field ) )
		qRGnr();

	GetEntries_( User, Field, Entries, Type, Number );
qRR
qRT
qRE
}

bso::sBool ogzdtb::mDatabase::Update(
	sURow User,
	sFRow FieldRow,
	const dData &Entries,
	qRPN ) const
{
	bso::sBool Exists = false;
qRH
	ogzfld::wField Field;
	ogzfld::sRow RawField = qNIL;
qRB
	if ( ( RawField = GetRaw_( User, FieldRow, qRP ) ) == qNIL )
		qRReturn;

	Field.Init();

	if ( !Fields.Recall( RawField, Field) )
		qRGnr();

	Remove_( RawField );

	Field.RemoveEntries();

	Append_( User, Entries, Field );

	Fields.Store( Field, RawField );

	Exists = true;
qRR
qRT
qRE
	return Exists;
}

void ogzdtb::mDatabase::GetFirstEntry_(
	sURow User,
	const ogzfld::dField &Field,
	dDatum &Entry ) const
{
qRH
	ogzclm::sColumn Column;
qRB
	if ( Field.Amount() != 0 ) {
		Column.Init();
		Columns.Recall( GetRaw_(User, Field.Column() ), Column );
		Entries.Recall( GetRaw_( User, Field(Field.First() ) ), Column.GetType(), Entry );
	}
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetFirstEntry_(
	sURow User,
	sFRow Row,
	dDatum &Entry ) const
{
qRH
	ogzfld::wField Field;
qRB
	Field.Init();

	this->Fields.Recall( GetRaw_( User, Row ), Field );

	GetFirstEntry_( User, Field, Entry );
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetFirstEntry_(
	sURow User,
	ogzrcd::sRow RecordRow,
	dDatum &Entry ) const
{
qRH
	ogzrcd::wRecord Record;
qRB
	Record.Init();

	Records.Recall( RecordRow, Record );

	if ( Record.Amount() == 0 )
		qRGnr();

	GetFirstEntry_( User, Record( Record.First() ), Entry );
qRR
qRT
qRE
}


void ogzdtb::mDatabase::GetRecords_(
	sURow User,
	const ogzusr::dRecords &Records,
	cRecordRetriever &Callback ) const
{
qRH
	wDatum Entry;
	sdr::sRow Row = qNIL;
qRB
	Row = Records.First();
	
	while ( Row != qNIL ) {
		Entry.Init();
		GetFirstEntry_( User, GetRaw_( User, Records( Row ) ), Entry );

		Callback.Retrieve( Records( Row ), Entry );

		Row = Records.Next( Row );
	}
qRR
qRT
qRE
}

void ogzdtb::mDatabase::GetRecords(
	sURow User,
	cRecordRetriever &Callback ) const
{
qRH
	ogzusr::wRecords Records;
qRB
	Records.Init();
	Users.GetRecordsSet( User, 0, ogzusr::AmountMax, Records );

	GetRecords_( User, Records, Callback );
qRR
qRT
qRE
}

sRRow ogzdtb::mDatabase::GetRecord(
	sURow User,
	sFRow Field,
	qRPN ) const
{
	ogzfld::sRow RawField = GetRaw_( User, Field, qRP );

	if ( RawField == qNIL )
		return qNIL;

	return Fields.GetRecord( RawField );
}

bso::sBool ogzdtb::mDatabase::EraseIfEmpty(
	sURow User,
	sRRow Record ) const
{
	ogzrcd::sRow RawRecord = GetRaw_( User, Record );

	if ( Records.IsEmpty( RawRecord ) ) {
		Records.Erase( GetRaw_( User, Record ) );
		return true;
	} else
		return false;
}


sFRow ogzdtb::mDatabase::Create(
	sURow User,
	const ogzclm::rColumnBuffer &Column,
	sRRow Record,
	const dData &Entries,
	qRPN ) const
{
	sFRow Field = Create_( User, Column, Record, qRP );

	if ( Field != qNIL )
		Update( User, Field, Entries );

	return Field;
}

void ogzdtb::mDatabase::Remove_( ogzfld::sRow FieldRow ) const
{
qRH
	ogzfld::wField Field;
qRB
	Field.Init();

	if ( !Fields.Recall( FieldRow, Field) )
		qRGnr();

	Field.RemoveEntries();

	Fields.Store( Field, FieldRow );
qRR
qRT
qRE
}

