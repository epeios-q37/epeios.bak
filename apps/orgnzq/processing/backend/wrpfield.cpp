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

#include "wrpfield.h"

#define REPORT( message ) sclmisc::ReportAndAbort( message )

using namespace wrpfield;

using common::rStuff;
using common::GetTypes;

const char *wrpfield::dField::PREFIX = WRPFIELD_FIELD_PREFIX;
const char *wrpfield::dField::NAME = WRPFIELD_FIELD_NAME;

#define ARGS (\
	fblbkd::backend___ &Backend,\
	dField_ &Field,\
	fblbkd::rRequest &Request,\
	rStuff &Stuff )\

typedef void (* f_manager ) ARGS;

void wrpfield::dField::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::untyped_module &Module,
	fblbkd::index__ Index,
	fblbkd::command__ Command,
	fblbkd::rRequest &Request,
	void *UP )
{
	((f_manager)Module.UPs( Command ))( Backend, *this, Request, *(rStuff *)UP );
}

#define DEC( name )	static void exported##name ARGS

DEC( Initialize )
{
	Field().Init();
}

DEC( Define )
{
	DATABASE;

	Database.GetEntries( *Request.IdIn(), Stuff.User(), Field );
}

#define D( name )	#name, (void *)exported##name

void wrpfield::dField::NOTIFY(
	fblbkd::untyped_module &Module,
	common::rStuff &Data )
{
	Module.Add( D( Initialize ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Define ),
			fblbkd::cId,		// Field id.
		fblbkd::cEnd,
		fblbkd::cEnd );

	/*
	Module.Add( D(  ),
		fblbkd::cEnd,
		fblbkd::cEnd );
	*/
}

