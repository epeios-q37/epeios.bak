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

using namespace wrpfield;

using common::rStuff;
using common::GetTypes;

const char *wrpfield::dField::PREFIX = WRPFIELD_FIELD_PREFIX;
const char *wrpfield::dField::NAME = WRPFIELD_FIELD_NAME;

#define ARGS (\
	dField_ &Field,\
	fblbkd::backend___ &BaseBackend,\
	fblbkd::rRequest &Request )

typedef void (* f_manager ) ARGS;

void wrpfield::dField::HANDLE(
	fblbkd::backend___ &Backend,
	fblbkd::rModule &Module,
	fblbkd::command__ Command,
	fblbkd::rRequest &Request )
{
	((f_manager)Module.Functions( Command ))( *this, Backend, Request );
}

#define DEC( name )	static void exported##name ARGS

DEC( Initialize )
{
	Field().Init();
}

DEC( Define )
{
	STUFF;
	DATABASE;

	ogztyp::sRow Type = qNIL;

	ogzbsc::sFRow FieldRow = *Request.IdIn();

	if ( !Database.GetEntries( FieldRow, Stuff.User(), Field, Field.Type(), Field.Number(), qRPU ) )
		REPORT( NoSuchField );
}

DEC( UpdateEntry )
{
	sdr::sRow Row = *Request.IdIn();	// If == 'qNIL', new entry is created.
	const str::dString &Entry = Request.StringIn();

	if ( !Field.Exists( Row ) )
		REPORT( NoSuchEntry );

	if ( !GetTypes()( Field.Type() ).Test( Entry ) )
		REPORT( BadEntryValue );

	if ( Row == qNIL ) {
		if ( Field.Amount() == 1 )
			if ( Field.Number() == ogzclm::nMono )
				REPORT( OnlyOneEntryAllowed );

		Row = Field.New();
		Field(Row).Init();
	}

	Field.Store( Entry, Row );
}

namespace {
	void Get_(
		const dField_ Field,
		fbltyp::dIStrings &Strings )
	{
	qRH
		fbltyp::wIString String;
		sdr::sRow Row = qNIL;
	qRB
		Row = Field.First();

		while ( Row != qNIL ) {
			String.Init(*Row, Field( Row ) );
			Strings.Append( String );

			Row = Field.Next( Row );
		}
	qRR
	qRT
	qRE
	}
}

DEC( Get )
{
	Request.IdOut() = *Field.Type();
	Request.Id8Out() = Field.Number();

	Get_(Field, Request.IStringsOut() );
}

namespace{
	void Update_(
		sdr::sRow Row,	// If == 'qNIL', a new entry is created.
		const str::dString &Entry,
		dField_ &Field )
	{
		if ( Row == qNIL ) {
			Row = Field.New();
			Field(Row).Init();
		} else if ( !Field.Exists( Row ) )
			REPORT( UnknownEntry );

		Field.Store( Entry, Row  );
	}
}

DEC( Update )
{
	sdr::sRow Row = *Request.IdIn();
	const str::dString &Entry = Request.StringIn();

	Update_( Row, Entry, Field );
}

#define D( name )	#name, (void *)exported##name

void wrpfield::dField::NOTIFY( fblbkd::rModule &Module )
{
	Module.Add( D( Initialize ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Define ),
			fblbkd::cId,		// Field id.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Get ),
		fblbkd::cEnd,
			fblbkd::cId,	// Type.
			fblbkd::cId8,	// Number,
			fblbkd::cIStrings,			
		fblbkd::cEnd );

	Module.Add( D( UpdateEntry ),
			fblbkd::cId,		// Id of entry to update ; if == 'qNIL', a new entry is created.
			fblbkd::cString,	// Then entry value.		
		fblbkd::cEnd,
		fblbkd::cEnd );



	/*
	Module.Add( D(  ),
		fblbkd::cEnd,
		fblbkd::cEnd );
	*/
}

