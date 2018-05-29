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

// THread Tool SUBmodule

#ifndef THTSUB__INC
# define THTSUB__INC

# define THTSUB_NAME		"THTSUB"

# if defined( E_DEBUG ) && !defined( THTSUB_NODBG )
#  define THTSUB_DBG
# endif

# include "cpe.h"

# ifdef CPE_S_POSIX
#  include <pthread.h>
# elif defined( CPE_S_WIN )
# else 
#  error
# endif

namespace thtsub {
# ifdef CPE_S_POSIX
	typedef pthread_t sThreadID;
# elif defined( CPE_S_WIN )
	typedef unsigned long sThreadID;
# else 
#  error
# endif
}

#endif
