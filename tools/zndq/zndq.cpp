/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of ZNDq.

	ZNDq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	ZNDq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with ZNDq. If not, see <http://www.gnu.org/licenses/>.
*/

# include "zndq.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define ZEND_WIN32	1
# define PHP_WIN32	1
# define ZEND_DEBUG	0
# define ZEND_WIN32_FORCE_INLINE
# define ZTS
#endif

# include "php.h"

extern "C" {
	PHP_FUNCTION( infoWrapper )
	{
		RETURN_STRING( "Hello World the World !!!" );
	}
}

extern zend_module_entry zndq_module_entry;
#define phpext_zenq_ptr &zenq_module_entry

static zend_function_entry zndq_functions[] = {
	PHP_FE( infoWrapper, NULL )
{
	NULL, NULL, NULL
}
};

# define STR_(x) #x
# define STR(x) STR_(x)

zend_module_entry zndq_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	NAME_LC,
	zndq_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	STR( VERSION ),
#endif
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE( zndq )

namespace sclmisc {
	const char *SCLMISCTargetName = NAME_LC;
	const char *SCLMISCProductName = NAME_MC;
}

