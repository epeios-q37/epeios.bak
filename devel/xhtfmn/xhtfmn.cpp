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

#include "frdkrn.h"

using namespace xhtfmn;

E_CDEF( char *, PredefinedProject, "PredefinedProject" );
E_CDEF( char *, UserProject, "UserProject" );


static xhtfbs::project_type__ GetProjectType_( xhtagent::agent___ &Agent )
{
	xhtfbs::project_type__ ProjectType = xhtfbs::pt_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	ProjectType = xhtfbs::GetProjectType( Agent.GetSelectValue( "ProjectType", Value ) );
ERRErr
ERREnd
ERREpilog
	return ProjectType;
}

void xhtfmn::SetAccessibility( xhtagent::agent___ &Agent )
{
ERRProlog
	bso::bool__ Predefined = false, User = false;
ERRBegin
	switch ( GetProjectType_( Agent ) ) {
	case xhtfbs::ptNew:
		break;
	case xhtfbs::ptPredefined:
		Predefined = true;
		break;
	case xhtfbs::ptUser:
		User = true;
		break;
	default:
		ERRFwk();
		break;
	}

	Agent.Show( PredefinedProject, Predefined );
	Agent.Show( UserProject, User );
ERRErr
ERREnd
ERREpilog
}

static const str::string_ &GetPredefinedProject_(
	xhtagent::agent___ &Agent,
	str::string &Buffer )
{
	return Agent.GetSelectValue( PredefinedProject, Buffer );
}

xhtfbs::project_type__ xhtfmn::GetProjectFeatures(
	xhtagent::agent___ &Agent,
	str::string_ &ProjectFeature )
{
	xhtfbs::project_type__ ProjectType = xhtfbs::pt_Undefined;
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	switch ( GetProjectType_( Agent ) ) {
	case xhtfbs::ptNew:
		break;
	case xhtfbs::ptPredefined:
		Agent.GetSelectValue( PredefinedProject, ProjectFeature );
		break;
	case xhtfbs::ptUser:
		ProjectFeature.Append( Agent.Get( UserProject, "value", Buffer ) );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return ProjectType;
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
