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

const char *wrprecord::dRecord::PREFIX = WRPRECORD_RECORD_PREFIX;
const char *wrprecord::dRecord::NAME = WRPRECORD_RECORD_NAME;

#define ARGS (\
	dRecord_ &Record,\
	fblbkd::backend___ &Backend,\
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
	((f_manager)Module.UPs( Command ))( *this, Backend, Request, *(rStuff *)UP );
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
		if ( !Database.Types.Exists( Type ) )
			REPORT( "UnknowFieldType");

		if ( !ogzclm::Exists( Number ) )
			REPORT( "UnknownFieldNumber" );

		if ( Record.CreateColumn( Type, Number ) == qNIL )
			qRFwk();
	}
}

# define RORD common::rRORack RORack

# define ROR\
	RORack.Init( common::Rack );\
	const common::rNakedRack &Rack = RORack();
	

DEC( CreateField )
{
	RORD;
	ROR;

	const fbltyp::sId &Type = Request.IdIn();
	const fbltyp::sId8 &Number = Request.Id8In();

	CreateField_( *Type, *Number, Rack.Database.Core, Record() );
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
		fblbkd::cEnd,
		fblbkd::cEnd );
}

