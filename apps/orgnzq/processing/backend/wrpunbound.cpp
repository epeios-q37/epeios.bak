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

#include "wrpunbound.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "ogzinf.h"

using namespace wrpunbound;

using common::rStuff;

#define STUFF common::rStuff &Stuff = *(common::rStuff *)UP
#define BACKEND	STUFF;common::rBackend &Backend = Stuff.Backend()

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &Backend,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::request__ &Request,\
		bso::bool__ &,\
		void *UP )

namespace {
	void GetTypes_(
		const ogztyp::vTypes &Types,
		fbltyp::strings_ &Labels,
		fbltyp::id8s_ &Ids )
	{
		ogztyp::fTRow Row = Types.First();

		while ( Row != qNIL ) {
			Labels.Add( str::iString( Types( Row ).GetLabel() ) );
			Ids.Add( *Row );

			Row = Types.Next( Row );
		}

	}
}

DEC( Login )
{
qRH
	common::rw_rack___ Rack;
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	ogzusr::fURow UserRow = qNIL;
qRB
	STUFF;

	Rack.Init( common::Rack );

	const str::vString &Username = Request.StringIn();
	const str::vString &Password = Request.StringIn();

	UserRow = Rack().Authentication.Login( Username, Password );

	if ( UserRow != qNIL )
		Stuff.SetUser( UserRow );

	Request.BooleanOut() = UserRow != qNIL;
qRR 
qRT
qRE
}

DEC( GetTypes )
{
qRH
	common::ro_rack___ Rack;
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
qRB
	Rack.Init( common::Rack );

	Labels.Init();
	Ids.Init();
	GetTypes_( Rack().Types.Core, Labels, Ids );

	Request.StringsOut() = Labels;
	Request.Id8sOut() = Ids;
qRR 
qRT
qRE
}

#define D( name )	OGZINF_UC_SHORT #name, ::name

void wrpunbound::Inform(
	fblbkd::backend___ &Backend,
	rStuff &Stuff )
{
	Backend.Add( D( Login ),
			fblbkd::cString,	// Username.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cBoolean,	// Success.
		fblbkd::cEnd );

	Backend.Add( D( GetTypes ),
		fblbkd::cEnd,
			fblbkd::cStrings,	// Labels,
			fblbkd::cId8s,		// Ids.
		fblbkd::cEnd );
}

