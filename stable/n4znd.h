/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// Native 4(for) ZeND
// Extension of the 'N4ALL' library for use with PHP.
// Common part between wrapper and component ('scln4a').

#ifndef N4ZND_INC_
# define N4ZND_INC_

# define N4ZND_NAME		"N4ZND"

# if defined( E_DEBUG ) && !defined( N4ZND_NODBG )
#  define N4ZND_DBG
# endif

# include "err.h"
# include "fdr.h"
# include "n4all.h"

namespace n4znd {

	struct gShared
	{
	public:
		void reset( bso::sBool = true )
		{}
		qCDTOR( gShared );
	};

	qENUM( Type ) {
		tString,
		tStream,
		tLong,
		tBool,
		tStrings,
		t_amount,
		t_Undefined
	};
}

#endif
