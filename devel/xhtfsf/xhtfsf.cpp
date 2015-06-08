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
qRH
	str::string Value;
qRB
	Value.Init();
	BackendType = frdbse::GetBackendType( Agent.GetSelectValue( BackendTypeId, Value ) );
qRR
qRT
qRE
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
qRH
	TOL_CBUFFER___ Buffer;
qRB
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
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Type;
}

void xhtfsf::DisplaySelectedEmbeddedBackendFileName(
	xhtagent::agent_core___ &Agent,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xhtcllbk::params Params;
	xhtcllbk::retriever__ Retriever;
qRB
	Params.Init();
	xhtcllbk::Split( str::string( Agent.GetAttribute( Id, xhtcllbk::ResultAttribute, Buffer ) ), Params );

	Retriever.Init( Params );

	FileName.Init();
	Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Agent.SetValue( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}

