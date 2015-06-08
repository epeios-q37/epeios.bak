/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define FBLBRQ__COMPILATION

#include "fblbrq.h"

using namespace fblbrq;

void fblbrq::request__::_Pop(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	sdr::row__ Row = qNIL;
	cast__ Cast = c_Undefined;

	Row = Casts.First();

	while ( ( Row != qNIL )
		    && ( ( Cast = (cast__)*Casts( Row ) ) != cEnd ) ) {

		if ( Flow.Get() != Cast )
			qRFwk();

		_C().PopIn( Row, Flow, Cast );

		Row = Casts.Next( Row );
	}

	if ( Row == qNIL )
		qRFwk();

	if ( Flow.Get() != fblcst::cEnd )
		qRFwk();

	_C().PopInEnd( Row, Flow);

	Row = Casts.Next( Row );

	while ( Row != qNIL ) {
		if ( ( Cast = (cast__)Flow.Get() ) != *Casts( Row ) )
			qRFwk();

		_C().PopOut( Row, Flow, Cast );

		Row = Casts.Next( Row );
	}
}
