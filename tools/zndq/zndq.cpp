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

#include "zndq.h"

// No inclusion of Epeios related header, to avoid redefinition errors and warnings.

# include "main.h"

#if PHP_MAJOR_VERSION != 7
# error "Does actually only work with PHP 7. \
The usually available documentation about extensions is only about PHP 5, \
and there are important differences with PHP 7. See http://wiki.php.net/phpng-upgrading"
#endif


PHP_FUNCTION( zndq_init )
{
	zend_string *String;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S", &String ) != SUCCESS )
		main::ThrowGenericError();

	main::Init( String->val, String->len );
}

PHP_FUNCTION( zndq_wrapper_info )
{
	RETURN_STRING( main::WrapperInfo() );
}

PHP_FUNCTION( zndq_component_info )
{
	zend_long Launcher = 0;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &Launcher ) != SUCCESS )
		main::ThrowGenericError();

	RETURN_STRING( main::ComponentInfo( Launcher ) );
}

PHP_FUNCTION( zndq_register )
{
	zend_string *String;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S", &String ) != SUCCESS )
		main::ThrowGenericError();

	// 'String->val' contains a list of arguments, as defined in the configuration file for the 'Launch' command.
	RETURN_LONG( main::Register( String->val, String->len ) )
}

PHP_FUNCTION( zndq_call )
{
	zend_long Launcher = 0, Index = 0;
	int num_varargs;
	zval *varargs = NULL;

	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ll*", &Launcher, &Index, &varargs, &num_varargs ) != SUCCESS )
		main::ThrowGenericError();

	main::Call( Launcher, Index, num_varargs, varargs, return_value TSRMLS_CC );
}


extern zend_module_entry zndq_module_entry;
#define phpext_zenq_ptr &zenq_module_entry

static zend_function_entry zndq_functions[] = {
	PHP_FE( zndq_init, NULL )
	PHP_FE( zndq_register, NULL )
	PHP_FE( zndq_wrapper_info, NULL )
	PHP_FE( zndq_component_info, NULL )
	PHP_FE( zndq_call, NULL )
{
	NULL, NULL, NULL
}
};

// To 'stringify' the 'VERSION' macro content below.
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
	// Do not manage to define the value of the 'VERSION' macro
	// as a string in the 'config.m4' file, so it is 'stringified' here.
	STR( VERSION ),
#endif
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE( zndq )
