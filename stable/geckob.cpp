/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define GECKOB__COMPILATION

#include "geckob.h"

#include "mtx.h"

using namespace geckob;

static mtx::handler__ Mutex_ = MTX_INVALID_HANDLER;
static geckoo::steering_callback__ *LoneSteering_ = NULL;

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#elif defined ( CPE_POSIX )
# define FUNCTION_SPEC
#else
# error
#endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( GECKOO_CREATE_STEERING_FUNCTION_NAME, geckoo::create_steering );
DEF( GECKOO_RETRIEVE_STEERING_FUNCTION_NAME, geckoo::retrieve_steering );
DEF( GECKOO_DELETE_STEERING_FUNCTION_NAME, geckoo::delete_steering );

#if 0
#ifdef CPE__WIN

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

geckoo::steering_callback__ *GECKOOCreateSteering( geckoo::shared_data__ *Data )
{
	if ( Data == NULL )
		ERRPrm();

	if ( strcmp( Data->Version, GECKOO_OVERLAPPING_VERSION ) )
		ERRChk();

	if ( Data->Control != Data->ControlComputing() )
		ERRChk();

	return geckob::GECKOBCreateSteering( Data );
}

geckoo::steering_callback__ *GECKOORetrieveSteering( void )
{
	geckoo::steering_callback__ *Steering = NULL;

	if ( !mtx::IsLocked( Mutex_ ) )
		ERRFwk();

	if ( LoneSteering_ == NULL )
		ERRFwk();

	Steering = LoneSteering_;

	LoneSteering_ = NULL;

	mtx::Unlock( Mutex_ );


//	geckob::GECKOBRetrieveSteering();

	return Steering;
}

void GECKOODeleteSteering( geckoo::steering_callback__ *Steering )
{
	geckob::GECKOBDeleteSteering( Steering );
}

void geckob::SetSteering( geckoo::steering_callback__ &Steering )
{
	mtx::Lock( Mutex_ );

	if ( LoneSteering_ != NULL )
		ERRFwk();

	LoneSteering_ = &Steering;
}

Q37_GCTOR( geckob )
{
	Mutex_ = mtx::Create();
}

Q37_GDTOR( geckob )
{
	if ( Mutex_ != MTX_INVALID_HANDLER )
		mtx::Delete( Mutex_, true );
}
