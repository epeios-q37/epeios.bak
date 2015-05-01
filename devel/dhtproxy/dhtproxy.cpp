/*
	'dhtproxy.cpp' by Claude SIMON (http://zeusw.org/).

	'dhtproxy' is part of the Epeios framework.

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

#define DHTPROXY__COMPILATION

#include "dhtproxy.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace dhtproxy;

extern "C" typedef xhtcllbk::retrieve retrieve;

bso::bool__ dhtproxy::proxy___::Init(
	const str::string_ &ModuleFileName,
	const char *Identification )
{
	bso::bool__ Success = false;
ERRProlog
	xhtcllbk::shared_data__ Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Location.Init();
	Data.Init( DHTPROXYGetProxyCallback(), Identification, fnm::GetLocation( ModuleFileName, Location ).UTF8( Buffer ) );

	_Library.Init( ModuleFileName );

	retrieve *Retrieve = dlbrry::GetFunction<retrieve *>( E_STRING( XHTCLLBK_RETRIEVE_FUNCTION_NAME ), _Library );

	if ( Retrieve == NULL )
		ERRReturn;

    _Callback = Retrieve();

	if ( _Callback == NULL )
		ERRFwk();

	_Callback->Initialize( Data );

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

