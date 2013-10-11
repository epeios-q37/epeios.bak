/*
	'geckob' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'geckob' header file ('geckob.h').
	Copyright (C) 20112004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: geckob.cpp,v 1.11 2013/04/09 17:57:31 csimon Exp $

#define GECKOB__COMPILATION

#include "geckob.h"

class geckobtutor
: public ttr_tutor
{
public:
	geckobtutor( void )
	: ttr_tutor( GECKOB_NAME )
	{
#ifdef GECKOB_DBG
		Version = GECKOB_VERSION "\b\bD $";
#else
		Version = GECKOB_VERSION;
#endif
		Owner = GECKOB_OWNER;
		Date = "$Date: 2013/04/09 17:57:31 $";
	}
	virtual ~geckobtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mtx.h"

using namespace geckob;

static mtx::mutex_handler__ Mutex_ = MTX_INVALID_HANDLER;
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

	if ( !mtx::IsOwner( Mutex_ ) )
		ERRFwk();

	if ( LoneSteering_ == NULL )
		ERRFwk();

	Steering = LoneSteering_;

	LoneSteering_ = NULL;

//	geckob::GECKOBRetrieveSteering();

	return Steering;
}

void GECKOODeleteSteering( geckoo::steering_callback__ *Steering )
{
	geckob::GECKOBDeleteSteering( Steering );
}

void geckob::SetSteering( geckoo::steering_callback__ &Steering )
{
	bso::bool__ FreshLock = false;

	mtx::Lock( Mutex_ );

	if ( LoneSteering_ != NULL )
		ERRFwk();

	LoneSteering_ = &Steering;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class geckobpersonnalization
: public geckobtutor
{
public:
	geckobpersonnalization( void )
	{
		Mutex_ = mtx::Create( mtx::mProtecting );
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~geckobpersonnalization( void )
	{
		if ( Mutex_ != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex_, true );
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static geckobpersonnalization Tutor;

ttr_tutor &GECKOBTutor = Tutor;
