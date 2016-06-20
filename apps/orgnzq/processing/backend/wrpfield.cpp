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

#include "wrpcolumn.h"

using namespace wrpfield;

using common::rStuff;
using common::GetTypes;

const char *wrpfield::dField::PREFIX = WRPFIELD_FIELD_PREFIX;
const char *wrpfield::dField::NAME = WRPFIELD_FIELD_NAME;

#define ARGS (\
	dField &Field,\
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

DEC( New )
{
	BACKEND;

	const ogzclm::rColumnBuffer &Column = Backend.Object<wrpcolumn::dColumn>( Request.ObjectIn() )();

	Field.Init();

	Field.Type() = Column.GetType();
	Field.Number() = Column.Number();
}

DEC( Fill )
{
	USER;
	DATABASE;

	ogztyp::sRow Type = qNIL;

	ogzbsc::sFRow FieldRow = *Request.IdIn();

	if ( FieldRow == qNIL )
		qRGnr();

	Field.Init();

	if ( !Database.GetEntries( User, FieldRow, Field, Field.Type(), Field.Number(), qRPU ) )
		REPORT( NoSuchField );
}

DEC( UpdateEntry )
{
	sdr::sRow Row = *Request.IdIn();	// If == 'qNIL', new entry is created, unless for a mono field, where the entry is created/updated.
	const str::dString &Entry = Request.StringIn();

	if ( !Entry.IsBlank() ) {
		if ( !GetTypes()( Field.Type() ).Test( Entry ) )
			REPORT( BadEntryValue );

		if ( Field.Number() == ogzclm::nMono ) {
			if ( Row != qNIL )
				REPORT( EntryRowShouldBeNILForMonoField );

			if ( Field.Amount() == 0 ) {
				Row = Field.New();
				Field( Row ).Init();
			} else if ( Field.Amount() == 1 )
				Row = Field.First();
			else
				qRGnr();
		} else if ( Row == qNIL ) {
			Row = Field.New();
			Field( Row ).Init();
		} else if ( !Field.Exists( Row ) )
			REPORT( NoSuchEntry );

		Field.Store( Entry, Row );
	} else if ( Row != qNIL ) {
		if ( Field.Number() == ogzclm::nMono )
			REPORT( EntryRowShouldBeNILForMonoField );

		if ( !Field.Exists( Row ) )
			REPORT( NoSuchEntry );

		Field.Remove( Row );
	}
}

namespace {
	void Get_(
		const dField_ &Field,
		fbltyp::dIds &Ids,
		fbltyp::dStrings &Strings )
	{
		sdr::sRow Row = Field.First();

		while ( Row != qNIL ) {
			Ids.Append( *Row );
			Strings.Append( Field( Row ) );

			Row = Field.Next( Row );
		}
	}
}

DEC( Get )
{
	Request.IdOut() = *Field.Type();
	Request.Id8Out() = Field.Number();

	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dStrings &Entries = Request.StringsOut();

	Get_(Field, Ids, Entries );
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
	Module.Add( D( New ),
			fblbkd::cObject,	// Column buffer id.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Fill ),
			fblbkd::cId,		// Field id.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Get ),
		fblbkd::cEnd,
			fblbkd::cId,		// Type.
			fblbkd::cId8,		// Number.
			fblbkd::cIds,		// Ids of the entries.
			fblbkd::cStrings,	// Entries.	
		fblbkd::cEnd );

	Module.Add( D( UpdateEntry ),
			fblbkd::cId,		// Id of entry to update ; if == 'qNIL', a new entry is created, unless it's a mono field, in which case the entry is updated/created.
			fblbkd::cString,	// Then entry value.		
		fblbkd::cEnd,
		fblbkd::cEnd );
}

