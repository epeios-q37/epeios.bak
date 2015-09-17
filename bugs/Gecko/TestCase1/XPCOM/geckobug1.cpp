/*
	'GeckoBug1.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'eGeckoCOM' software.

    'GeckoBug1' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'GeckoBug1' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef _MSC_VER
# define XP_WIN
#elif defined ( __GNUC__ )
# define XP_UNIX
#else
# error
#endif

#ifdef XP_WIN
# include <windows.h>
# include <sys/utime.h>
#endif

#ifdef XP_UNIX
# include <utime.h>
#endif

#include "GeckoBug1.h"

#define E__STRING(x) #x
#define E_STRING(x) E__STRING(x)
// The use of 'utime()' allows to see what happens using 'strace' under Linux and 'dtruss' under MacOS'.
#define LOG utime( "LOG : " __FILE__ " " E_STRING( __LINE__ ), NULL )

#include "xpcom-config.h"

#define COMPONENT_VERSION	"1"

#define APP_NAME	"GeckoBug1"

#define GECKOBUG1_CONTRACTID "@q37.info/geckobug1;" COMPONENT_VERSION
#define GECKOBUG1_CLASSNAME "Gecko bug 1"
#define GECKOBUG1_CID  IGECKOBUG1_IID

class geckobug1__
: public IGeckoBug1
{
private:
	~geckobug1__();
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IGECKOBUG1

  geckobug1__();
};

#include "nsServiceManagerUtils.h"
#include "nsCOMPtr.h"
#include "nsIDirectoryService.h"

template <typename t> inline t *GetService_(
	const char *ContractID,
	nsCOMPtr<t> &Service )
{	
	nsresult Error = NS_OK;

	LOG;

	Service = do_GetService( ContractID, &Error );

	if ( Error == NS_OK )
		LOG;	// Under Windows, in both cases (see 'Main.xul'), we go through here, indicating that the call succeeds.
	else if ( Error == NS_ERROR_NOT_INITIALIZED )
		LOG;	// Under Linux and MacOS, in both cases (see 'Main.xul'), we go through here, indicating that the call fails with the 'NS_ERROR_NOT_INITIALIZED' error.
	else {
		char Buffer[100];
		sprintf( Buffer, "%x", Error );
		utime( Buffer, NULL );
	}

	return Service;
}


NS_IMETHODIMP geckobug1__::Test( void )
{
	LOG;

	nsCOMPtr<nsIDirectoryServiceProvider> DirectoryServiceProvider;

	// It also has the same issue with other valid CID as "@mozilla.org/file/directory_service;1".
	if ( GetService_( "@mozilla.org/file/directory_service;1", DirectoryServiceProvider ) != NULL )
		LOG;
	else
		LOG;

	return NS_OK;
}

geckobug1__::geckobug1__( void )
{
	LOG;
}

geckobug1__::~geckobug1__( void )
{
	LOG;
}

/* Gecko required part. */

# include "mozilla/ModuleUtils.h"

NS_IMPL_ISUPPORTS1(geckobug1__, IGeckoBug1)
NS_GENERIC_FACTORY_CONSTRUCTOR(geckobug1__)

NS_DEFINE_NAMED_CID(GECKOBUG1_CID);

static const mozilla::Module::CIDEntry kGeckoBug1CIDs[] = {
    { &kGECKOBUG1_CID, false, NULL, geckobug1__Constructor },
    { NULL }
};

static const mozilla::Module::ContractIDEntry kGeckoBug1Contracts[] = {
    { GECKOBUG1_CONTRACTID, &kGECKOBUG1_CID },
    { NULL }
};

static const mozilla::Module::CategoryEntry kGeckoBug1Categories[] = {
    { NULL }
};

static const mozilla::Module GeckoBug1Module = {
    mozilla::Module::kVersion,
    kGeckoBug1CIDs,
    kGeckoBug1Contracts,
    kGeckoBug1Categories
};

NSMODULE_DEFN(GeckoBug1Module) = &GeckoBug1Module;

// To verify if the component is loaded or not.
class auto__ {
public:
	auto__( void )
	{
		LOG;
	}
	~auto__( void )
	{
		LOG;
	}
} Auto;