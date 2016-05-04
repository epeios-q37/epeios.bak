/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrpexample.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"

#include "ogzinf.h"

#include "common.h"

#include "sclmisc.h"

using namespace wrpexample;
using namespace ogzxmp;
using common::rStuff;

const char *wrpexample::dMyObject::PREFIX = WRPEXAMPLE_MYOBJECT_PREFIX;
const char *wrpexample::dMyObject::NAME = WRPEXAMPLE_MYOBJECT_NAME;

#define ARGS (\
	dMyObject_ &MyObject,\
	fblbkd::backend___ &Backend,\
	fblbkd::rRequest &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrpexample::dMyObject::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::rRequest &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( *this, Backend, Request, *(rStuff *)UP );
}

#define REPORT( message ) sclmisc::ReportAndAbort( message )

#define DEC( name )	static void exported##name ARGS

DEC( Test )
{
qRH
qRB
	sclmisc::ReportAndAbort( "TestMessage" );
qRR
qRT
qRE
}

#define D( name )	#name, (void *)exported##name

void wrpexample::dMyObject::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

