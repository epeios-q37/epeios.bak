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

enum message__ {
	mTestMessage,

	m_amount,
	m_Undefined,
	m_OK
};

#define CASE( i )	\
	case m##i:\
		Message = OGZINF_UC_AFFIX "_" #i;\
		break

static const char *GetRawMessage_( message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( TestMessage );
	break;
	default:
		qRGnr();
		break;
	}

	return Message;
}

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &Backend,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::request__ &Request,\
		bso::bool__ &,\
		void *UP )

#define STUFF common::rStuff &Stuff = *(common::rStuff *)UP
#define BACKEND	STUFF;common::rBackend &Backend = Stuff.Backend()

static void Report_(
	message__ Message,
	const fblbkd::backend___ &Backend,
	fblbrq::request__ &Request )
{
qRH
	str::string Translation;
	TOL_CBUFFER___ Buffer;
qRB
	Translation.Init();

	Backend.Locale().GetTranslation( GetRawMessage_( Message ), Backend.Language(), Translation );
	Request.ReportRequestError( Translation.Convert( Buffer ) );
qRR
qRT
qRE
}

#define REPORT( v )	Report_( ( v ), Backend, Request )

inline static void Return_(
	message__ &M,
	message__ m )
{
	M = m;
	qRReturn;
}

#define RETURN( message )\
	Return_( Message, m##message )\

static void Handle_(
	message__ Message,
	const fblbkd::backend___ &Backend,
	fblbrq::request__ &Request )
{
	if ( Message != m_OK )
		REPORT( Message );
}

#define HANDLE( f )	Handle_( ( f ), Backend, Request )

#define DEC( name )\
	static inline void name(\
		fblbkd::backend___ &Backend,\
		fblbkd::untyped_module &,\
		fblbkd::index__,\
		fblbkd::command__,\
		fblbkd::request__ &Request,\
		bso::bool__ &,\
		void *UP )

DEC( Test )
{
	message__ Message = m_OK;
qRH
qRB
	BACKEND;

	Message = mTestMessage;
qRR
qRT
	HANDLE( Message );
qRE
}

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

	Backend.Add( D( Test ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Backend.Add( D( GetTypes ),
		fblbkd::cEnd,
			fblbkd::cStrings,	// Labels,
			fblbkd::cId8s,		// Ids.
		fblbkd::cEnd );
}

