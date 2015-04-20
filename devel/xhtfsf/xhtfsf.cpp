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

void xhtfsf::GetContent(
	xhtagent::agent_core___ &Agent,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::backend_type__ GetBackendType_( xhtagent::agent_core___ &Agent )
{
	frdbse::backend_type__ BackendType = frdbse::bt_Undefined;
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	BackendType = frdbse::GetBackendType( Agent.GetSelectValue( BackendTypeId, Value ) );
ERRErr
ERREnd
ERREpilog
	return BackendType;
}

void xhtfsf::GetContext(
	xhtagent::agent_core___ &Agent,
	xml::writer_ &Writer )
{
	Writer.PushTag( "BackendType" );

	Writer.PutValue( frdbse::GetLabel( GetBackendType_( Agent ) ) );

	Writer.PopTag();
}

frdbse::backend_type__ xhtfsf::GetBackendFeatures(
	xhtagent::agent_core___ &Agent,
	str::string_ &Feature )
{
	frdbse::backend_type__ Type = frdbse::bt_Undefined;
ERRProlog
	TOL_CBUFFER___ Buffer;
ERRBegin
	switch ( Type = GetBackendType_( Agent ) ) {
	case frdbse::btDaemon:
		Feature.Append( Agent.GetValue( DaemonBackendId, Buffer ) );
		break;
	case frdbse::btEmbedded:
		Feature.Append( Agent.GetValue( EmbeddedBackendId, Buffer ) );
		break;
	case frdbse::btPredefined:
		Feature.Append( Agent.GetValue( PredefinedBackendId, Buffer ) );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Type;
}

void xhtfsf::DisplaySelectedEmbeddedBackendFileName(
	xhtagent::agent_core___ &Agent,
	const char *Id )
{
ERRProlog
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xhtcllbk::params Params;
	xhtcllbk::retriever__ Retriever;
ERRBegin
	Params.Init();
	xhtcllbk::Split( str::string( Agent.GetAttribute( Id, xhtcllbk::ResultAttribute, Buffer ) ), Params );

	Retriever.Init( Params );

	FileName.Init();
	Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Agent.SetValue( EmbeddedBackendId, FileName );
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
