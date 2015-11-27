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

#ifndef CSDRCD__INC
# define CSDRCD__INC

# define CSDRCD_NAME		"CSDRCD"

# if defined( E_DEBUG ) && !defined( CSDRCD_NODBG )
#  define CSDRCD_DBG
# endif

// Client-Server Devices Remove Client Downstream

# include "csdrcc.h"

# include "err.h"

namespace csdrcd {

	using csdrcc::driver___;

}

#endif
