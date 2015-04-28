/*
	'fblbrq' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblbrq' header file ('fblbrq.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define FBLBRQ__COMPILATION

#include "fblbrq.h"

using namespace fblbrq;

void fblbrq::request__::_Pop(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	sdr::row__ Row = E_NIL;
	cast__ Cast = c_Undefined;

	Row = Casts.First();

	while ( ( Row != E_NIL )
		    && ( ( Cast = (cast__)*Casts( Row ) ) != cEnd ) ) {

		if ( Flow.Get() != Cast )
			ERRFwk();

		_C().PopIn( Row, Flow, Cast );

		Row = Casts.Next( Row );
	}

	if ( Row == E_NIL )
		ERRFwk();

	if ( Flow.Get() != fblcst::cEnd )
		ERRFwk();

	_C().PopInEnd( Row, Flow);

	Row = Casts.Next( Row );

	while ( Row != E_NIL ) {
		if ( ( Cast = (cast__)Flow.Get() ) != *Casts( Row ) )
			ERRFwk();

		_C().PopOut( Row, Flow, Cast );

		Row = Casts.Next( Row );
	}
}
