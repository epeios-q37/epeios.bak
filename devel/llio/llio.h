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

// Low-Level Input/Output 

#ifndef LLIO_INC_
#define LLIO_INC_

#define LLIO_NAME		"LLIO"

#define	LLIO_VERSION	"$Revision: 1.13 $"

#define LLIO_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"


#if defined( E_DEBUG ) && !defined( LLIO_NODBG )
#define LLIO_DBG
#endif

#include "cpe.h"

# undef LLIO__USE_PLLIO
# undef LLIO__USE_WLLIO

#ifdef CPE_S_WIN
# define LLLIO__USE_WLLIO
#elif defined( CPE_S_POSIX )
# define LLLIO__USE_PLLIO
#endif

# if defined( LLLIO__USE_PLLIO )
#  include "pllio.h"
#  define LLIO_UNDEFINED_DESCRIPTOR PLLIO_UNDEFINED_DESCRIPTOR
# elif defined( LLLIO__USE_WLLIO )
#  include "wllio.h"
#  define LLIO_UNDEFINED_DESCRIPTOR WLLIO_UNDEFINED_DESCRIPTOR
# else
#  error "Should not occur !!!"
# endif

namespace llio {
# ifdef LLLIO__USE_MLLIO
	using namespace mllio;
# elif defined( LLLIO__USE_PLLIO )
	using namespace pllio;
# elif defined( LLLIO__USE_WLLIO )
	using namespace wllio;
# else
#  error "Should not occur !!!"
# endif
}

/*$END$*/
#endif
