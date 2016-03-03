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

using namespace wrprecord;
using namespace ogzrcd;
using common::rStuff;

const char *wrprecord::vRecord::PREFIX = WRPRECORD_RECORD_PREFIX;
const char *wrprecord::vRecord::NAME = WRPRECORD_RECORD_NAME;

#define ARGS (\
	vRecord_ &Record,\
	fblbkd::backend___ &Backend,\
	fblbkd::request__ &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrprecord::vRecord::HANDLE(
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
	fRRow Row = qNIL;
qRB
	Row = Request.IdIn();

	if ( Row != *fbltyp::UndefinedId )
		qRLmt();

	Record().Init();
qRR
qRT
qRE
}



#define D( name )	#name, (void *)exported##name

void wrprecord::vRecord::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( EditRecord ),
			fblbkd::cId,	// Record to edit. If == 'qNIL', we want a new record.
		fblbkd::cEnd,
		fblbkd::cEnd );
}

