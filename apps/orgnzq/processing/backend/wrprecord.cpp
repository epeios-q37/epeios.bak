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

#include "wrprecord.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"

#include "ogzinf.h"

#include "common.h"

#include "sclmisc.h"

#define REPORT( message ) sclmisc::ReportAndAbort( message )

using namespace wrprecord;
using namespace ogzrcd;
using common::rStuff;
using common::GetTypes;

const char *wrprecord::dRecord::PREFIX = WRPRECORD_RECORD_PREFIX;
const char *wrprecord::dRecord::NAME = WRPRECORD_RECORD_NAME;

#define ARGS (\
	dRecord_ &Record,\
	fblbkd::request__ &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrprecord::dRecord::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::request__ &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( *this, Request, *(rStuff *)UP );
}

#define DEC( name )	static void exported##name ARGS

DEC( EditRecord )
{
qRH
	sRRow Row = qNIL;
qRB
	Row = Request.IdIn();

	if ( Row != *fbltyp::UndefinedId )
		qRLmt();

	Record().Init();
qRR
qRT
qRE
}

namespace {
	void CreateField_(
		ogztyp::sRow Type,
		ogzclm::eNumber Number,
		const ogzdtb::rDatabase &Database,
		ogzrcd::rRecordBuffer &Record )
	{
		if ( !GetTypes().Exists( Type ) )
			REPORT( "UnknowFieldType");

		if ( !ogzclm::Exists( Number ) )
			REPORT( "UnknownFieldNumber" );

		if ( Record.CreateColumn( Type, Number ) == qNIL )
			qRFwk();
	}
}

DEC( CreateField )
{
qRH
	ROL;
qRB
	ROR;

	const fbltyp::sId &Type = Request.IdIn();
	const fbltyp::sId8 &Number = Request.Id8In();

	if ( *Number >= ogzclm::n_amount )
		qRGnr();

	CreateField_( *Type, (ogzclm::eNumber)*Number, Rack.Database.Core, Record() );
qRR
qRT
qRE
}

namespace {
	void GetData_(
		const ogzfld::dField &Field,
		ogztyp::sRow Type,
		const ogztyp::dTypes &Types,
		const ogzdta::sData &DataRep,
		fbltyp::dStrings &Data )
	{
	qRH
		ogzbsc::wDatum Datum;
		str::wString XML;
	qRB
		sdr::sRow Row = Field.First();

		while ( Row != qNIL ) {
			Datum.Init();

			DataRep.Recall( Field( Row ), Type, Datum );

			XML.Init();
			Types( Type ).ToXML( Datum, XML );

			Data.Append( XML );

			Row = Field.Next( Row );
		}
	qRR
	qRT
	qRE
	}

	void GetData_(
		sdr::sRow Row,
		const ogzrcd::rRecordBuffer &Record,
		const ogztyp::dTypes &Types,
		fbltyp::dStrings &Data )
	{
	qRH
		ogzfld::wField Field;
		ogzclm::sColumn Column;
	qRB
		Field.Init();
		Record.Fields.Recall( Record( Row ), Field );

		Column.Init();
		Record.Columns.Recall(Field.Column(), Column );

		GetData_( Field, Column.Type(), Types, Record.Data, Data );
	qRR
	qRT
	qRE
	}

	void GetDataSet_(
		const ogzrcd::rRecordBuffer &Record,
		fbltyp::dIds &Ids,
		fbltyp::dXStrings &DataSet )
	{
	qRH
		sdr::sRow Row = qNIL;
		fbltyp::wString Data;
	qRB
		Row = Record.First();

		while ( Row != qNIL ) {
			Ids.Append( *Row );

			Data.Init();
			GetData_(Row, Record, GetTypes(), Data );
			DataSet.Append( Data );

			Row = Record.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

DEC( GetFields )
{
	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dXStrings &DataSet = Request.XStringsOut();

	GetDataSet_( Record, Ids, DataSet );
}

#define D( name )	#name, (void *)exported##name

void wrprecord::dRecord::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( EditRecord ),
			fblbkd::cId,	// Record to edit. If == 'qNIL', we want a new record.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( CreateField ),
			fblbkd::cId,	// Field type.
			fblbkd::cId8,	// Field number ('ogzclm::eNumber).
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( GetFields ),
		fblbkd::cEnd,
			fblbkd::dIds,	// row of the fields,
			fblbkd::dXStrings	// Data of each field.
		fblbkd::cEnd );

	/*
	Module.Add( D(  ),
		fblbkd::cEnd,
		fblbkd::cEnd );
	*/
}

