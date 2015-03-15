/*
	'sclplugin.cpp' by Claude SIMON (http://zeusw.org/).

	'sclplugin' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLPLUGIN__COMPILATION

#include "sclplugin.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "sclmisc.h"

using namespace sclplugin;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME, plgncore::plugin_identification );
DEF( PLGNCORE_RETRIEVE_PLUGIN_FUNCTION_NAME, plgncore::retrieve_plugin );
DEF( PLGNCORE_RELEASE_PLUGIN_FUNCTION_NAME, plgncore::release_plugin );

const char *PLGNCORE_PLUGIN_IDENTIFICATION_FUNCTION_NAME( void )
{
	return sclplugin::SCLPLUGINPluginIdentification();
}

void *PLGNCORE_RETRIEVE_PLUGIN_FUNCTION_NAME( const plgncore::data__ *Data )
{
	if ( Data == NULL )
		ERRFwk();

	if ( strcmp( Data->Version, PLGNCORE_SHARED_DATA_VERSION ) )
		ERRFwk();

	if ( Data->ControlValue != plgncore::data__::Control() )
		ERRFwk();

	if ( Data->Directory == NULL )
		ERRFwk();

	if ( !sclmisc::IsInitialized() )
		sclmisc::Initialize( Data->ERRError, Data->SCLError, Data->Directory );

	return sclplugin::SCLPLUGINRetrievePlugin();
}

void PLGNCORE_RELEASE_PLUGIN_FUNCTION_NAME( void *Callback )
{
	return sclplugin::SCLPLUGINReleasePlugin( Callback );
}

class sclpluginpersonnalization
{
public:
	sclpluginpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclpluginpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclpluginpersonnalization Tutor;
