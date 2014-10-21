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

static void EscapeQuotes_(
        const str::string_ &Source,
        str::string_ &Target )
{
	xhtcllbk::EscapeQuotes( Source, Target );
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

const str::string_ &xhtagent::agent___::GetSelectValue(
	const char *Id,
	str::string_ &Buffer )
{
ERRProlog
	TOL_CBUFFER___ CBuffer;
ERRBegin
	Buffer.Append( GetSelectValue( Id, CBuffer ) );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

void xhtagent::agent___::Alert( const str::string_ &Message )
{
ERRProlog
	str::string Script;
ERRBegin
	Script.Init( "alert(\"" );
	EscapeQuotes_( Message, Script );
	Script.Append("\");" );

	ExecuteJavascript( Script );
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
