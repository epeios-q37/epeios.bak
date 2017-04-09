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


#include "sclznd.h"
//#include "zend_modules.h"

#include "phpq.h"

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
ZEND_FUNCTION(Wrapper);
// list of custom PHP functions provided by this extension
// set {NULL, NULL, NULL} as the last record to mark the end of list
static zend_function_entry my_functions[] = {
    ZEND_FE(Wrapper, NULL)
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
 
void Test( sclznd::sArguments &Arguments )
{
	long Long1 = 0, Long2 = 0, Long3 = 0, Long4 = 0, Long5 = 5;
	zendq::sArray Array;
	phpq::sStream Stream;

	Array.Init();
	Stream.Init();
	Arguments.Get( Long1, Long2, Array, Stream );

	Array.Get( 0, Long3 );
	Array.Get( 1, Long4 );
	Array.Get( "toto", Long5 );

	cio::COut << "SCLZND !" << txf::pad << Long1 << txf::pad << Long2 << txf::pad << Long3 << txf::pad << Long4 << txf::pad << Long5 << txf::nl << txf::commit;

	int C;

	while ( ( C = Stream.Get() ) != -1 )
		cio::COut << (char)C;

	cio::COut << txf::commit;
}

void sclznd::SCLZNDRegister( sRegistrar &Registrar )
{
	Registrar.Register( Test );
}

namespace sclmisc {
const char *SCLMISCTargetName = NAME_LC;
const char *SCLMISCProductName = NAME_MC;
}