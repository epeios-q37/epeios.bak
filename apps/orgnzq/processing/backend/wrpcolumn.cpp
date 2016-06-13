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

#include "wrpcolumn.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"

#include "ogzinf.h"

#include "common.h"

#include "sclmisc.h"

using namespace wrpcolumn;
using namespace ogzclm;
using common::rStuff;
using common::GetTypes;

const char *wrpcolumn::dColumn::PREFIX = WRPCOLUMN_COLUMN_PREFIX;
const char *wrpcolumn::dColumn::NAME = WRPCOLUMN_COLUMN_NAME;

#define ARGS (\
	dColumn_ &Column,\
	fblbkd::backend___ &Backend,\
	fblbkd::rRequest &Request )

typedef void (* f_manager ) ARGS;

void wrpcolumn::dColumn::HANDLE(
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
qRH
qRB
	Column().Init();
qRR
qRT
qRE
}

DEC( Define )
{
	if ( Request.IdIn() != fbltyp::UndefinedId )
		qRVct();

	Column().Init();
}

namespace {
	inline void Update_(
		ogztyp::sRow Type,
		ogzclm::eNumber Number,
		const str::dString &RawLabel,
		const str::dString &Comment,
		ogzclm::rColumnBuffer &Column )
	{
	qRH
		str::wString Label;
	qRB
		Label.Init( RawLabel );

		Label.StripCharacter(' ' );

		if ( Label.Amount() == 0 )
			REPORT( FieldLabelCanNotBeEmpty );

		if ( !GetTypes().Exists( Type ) )
			REPORT( UnknowFieldType);

		if ( !ogzclm::Exists( Number ) )
			REPORT( UnknownFieldNumber );

		Column.Init( Type, Number, Label, Comment );
	qRR
		qRT
		qRE
	}
}

DEC( Update )
{
	const fbltyp::sId &Type = Request.IdIn();
	const fbltyp::sId8 &Number = Request.Id8In();
	const fbltyp::dString
		&Label = Request.StringIn(),
		&Comment = Request.StringIn();

	if ( *Number >= ogzclm::n_amount )
		qRGnr();

	Update_( *Type, (ogzclm::eNumber)*Number, Label, Comment, Column() );
}

namespace {
	void Get_(
		const ogzclm::rColumnBuffer &Column,
		fblbkd::rRequest &Request )
	{
		Request.IdOut() = *Column.GetType();
		Request.Id8Out() = ( Column.GetNumber() == ogzclm::n_Undefined ? fbltyp::UndefinedId8 : Column.GetNumber() );
		Request.StringOut() = Column.GetLabel();
		Request.StringOut() = Column.GetComment();
	}
}

DEC( Get )
{
	Get_( Column(), Request );
}

#define D( name )	#name, (void *)exported##name

void wrpcolumn::dColumn::NOTIFY( fblbkd::rModule &Module )
{
	Module.Add( D( Initialize ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Module.Add( D( Define ),
		fblbkd::cId,		// Column id. New column if == 'qNIL'.
	fblbkd::cEnd,
	fblbkd::cEnd );

	Module.Add( D( Update ),
		fblbkd::cId,		// Field type.
		fblbkd::cId8,		// Field number. ('ogzclm::eNumber').
		fblbkd::cString,	// Label.
		fblbkd::cString,	// Comment.
	fblbkd::cEnd,
	fblbkd::cEnd );

	Module.Add( D( Get ),
		fblbkd::cEnd,
			fblbkd::cId,		// Type id.
			fblbkd::cId8,		// Number id.
			fblbkd::cString,	// Label.
			fblbkd::cString,	// Comment.
		fblbkd::cEnd );
}

