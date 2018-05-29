/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

// STacK CRaTe

#ifndef STKCRT_INC_
# define STKCRT_INC_

# define STKCRT_NAME		"STKCRT"

# if defined( E_DEBUG ) && !defined( STKCRT_NODBG )
#  define STKCRT_DBG
# endif

# include "err.h"

# include "stkctn.h"

namespace stkcrt {
	using namespace stkctn;
}

#endif
