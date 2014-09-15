/*
	'xhtfmn.cpp' by Claude SIMON (http://zeusw.org/).

	'xhtfmn' is part of the Epeios framework.

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

#define XHTFMN__COMPILATION

#include "xhtfmn.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtfmn;

void xhtfmn::event_handlers__::HandleProjectTypeSelection( xhtagent::agent___ &Agent )
{
ERRProlog
	TOL_CBUFFER___ Buffer1, Buffer2;
	str::string Script, Value;
ERRBegin
	Value.Init( Agent.GetSelectValue( "ProjectTypeSelection", Buffer1 ) );

	if ( Value == "PredefinedProject" )
		Agent.Show( "PredefinedProjects" );
	else
		Agent.Hide( "PredefinedProjects" );

	if ( Value == "UserProject" )
		Agent.Show( "UserProject" );
	else
		Agent.Hide( "UserProject" );
ERRErr
ERREnd
ERREpilog
}

void xhtfmn::event_handlers__::HandleSubmission( xml::writer_ &Writer )
{
}






/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xhtfmnpersonnalization
{
public:
	xhtfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xhtfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static xhtfmnpersonnalization Tutor;
