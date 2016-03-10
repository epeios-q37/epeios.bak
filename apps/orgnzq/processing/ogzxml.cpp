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

#include "ogzxml.h"

#include "stsfsm.h"

using namespace ogzxml;

using xml::dWriter;

namespace {
	stsfsm::wAutomat RecordTagAutomats_[RecordLastVersion+1];
	stsfsm::wAutomat RecordAttributeAutomats_[RecordLastVersion+1];
}

const char *ogzxml::GetLabel(
	eRecordTag Tag,
	sVersion Version )
{
	switch ( Tag ) {
	case rtRecord:
		return "Record";
		break;
	case rtFields:
		return "Fields";
		break;
	case rtField:
		return "Field";
		break;
	case rtData:
		return "Data";
		break;
	case rtDatum:
		return "Datum";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avod a warning;
}

const char *ogzxml::GetLabel(
	eRecordAttribute Attribute,
	sVersion Version )
{
	switch ( Attribute ) {
	case raId:
		return "Id";
		break;
	case raAmount:
		return "Amount";
		break;
	case raType:
		return "Type";
		break;
	case raNumber:
		return "Number";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avod a warning;
}

#define T( tag )	GetLabel( rt##tag )
#define A( tag )	GetLabel( ra##tag )

namespace {
	inline void Dump_(
		ogztyp::sRow TypeRow,
		const ogztyp::sType &Type,
		const ogzdta::fData &Data,
		ogzdta::sDRow DatumRow,
		dWriter &Writer )
	{
	qRH
		ogzdta::wDatum Datum;
		ogzdta::sSize Size = 0;
		str::string XML;
	qRB
		XML.Init();

		Writer.PushTag( T( Datum ) );

		Datum.Init();
		Data.Recall( DatumRow, TypeRow, Datum );

		Writer.PutValue( Type.ToXML( Datum, XML ) );

		Writer.PopTag();
	qRR
	qRT
	qRE
	}

	inline void Dump_(
		ogztyp::sRow TypeRow,
		const ogztyp::sType &Type,
		const ogzdta::fData &Data,
		const ogzdta::dDatumList &DatumList,
		dWriter &Writer )
	{
		sdr::sRow Row = DatumList.First();

		Writer.PushTag( T( Data ) );
		xml::PutAttribute( A(Amount), DatumList.Amount(), Writer );

		while ( Row != qNIL ) {
			Dump_( TypeRow, Type, Data, DatumList( Row ), Writer );

			Row = DatumList.Next( Row );
		}

		Writer.PopTag();
	}

	inline void Dump_(
		const ogztyp::dTypes &Types,
		const ogzclm::fColumns &Columns,
		const ogzdta::fData &Data,
		const ogzfld::dField &Field,
		dWriter &Writer )
	{
	qRH
		ogzclm::sColumn Column;
	qRB
		Column.Init();
		
		Columns.Recall( Field.Column(), Column );

		Writer.PushTag ( T( Field ) );

		xml::PutAttribute( A( Type ), *Column.Type(), Writer );
		Writer.PutAttribute( A( Number ), ogzclm::GetLabel( Column.Number() ) );

		Dump_( Column.Type(), Types( Column.Type() ), Data, Field, Writer );

		Writer.PopTag();
	qRR
	qRT
	qRE
	}

	inline void Dump_(
		const ogztyp::dTypes &Types,
		const ogzfld::fFields &Fields,
		ogzrcd::sFRow Row,
		const ogzclm::fColumns &Columns,
		const ogzdta::fData &Data,
		dWriter &Writer )
	{
	qRH
		ogzfld::wField Field;
	qRB
		Field.Init();
		Fields.Recall( Row, Field );

		Dump_( Types, Columns, Data, Field, Writer );
	qRR
	qRT
	qRE
	}

	inline void Dump_(
		const ogztyp::dTypes &Types,
		const ogzfld::fFields &Fields,
		const ogzfld::dFieldList &FieldList,
		const ogzclm::fColumns &Columns,
		const ogzdta::fData &Data,
		dWriter &Writer )
	{
		sdr::sRow Row = FieldList.First();

		Writer.PushTag( T( Fields ) );

		xml::PutAttribute( A( Amount ), FieldList.Amount(), Writer );

		while ( Row != qNIL ) {
			Dump_( Types, Fields, FieldList( Row ), Columns, Data, Writer );

			Row = FieldList.Next( Row );
		}

		Writer.PopTag();
	}

	inline void Dump_(
		const ogzrcd::dRecord &Record,
		const ogzdtb::rDatabase &Database,
		xml::dWriter &Writer )
	{
		Writer.PushTag( T( Record ) );
		Dump_( Database.Types, Database.Fields, Record, Database.Columns, Database.Data, Writer );
		Writer.PopTag();
	}

	inline void Dump_(
		ogzrcd::sRRow RecordRow,
		const ogzrcd::fRecords &Records,
		const ogzdtb::rDatabase &Database,
		xml::dWriter &Writer )
	{
	qRH
		ogzrcd::wRecord Record;
	qRB
		Record.Init();
		
		Records.Recall( RecordRow, Record );

		Dump_( Record, Database, Writer );
	qRR
	qRT
	qRE
	}
}

void ogzxml::Dump(
	ogzrcd::sRRow RecordRow,
	const ogzdtb::rDatabase &Database,
	dWriter &Writer )
{
	Dump_( RecordRow, Database.Records, Database, Writer );
}

void ogzxml::Dump(
	const ogztyp::dTypes &Types,
	const ogzrcd::rRecordBuffer &Record,
	xml::dWriter &Writer )
{
	Writer.PushTag( T( Record ) );

	if ( Record.Id() != qNIL )
		xml::PutAttribute( A( Id ), *Record.Id(), Writer );

	Dump_( Types, Record.Fields, Record, Record.Columns, Record.Data, Writer );

	Writer.PopTag();
}

namespace {
		template <typename type> inline void InitAndFillAutomat_(
		stsfsm::dAutomat &Automat,
		sVersion Version,
		type Amount )
	{
		Automat.Init();
		stsfsm::Fill<type, sVersion>( Version, Automat, Amount, GetLabel );
	}

	template <typename type>  void InitAndFillAutomats_(
		stsfsm::dAutomat *Automats,
		sVersion LastVersion,
		type Amount )
	{
		for ( int i = 0; i <= RecordLastVersion; i++ )
			InitAndFillAutomat_(Automats[i], i, Amount );
	}

	void InitAndFillRecordAutomats_( void )
	{
		InitAndFillAutomats_( RecordTagAutomats_, RecordLastVersion, rt_amount );
		InitAndFillAutomats_( RecordAttributeAutomats_, RecordLastVersion, ra_amount );
	}

	void InitAndFillAutomats_( void )
	{
		InitAndFillRecordAutomats_();
	}
}

qGCTOR( ogzxml )
{
	InitAndFillAutomats_();
}



