/*
	'xdhups.cpp' by Claude SIMON (http://zeusw.org/).

	'xdhups' is part of the Epeios framework.

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

#define XDHUPS__COMPILATION

#include "xdhups.h"

using namespace xdhups;

extern "C" typedef xdhcbk::retrieve retrieve;

bso::bool__ xdhups::agent___::Init(
	const str::string_ &ModuleFileName,
	const char *Identification )
{
	bso::bool__ Success = false;
ERRProlog
	xdhcbk::shared_data__ Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
ERRBegin
	Location.Init();
	Data.Init( XDHPXYGetUpstreamCallback(), Identification, fnm::GetLocation( ModuleFileName, Location ).UTF8( Buffer ) );

	_Library.Init( ModuleFileName );

	retrieve *Retrieve = dlbrry::GetFunction<retrieve *>( E_STRING( XDHCBK_RETRIEVE_FUNCTION_NAME ), _Library );

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
