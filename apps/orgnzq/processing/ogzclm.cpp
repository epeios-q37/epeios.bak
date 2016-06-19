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

#include "ogzclm.h"

using namespace ogzclm;

#define C( name )	case n##name : return #name ; break

const char *ogzclm::GetLabel( eNumber Number )
{
	switch ( Number ) {
		C( Mono );
		C( Multi );
	default:
		qRGnr();
		break;
	}

	return NULL; // To avoid a warning.
}

sRow ogzclm::mColumns::New(
	ogztyp::sRow Type,
	eNumber Number,
	ogzmta::sRow Label,
	ogzmta::sRow Comment,
	sRow ColumnRow ) const
{
	sColumn Column;

	Column.Init( Type, Number, Label, Comment );

	ColumnRow = mColumns_::New( ColumnRow );

	Store( Column, ColumnRow );

	return ColumnRow;
}

bso::sBool ogzclm::mColumns::GetTypeAndNumber(
	sRow ColumnRow,
	ogztyp::sRow &Type,
	eNumber &Number,
	qRPN ) const
{
	bso::sBool Exists = false;
qRH
	ogzclm::sColumn Column;
qRB
	Column.Init();
	Exists = Recall( ColumnRow, Column );

	if ( !Exists ) {
		if ( qRPT )
			qRFwk();
		else
			qRReturn;
	}

	Type = Column.GetType();
	Number = Column.GetNumber();
qRR
qRT
qRE
	return Exists;
}


