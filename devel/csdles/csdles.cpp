/*
	'csdles.cpp' by Claude SIMON (http://zeusw.org/).

	'csdles' is part of the Epeios framework.

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

#define CSDLES__COMPILATION

#include "csdles.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace csdles;

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME, csdleo::retrieve_callback );
// DEF( CSDLEO_RELEASE_CALLBACK_FUNCTION_NAME, csdleo::release_callback );

static inline void DoNothing_( void )
{}

csdleo::callback__ &CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME( void )
{
	return csdles::CSDLESRetrieveCallback();
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdlespersonnalization
{
public:
	csdlespersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdlespersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static csdlespersonnalization Tutor;
