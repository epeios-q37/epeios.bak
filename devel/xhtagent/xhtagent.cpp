/*
	'xhtagent.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtagent' is part of the Epeios framework.

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

#define XHTAGENT__COMPILATION

#include "xhtagent.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtagent;

void xhtagent::agent___::ExecuteJavascript( const char *Script )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	ExecuteJavascript( Script, Buffer );
ERRErr
ERREnd
ERREpilog
}


const char *xhtagent::agent___::GetSelectValue(
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


void xhtagent::agent___::SetChildren(
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

void xhtagent::agent___::SetChildren(
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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtagentpersonnalization
{
public:
	xhtagentpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtagentpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtagentpersonnalization Tutor;
