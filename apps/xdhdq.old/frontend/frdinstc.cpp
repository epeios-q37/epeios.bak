/*
	'frdinstc.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "frdinstc.h"

#include "flf.h"
#include "fnm.h"

using namespace frdinstc;


#define F( name )	case f##name : return #name; break

const char *frdinstc::GetLabel( field__ Field )
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

field__ frdinstc::GetField( const str::string_ &Pattern )
{
	return stsfsm::GetId( Pattern, FieldAutomat_, f_Undefined, f_amount );
}


Q37_GCTOR( frdinstc )
{
	InitAndFillFieldAutomat_();
}



