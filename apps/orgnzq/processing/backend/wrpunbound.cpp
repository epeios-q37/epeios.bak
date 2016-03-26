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
#include "sclmisc.h"

using namespace wrpunbound;

using common::rStuff;

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::request__ &Request,\
		bso::bool__ &,\
		void *UP )

namespace {
	void GetTypes_(
		const ogztyp::dTypes &Types,
		fbltyp::strings_ &Labels,
		fbltyp::ids_ &Ids )
	{
		ogztyp::sRow Row = Types.First();

		while ( Row != qNIL ) {
			Labels.Add( str::wString( Types( Row ).GetLabel() ) );
			Ids.Add( *Row );

			Row = Types.Next( Row );
		}

	}
}

# define STUFF common::rStuff &Stuff = *(common::rStuff *)UP
# define BACKEND	STUFF;common::rBackend &Backend = Stuff.Backend()

DEC( Login )
{
qRH
	RWL;
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	ogzusr::sRow UserRow = qNIL;
qRB
	STUFF;

	RWR;

	const str::dString &Username = Request.StringIn();
	const str::dString &Password = Request.StringIn();

	UserRow = Rack.Authentication.Login( Username, Password );

	if ( UserRow != qNIL )
		Stuff.SetUser( UserRow );

	Request.BooleanOut() = UserRow != qNIL;
qRR 
qRT
qRE
}

namespace {
	const str::dString &GetWording_( ogzclm::eNumber Number )
	{
		static str::wString Wording;
	qRH
		str::wString RawWording;
	qRB
		RawWording.Init();
		RawWording.Append( ogzclm::GetLabel( Number ) );
		RawWording.Append( "NumberWording" );

		Wording.Init();
		sclmisc::GetBaseTranslation( RawWording, Wording );
	qRR
	qRT
	qRE
		return Wording;
	}

	void GetNumbers_(
		fbltyp::dId8s &Ids,
		fbltyp::dStrings &Labels,
		fbltyp::dStrings &Wordings )
	{
		int Number = 0;

		while ( Number < ogzclm::n_amount )
		{
			Ids.Append( Number );
			Labels.Append( str::wString( ogzclm::GetLabel( (ogzclm::eNumber)Number ) ) );
			Wordings.Append( GetWording_( (ogzclm::eNumber)Number ) );

			Number++;
		}
	}
}

DEC( GetNumbers )
{
qRH
	fbltyp::wId8s Ids;
	fbltyp::wStrings Labels, Wordings;
qRB
	Ids.Init();
	Labels.Init();
	Wordings.Init();

	GetNumbers_( Ids, Labels, Wordings );

	Request.Id8sOut() = Ids;
	Request.StringsOut() = Labels;
	Request.StringsOut() = Wordings;
qRR 
qRT
qRE
}

DEC( GetTypes )
{
qRH
	ROL;
	fbltyp::ids Ids;
	fbltyp::strings Labels;
qRB
	ROR;

	Labels.Init();
	Ids.Init();
	GetTypes_( common::GetTypes(), Labels, Ids );

	Request.IdsOut() = Ids;
	Request.StringsOut() = Labels;
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

	Backend.Add( D( GetNumbers ),
		fblbkd::cEnd,
			fblbkd::cId8s,		// Ids.
			fblbkd::cStrings,	// Labels,
			fblbkd::cStrings,	// Wordings,
		fblbkd::cEnd );

	Backend.Add( D( GetTypes ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids.
			fblbkd::cStrings,	// Labels,
		fblbkd::cEnd );
}

