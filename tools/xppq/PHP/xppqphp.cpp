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

#include "sclmisc.h"

# define NAME_MC			"XPPqPHP"
# define NAME_LC			"xppqphp"
# define NAME_UC			"XPPQPHP"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"

#define PHP_XPPQ_VERSION "1.0"
#define PHP_XPPQ_EXTNAME "xppq"


extern zend_module_entry xppq_module_entry;
#define phpext_wppq_ptr &xppq_module_entry
 
// declaration of a custom my_function()
PHP_FUNCTION(MyFunction);
// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static zend_function_entry my_functions[] = {
    PHP_FE(MyFunction, NULL)
    {NULL, NULL, NULL}
};
 
// the following code creates an entry for the module and registers it with Zend.
zend_module_entry xppq_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_XPPQ_EXTNAME,
    my_functions,
    NULL, // name of the MINIT function or NULL if not applicable
    NULL, // name of the MSHUTDOWN function or NULL if not applicable
    NULL, // name of the RINIT function or NULL if not applicable
    NULL, // name of the RSHUTDOWN function or NULL if not applicable
    NULL, // name of the MINFO function or NULL if not applicable
#if ZEND_MODULE_API_NO >= 20010901
    PHP_XPPQ_VERSION,
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

 
// implementation of a custom my_function()
PHP_FUNCTION(MyFunction)
{
    //RETURN_STRING("This is my function from the NEW XPPQ component.\n", 1);

	str::wString Location;

	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	// TODO : Find a way to fill 'Location' with the path of the binary.

	sclmisc::Initialize( Rack_, Location, qRPU );

	cio::COut << "Hello !!!" << txf::nl << txf::commit;
}

namespace sclmisc {
const char *SCLMISCTargetName = NAME_LC;
const char *SCLMISCProductName = NAME_MC;
}