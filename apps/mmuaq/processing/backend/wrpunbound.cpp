/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrpunbound.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "muainf.h"

#include "sclmisc.h"

using namespace wrpunbound;

using common::rStuff;

#define DEC( name, version )\
	static inline void name##_##version(\
		fblbkd::backend___ &BaseBackend,\
		fblbkd::rRequest &Request )

DEC( Test, 1 )
{
qRH
qRB
	REPORT( TestMessage );
qRR
qRT
qRE
}

DEC( Login, 1 )
{
qRH
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	muaacc::sRow Account = qNIL;
	bso::sBool New = false;
qRB
	STUFFb;
	AUTHENTICATIONb;

	const str::dString &Username = Request.StringIn();
	const str::dString &Password = Request.StringIn();

	Account = Authentication.Authenticate( Username, Password );

	if ( Account != qNIL )
		Stuff.SetAccount( Account );

	Request.BooleanOut() = Account != qNIL;
qRR 
qRT
qRE
}

DEC( Test_, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;
qRR 
qRT
qRE
}


#define D( name, version )	MUAINF_UC_SHORT #name "_" #version, ::name##_##version

void wrpunbound::Inform( fblbkd::backend___ &Backend )
{
	Backend.Add( D( Test, 1 ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Backend.Add( D( Login, 1 ),
			fblbkd::cString,	// Username.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cBoolean,	// Success.
		fblbkd::cEnd );

}

