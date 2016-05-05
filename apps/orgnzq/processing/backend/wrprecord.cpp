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

#include "wrpcolumn.h"

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
	fblbkd::backend___ &Backend,\
	fblbkd::rRequest &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrprecord::dRecord::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::rRequest &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( *this, Backend, Request, *(rStuff *)UP );
}

#define DEC( name )	static void exported##name ARGS

DEC( Initialize )
{
	Record().Init(common::GetMandatoryTextType() );
}

DEC( Define )
{
qRH
	ogzrcd::sRow Row = qNIL;
qRB
	Row = *Request.IdIn();

	if ( Row != qNIL )
		qRLmt();

	Record().Init( common::GetMandatoryTextType() );
qRR
qRT
qRE
}

namespace {
	void GetColumns_(
		const ogzrcd::rRecordBuffer &Record,
		fbltyp::dIds &Ids,
		fbltyp::dIds &Types,
		fbltyp::dId8s &Numbers,
		fbltyp::dStrings &Labels,
		fbltyp::dStrings &Comments )
	{
	qRH
		ogzclm::sLRow Row = qNIL;
		ogztyp::sRow Type = qNIL;
		ogzclm::eNumber Number = ogzclm::n_Undefined;
		str::wString Label, Comment;
		ogzclm::wRows Rows;
	qRB
		Rows.Init();
		Record.Fields().Columns().Core().GetList( 0, 0, Rows );

		Row = Rows.First();

		while ( Row != qNIL ) {
			Label.Init();
			Comment.Init();

			Record.Fields().Columns().GetFeatures( Rows( Row ), Type, Number, Label, Comment );

			Ids.Append( *Row );
			Types.Append( *Type );
			Numbers.Append( Number );
			Labels.Append( Label );
			Comments.Append( Comment );

			Row = Rows.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

namespace {
	void GetFields_(
		const ogzrcd::rRecordBuffer &Record,
		fbltyp::dIds &Ids,
		fbltyp::dIds &Columns )
	{
		ogzfld::sLRow Row = Record.First();
		ogzclm::sRow Column = qNIL;

		while ( Row != qNIL ) {
			Ids.Append( *Record( Row ) );

			Record.GetFieldFeatures( Row, Column );
			Columns.Append( *Column );

			Row = Record.Next( Row );
		}
	}
}

DEC( GetFields )
{
	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dIds &Columns = Request.IdsOut();

	GetFields_( Record(), Ids, Columns );
}

DEC( GetColumns )
{
	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dIds &Types = Request.IdsOut();
	fbltyp::dId8s &Numbers = Request.Id8sOut();
	fbltyp::dStrings &Labels = Request.StringsOut();
	fbltyp::dStrings &Comments = Request.StringsOut();

	GetColumns_( Record(), Ids, Types, Numbers, Labels, Comments );
}

DEC( CreateField )
{
	fbltyp::sObject Object = Request.ObjectIn();
	
	Request.IdOut() = *Record().CreateField( ( (wrpcolumn::dColumn *)Backend.Object( Object, WRPCOLUMN_COLUMN_NAME ) )->operator()() );
}

#define D( name )	#name, (void *)exported##name

void wrprecord::dRecord::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( Define ),
			fblbkd::cId,	// Record to edit. If == 'qNIL', new record.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Initialize ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( GetColumns ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the columns,
			fblbkd::cIds,		// Types.
			fblbkd::cId8s,		// Numbers.
			fblbkd::cStrings,	// Labels.
			fblbkd::cStrings,	// Comments.
		fblbkd::cEnd );

	Module.Add( D( GetFields ),
		fblbkd::cEnd,
			fblbkd::cIds,	// Ids of the fields.
			fblbkd::cIds,	// Ids the column for each fields.
		fblbkd::cEnd );

	Module.Add( D( CreateField ),
			fblbkd::cObject,	// Column buffer.
		fblbkd::cEnd,
			fblbkd::cId,		// The newly created field id.
		fblbkd::cEnd );
	/*
	Module.Add( D(  ),
		fblbkd::cEnd,
		fblbkd::cEnd );
	*/
}

