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

#define WKAGENT__COMPILATION

#include "wkagent.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace wkagent;

void wkagent::agent___::ExecuteJavascript( const char *Script )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	ExecuteJavascript( Script, Buffer );
qRR
qRT
qRE
}


const char *wkagent::agent___::GetSelectValue(
	const char *Id,
	TOL_CBUFFER___ &Buffer  )
{
qRH
	TOL_CBUFFER___ LocalBuffer;
	str::string Name;
qRB
	Name.Init( "options[" );
	Name.Append( _C().Get( Id, "selectedIndex", LocalBuffer ) );
	Name.Append( "].value" );

	_C().Get( Id, Name.Convert( LocalBuffer ), Buffer );
qRR
qRT
qRE
	return Buffer;
}


void wkagent::agent___::SetChildren(
	const char *Id,
	const str::string_ &XML,
	const str::string_ &XSL )
{
qRH
	TOL_CBUFFER___ BXML, BXSL;
qRB
	SetChildren( Id, XML.Convert(BXML), XSL.Convert( BXSL ) );
qRR
qRT
qRE
}

void wkagent::agent___::SetChildren(
	const str::string_ &Id,
	const str::string_ &XML,
	const str::string_ &XSL )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	SetChildren( Id.Convert( Buffer), XML, XSL );
qRR
qRT
qRE
}

