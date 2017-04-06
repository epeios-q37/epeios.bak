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

// From Zend (PHP related framework).

#ifndef ZENDQ_INC_
# define ZENDQ_INC_

# define ZENDQ_NAME		"ZENDQ"

# if defined( E_DEBUG ) && !defined( ZENDQ_NODBG )
#  define ZENDQ_DBG
# endif

# include "cpe.h"

# ifdef CPE_S_WIN
#  define ZEND_WIN32
# endif

// Note to developer : include 'h:\hg\ZNDIncludeDirectories.props' in the '.vcxproj'.
# include "zend_API.h"

# include "err.h"

namespace zendq {

}

#endif
