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

// OrGaniZer USeR

#ifndef OGZUSR__INC
# define OGZUSR__INC

# ifdef XXX_DBG
#	define OGZUSR__DBG
# endif

# include "ogzbsc.h"
# include "ogzcbs.h"

# include "bch.h"

namespace ogzusr {
	using ogzbsc::fURow;
	using ogzbsc::fRRow;
	using ogzbsc::fURRow;

	typedef bch::qBUNCHv( fRRow, fURRow ) vUser;
	qW( User );

// Template parameters.
# define OGZUSR_TP	ogzusr::vUser, ogzusr::iUser, ogzusr::fURow, ogzusr::fRRow, ogzusr::fURRow

	typedef ogzcbs::fDCallback<OGZUSR_TP> fCallback;

	typedef ogzcbs::fDItems<OGZUSR_TP> fUsers;
}


#endif
