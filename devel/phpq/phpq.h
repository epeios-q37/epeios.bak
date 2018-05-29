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

// (From PHP)

#ifndef PHPQ_INC_
# define PHPQ_INC_

# define PHPQ_NAME		"PHPQ"

# if defined( E_DEBUG ) && !defined( PHPQ_NODBG )
#  define PHPQ_DBG
# endif

#include "cpe.h"

# ifdef CPE_S_WIN
#  define PHP_WIN32
# endif

# include "zendq.h"
// Below line MUST BE below above line.
# include "php.h"

# include "err.h"

namespace phpq {
	typedef zendq::sResource<php_stream, php_file_le_stream> sStream_;

	class sStream
	: public sStream_
	{
	public:
		int Get( void )
		{
# ifdef ZTS
			return _php_stream_getc( P_(), T_() );
# else
			return php_stream_getc( P_() );
# endif
		}
	};
	
}

#endif
