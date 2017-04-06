/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/


#include "php.h"

#include "zend_API.h"
//#include "zend_modules.h"

#include "sclmisc.h"

# define NAME_MC			"XPPqZND"
# define NAME_LC			"xppqznd"
# define NAME_UC			"XPPQZND"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"

#define ZND_XPPQ_VERSION "1.0"
#define ZND_XPPQ_EXTNAME "xppq"

/*
extern zend_module_entry xppq_module_entry;
#define phpext_wppq_ptr &xppq_module_entry
 */
// declaration of a custom my_function()
ZEND_FUNCTION(MyFunction);
// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static zend_function_entry my_functions[] = {
    ZEND_FE(MyFunction, NULL)
    {NULL, NULL, NULL}
};
 
// the following code creates an entry for the module and registers it with Zend.
zend_module_entry xppq_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    ZND_XPPQ_EXTNAME,
    my_functions,
    NULL, // name of the MINIT function or NULL if not applicable
    NULL, // name of the MSHUTDOWN function or NULL if not applicable
    NULL, // name of the RINIT function or NULL if not applicable
    NULL, // name of the RSHUTDOWN function or NULL if not applicable
    NULL, // name of the MINFO function or NULL if not applicable
#if ZEND_MODULE_API_NO >= 20010901
    ZND_XPPQ_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
 
ZEND_GET_MODULE(xppq)

namespace {
	err::err___ Error_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;

	void ERRFinal_( void )
	{
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

//		Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message );
	}
}

void Get_(
	zval **Val,
	void ***tsrm_ls,
	long &Long )
{
	zval **ZVal = NULL;
	php_stream *Stream = NULL;
	int le_myfile = 0;

	long **L = NULL;

	switch ( Z_TYPE_P(*Val) ) {
	case IS_LONG:
		Long = Z_LVAL_PP( Val );
		break;
	case IS_ARRAY:
		zend_hash_index_find( Z_ARRVAL_PP( Val ), 0, (void **)&ZVal );
		zend_hash_index_find( Z_ARRVAL_PP( Val ), 1, (void **)&ZVal );
		zend_hash_index_find( Z_ARRVAL_PP( Val ), 2, (void **)&ZVal );
		zend_hash_index_find( Z_ARRVAL_PP( Val ), 3, (void **)&ZVal );
//		zend_hash_get_current_key_zval( , &ZVal);
		break;
	case IS_RESOURCE:
		Long = Z_RESVAL_PP( Val );
//		php_stream_from_zval( Stream, Val );
		ZEND_FETCH_RESOURCE_NO_RETURN( Stream, php_stream *, Val, -1, "stram", php_file_le_stream());
//		ZEND_FETCH_RESOURCE_NO_RETURN( File, FILE *, Val, -1, "standard-c-file", le_myfile );
		break;
	default:
		qRFwk();
		break;
	}

}

void Get_(
	zval ***ZVals,
	void ***tsrm_ls,
	int Index )
{
}

template <typename arg, typename ...args> void Get_(
	zval ***ZVals,
	void *** tsrm_ls,
	int Index,
	arg &Arg,
	args &...Args )
{
	Get_( ZVals[Index], tsrm_ls, Arg );

	Get_( ZVals, tsrm_ls, Index+1, Args... );
}


template <typename ...args> void Get(
	int num_args TSRMLS_DC,
	args &...Args )
{
	int num_varargs;
	zval ***varargs = NULL;

	zend_parse_parameters( num_args TSRMLS_CC, "*", &varargs, &num_varargs );

	Get_( varargs, tsrm_ls, 0, Args... );
}


 
// implementation of a custom my_function()
ZEND_FUNCTION(MyFunction)
{
//    RETURN_STRING("This is my function from the NEW XPPQ component !!!!\n", 1);

	long Long1 = 0, Long2 = 0;

	Get( ZEND_NUM_ARGS() TSRMLS_CC, Long1, Long2 );

//	zend_parse_parameters( ht , tsrm_ls, "l", &Long );

	RETURN_LONG( Long2 );


	str::wString Location;

	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	// TODO : Find a way to fill 'Location' with the path of the binary.

	sclmisc::Initialize( Rack_, Location, qRPU );

	cio::COut << "Hello !!!!" << txf::nl << txf::commit;

//	zend_hash_get_current_data
}

namespace sclmisc {
const char *SCLMISCTargetName = NAME_LC;
const char *SCLMISCProductName = NAME_MC;
}