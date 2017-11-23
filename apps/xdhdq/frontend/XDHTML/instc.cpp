/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "instc.h"

#include "flf.h"
#include "fnm.h"

using namespace instc;

using fbltyp::wString;
using fbltyp::wStrings;

#define F( name )	case f##name : return #name; break

const char *instc::GetLabel( eField Field )
{
	switch ( Field ) {
	F( CKEditor );
	F( JQTE );
	F( Datepicker );
	F( Timepicker );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

namespace {
	stsfsm::automat FieldAutomat_;

	void InitAndFillFieldAutomat_( void )
	{
		FieldAutomat_.Init(),
		stsfsm::Fill( FieldAutomat_, f_amount, GetLabel );
	}
}

eField instc::GetField( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, FieldAutomat_, f_Undefined, f_amount );
}


Q37_GCTOR( instc )
{
	InitAndFillFieldAutomat_();
}


