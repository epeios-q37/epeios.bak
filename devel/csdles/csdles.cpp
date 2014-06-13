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

static bch::E_BUNCH( csdleo::callback__ *) Callbacks_;

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( CSDLEO_RETRIEVE_CALLBACK_FUNCTION_NAME, csdleo::retrieve_callback );
DEF( CSDLEO_RELEASE_CALLBACK_FUNCTION_NAME, csdleo::release_callback );

#if 0
#ifdef CPE_WIN

#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
    }
    return TRUE;
}
#endif
#endif

static void Terminate_( void )
{
	sdr::row__ Row = Callbacks_.First();
	csdleo::callback__ *Callback = NULL;

	while ( Row != E_NIL ) {

		Callback = Callbacks_( Row );

		if ( Callback != NULL )
			delete Callback;

		Callbacks_.Store( NULL, Row );

		Row = Callbacks_.Next( Row );
	}
}

static void ExitFunction_( void )
{
	Terminate_();
}

csdleo::callback__ *CSDLEORetrieveCallback( csdleo::shared_data__ *Data )
{
	csdleo::callback__ *Callback = NULL;
ERRFProlog
	flw::standalone_oflow__<> OFlow;
	txf::text_oflow__ TOFlow;
	err::buffer__ Buffer;
ERRFBegin
	atexit( ExitFunction_ );

	if ( Data == NULL )
		ERRPrm();

	if ( strcmp( Data->Version, CSDLEO_SHARED_DATA_VERSION ) )
		ERRChk();

	if ( Data->Control != Data->ControlComputing() )
		ERRChk();

	Callback = csdles::CSDLESRetrieveCallback( Data );

	if ( Callback != NULL )
		Callbacks_.Append( Callback );
ERRFErr
	OFlow.Init( *Data->CErr, FLW_AMOUNT_MAX );
	TOFlow.Init( OFlow );
	TOFlow << err::Message( Buffer );

	ERRRst();
ERRFEnd
ERRFEpilog
	return Callback;
}

void CSDLEOReleaseCallback( csdleo::callback__ *Callback )
{
	sdr::row__ Row = Callbacks_.Search( Callback );

	if ( Row != E_NIL ) {
		csdles::CSDLESReleaseCallback( Callback );
		Callbacks_.Store( NULL, Row );
	}
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
		Callbacks_.Init();
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
