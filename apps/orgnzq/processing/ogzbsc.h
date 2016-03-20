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

// OrGaniZer BaSiCs

#ifndef OGZBSC__INC
# define OGZBSC__INC

# include "bso.h"
# include "err.h"
# include "sdr.h"
# include "str.h"

namespace ogzbsc {

	qROW( DRow );	// Datum row.
	qROW( TRow );	// Type row.
	qROW( UTRow );	// User type row.
	qROW( CRow );	// Column row.
	qROW( FRow );	// Field row.
	qROW( RRow );	// Record row.
	qROW( URow );	// User row.
	qROW( URRow );	// User record row.

	using sdr::sSize;

	typedef str::dString dDatum;
	qW( Datum );
}

#endif
