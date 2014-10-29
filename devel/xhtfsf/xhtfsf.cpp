/*
	'xhtfsf.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtfsf' is part of the Epeios framework.

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

#define XHTFSF__COMPILATION

#include "xhtfsf.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtfsf;

void xhtfsf::GetContext(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer )
{
ERRProlog
	str::string Value;
ERRBegin
	Writer.PushTag( "BackendType" );

	Value.Init();
	switch ( xhtfbs::GetBackendType( Agent.GetSelectValue( "BackendTypeSelection", Value ) ) ) {
	case xhtfbs::btDaemon:
		Writer.PutValue( "Daemon" );
		break;
	case xhtfbs::btEmbedded:
		Writer.PutValue( "Embedded" );
		break;
	case xhtfbs::btPredefined:
		Writer.PutValue( "Predefined" );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtfsfpersonnalization
{
public:
	xhtfsfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtfsfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtfsfpersonnalization Tutor;
