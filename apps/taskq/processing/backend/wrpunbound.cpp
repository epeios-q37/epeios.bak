/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrpunbound.h"

#include "taskqbkd.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"

#include "sclm.h"

using namespace wrpunbound;

#define M( message )	qCDEF( char *, message##_, #message )

namespace {
	M( TestMessage );
}

#undef M

using common::rStuff;

#define DEC( name, version )\
	static inline void unbound_##name##_##version(\
		fblbkd::rBackend &BaseBackend,\
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

#define D( name, version )	TSKINF_UC_SHORT #name "_" #version, ::unbound_##name##_##version

void wrpunbound::Inform( fblbkd::backend___ &Backend )
{
	Backend.Add( D( Test, 1 ),
		fblbkd::cEnd,
		fblbkd::cEnd );
}

