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

namespace {
	bso::bool__ IsInitialized_ = false;

	typedef csdleo::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void CSDLEOInitialize(
			const csdleo::shared_data__ *Data,
			... ) override
		{
		ERRProlog
			fnm::name___ Directory;
			TOL_CBUFFER___ Buffer;
			str::string Error;
		ERRBegin
			if ( Data == NULL )
				ERRPrm();

			if ( IsInitialized_ )
				ERRPrm();
		
			if ( !IsInitialized_ && ( Data->Context == csdleo::cRegular ) )	{

				// Does not work when placed in 'global_cdtor'.
				Directory.Init();
				fnm::GetLocation( Data->LibraryLocationAndName, Directory );
				sclmisc::Initialize( Data->ERRError, (sclerror::error___ *)Data->UP, *Data->CIO, Directory.UTF8( Buffer ) );
				IsInitialized_ = true;
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		virtual csdscb::callback__ *CSDLEORetrieveCallback(
			csdleo::context__ Context,
			csdleo::mode__ Mode )  override
		{
			return SCLDAEMONGetCallback( Context, Mode );
		}
	} _Callback;
}

csdleo::callback__ &csdles::CSDLESRetrieveCallback( void )
{
	_Callback.Init();

	return _Callback;
}
