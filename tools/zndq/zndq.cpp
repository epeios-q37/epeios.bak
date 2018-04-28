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

#include "epsmsc.h"
#include "lstcrt.h"
#include "sclmisc.h"

#if PHP_MAJOR_VERSION != 7
# error "Does actually only work with PHP 7. \
The usually available documentation about extensions is only about PHP 5, \
and there are important differences with PHP 7. See http://wiki.php.net/phpng-upgrading"
#endif

SCLI_DEF( zndq, NAME_LC, NAME_MC );

namespace {
	bso::sBool Initialized_ = false;
	qROW( DRow_ );
	lstbch::qLBUNCHw( zend_long, sDRow_ ) Launchers_;
	lstcrt::qLMCRATEw( str::dString, sDRow_ ) Discriminators_;

	zend_long Search_( const str::dString &Discriminator )
	{
		sDRow_ Row = Discriminators_.First();

		while ( (Row != qNIL) && (Discriminators_( Row ) != Discriminator) )
			Row = Discriminators_.Next( Row );

		if ( Row != qNIL )
			return Launchers_( Row );
		else
			return 0;
	}

	void Set_(
		const str::dString &Discriminator,
		zend_long Launcher )
	{
		sDRow_ Row = Discriminators_.New();

		Discriminators_( Row ).Init( Discriminator );

		if ( Row != Launchers_.Add( Launcher ) )
			qRGnr();
	}
}

PHP_FUNCTION( zndq_init )
{
	if ( !Initialized_ ) {
		Initialized_ = true;
		zend_string *String;

		if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "S", &String ) != SUCCESS )
			main::ThrowGenericError();

		main::Init( String->val, String->len );
	}
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
	zend_long Launcher = 0;
qRH;
	zend_string *String = NULL, *RawDiscriminator = NULL;
	str::wString Discriminator;
qRB;
	if ( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "SS", &String, &RawDiscriminator ) != SUCCESS )
		main::ThrowGenericError();

	Discriminator.Init();
	Discriminator.Append( RawDiscriminator->val, RawDiscriminator->len );

	// A PHP script may be launched on each http request, but the registering must only occurring the first time.
	// Also, 'zndq' could be used for different scripts at the same time, hence the discriminator.
	if ( ( Launcher = Search_( Discriminator ) ) == 0 )	{
		// 'String->val' contains a list of arguments, as defined in the configuration file for the 'Launch' command.
		Launcher = main::Register( String->val, String->len );
		Set_( Discriminator, Launcher );
	}
qRR;
qRT;
qRE;
	RETURN_LONG( Launcher );
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

qGCTOR( zndq )
{
	Launchers_.Init();
	Discriminators_.Init();
}
