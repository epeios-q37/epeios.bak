/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'ZNDq.

    'ZNDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'ZNDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'ZNDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAIN_INC_
# define MAIN_INC_

#ifdef HAVE_CONFIG_H	// Used under 'POSIX' environment.
# include "config.h"
#else	// Used under 'Windows'.
# define ZEND_WIN32	1
# define PHP_WIN32	1
# define ZEND_DEBUG	0
# define ZEND_WIN32_FORCE_INLINE
# define ZTS
#endif

#include "php.h"

// Do NOT include any Epeios related header, as this header is included along PHP header,
// 'See comment in 'zndq.cpp').

namespace main {
	void ThrowGenericError( void );

	void Init( void );
	void Register(
		const char *Arguments,
		size_t ArgumentsLength );
	const char *WrapperInfo( void );
	const char *ComponentInfo( void );
	void Launch(
		zend_long Index,
		int num_varargs,
		zval **varargs,
		zval *return_value
		TSRMLS_DC );
}

#endif
