/*
	'wkagent.cpp' by Claude SIMON (http://zeusw.org/).

	'wkagent' is part of the Epeios framework.

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

#define WKAGENT__COMPILATION

#include "wkagent.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace wkagent;

void wkagent::agent___::ExecuteJavascript( const char *Script )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	ExecuteJavascript( Script, Buffer );
ERRErr
ERREnd
ERREpilog
}


const char *wkagent::agent___::GetSelectValue(
	const char *Id,
	TOL_CBUFFER___ &Buffer  )
{
ERRProlog
	TOL_CBUFFER___ LocalBuffer;
	str::string Name;
ERRBegin
	Name.Init( "options[" );
	Name.Append( _C().Get( Id, "selectedIndex", LocalBuffer ) );
	Name.Append( "].value" );

	_C().Get( Id, Name.Convert( LocalBuffer ), Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}


void wkagent::agent___::SetChildren(
	const char *Id,
	const str::string_ &XML,
	const str::string_ &XSL )
{
ERRProlog
	TOL_CBUFFER___ BXML, BXSL;
ERRBegin
	SetChildren( Id, XML.Convert(BXML), XSL.Convert( BXSL ) );
ERRErr
ERREnd
ERREpilog
}

void wkagent::agent___::SetChildren(
	const str::string_ &Id,
	const str::string_ &XML,
	const str::string_ &XSL )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	SetChildren( Id.Convert( Buffer), XML, XSL );
ERRErr
ERREnd
ERREpilog
}

