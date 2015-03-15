/*
	'scldaemon.cpp' by Claude SIMON (http://zeusw.org/).

	'scldaemon' is part of the Epeios framework.

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

#define SCLDAEMON__COMPILATION

#include "scldaemon.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace scldaemon;

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"

#include "lcl.h"

#include "csdleo.h"
#include "csdles.h"

#include "fnm.h"

static bso::bool__ IsInitialized_ = false;

csdleo::callback__ *csdles::CSDLESRetrieveCallback( csdleo::data__ *Data )
{
	csdleo::callback__ *Callback = NULL;
ERRProlog
	fnm::name___ Directory;
	TOL_CBUFFER___ Buffer;
	str::string Error;
ERRBegin
	if ( Data == NULL )
		ERRPrm();
		
	if ( !IsInitialized_ && ( Data->Context == csdleo::cRegular ) )	{

		// Does not work when placed in 'global_cdtor'.
		Directory.Init();
		fnm::GetLocation( Data->LibraryLocationAndName, Directory );
		sclmisc::Initialize( Data->ERRError, (sclerror::error___ *)Data->UP, Directory.UTF8( Buffer ) );
		IsInitialized_ = true;
	}

/*
	switch ( Data->Mode ) {
	case csdleo::mEmbedded:
		Mode = fblbur::mEmbedded;
		break;
	case csdleo::mRemote:
		Mode = fblbur::mRemote;
		break;
	default:
		ERRFwk();
		break;
	}
*/

	Callback = SCLDAEMONNewCallback( Data->Mode, Data->Context );
ERRErr
ERREnd
ERREpilog
	return Callback;
}

void csdles::CSDLESReleaseCallback( csdleo::callback__ *Callback )
{
	if ( Callback != NULL  )
		delete Callback;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scldaemonpersonnalization
{
public:
	scldaemonpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scldaemonpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static scldaemonpersonnalization Tutor;
