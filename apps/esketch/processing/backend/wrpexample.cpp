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

#include "sktinf.h"

#include "common.h"

#include "sclmisc.h"

using namespace wrpexample;
using namespace sktxmp;
using common::rStuff;

const char *wrpexample::vMyObject::PREFIX = WRPEXAMPLE_MYOBJECT_PREFIX;
const char *wrpexample::vMyObject::NAME = WRPEXAMPLE_MYOBJECT_NAME;

#define ARGS (\
	vMyObject_ &MyObject,\
	fblbkd::backend___ &Backend,\
	fblbkd::request__ &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrpexample::vMyObject::HANDLE(
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

DEC( Test )
{
qRH
qRB
	sclmisc::ReportAndAbort( "TestMessage" );
qRR
qRT
qRE
}

DEC( ToUC )
{
qRH
	str::string String;
qRB
	String.Init(Request.StringIn() );

	str::ToUpper( String );

	Request.StringOut() = String;
qRR
qRT
qRE
}

#define D( name )	#name, (void *)exported##name

void wrpexample::vMyObject::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( ToUC ),
			fblbkd::cString,
		fblbkd::cEnd,
			fblbkd::cString,
		fblbkd::cEnd );

	Module.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

