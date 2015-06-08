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

#define XHTLOGIN__COMPILATION

#include "xhtlogin.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtlogin;

void xhtlogin::GetContent(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::backend_type__ GetBackendType_( xhtagent::agent___ &Agent )
{
	frdbse::backend_type__ BackendType = frdbse::bt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	BackendType = frdbse::GetBackendType( Agent.GetContent( BackendTypeId, Value ) );
qRR
qRT
qRE
	return BackendType;
}

void xhtlogin::GetContext(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer )
{
	Writer.PushTag( "BackendType" );

	Writer.PutValue( frdbse::GetLabel( GetBackendType_( Agent ) ) );

	Writer.PopTag();
}

frdbse::backend_type__ xhtlogin::GetBackendFeatures(
	xhtagent::agent___ &Agent,
	str::string_ &Feature )
{
	frdbse::backend_type__ Type = frdbse::bt_Undefined;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	switch ( Type = GetBackendType_( Agent ) ) {
	case frdbse::btDaemon:
		Feature.Append( Agent.GetContent( DaemonBackendId, Buffer ) );
		break;
	case frdbse::btEmbedded:
		Feature.Append( Agent.GetContent( EmbeddedBackendId, Buffer ) );
		break;
	case frdbse::btPredefined:
		Feature.Append( Agent.GetContent( PredefinedBackendId, Buffer ) );
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

void xhtlogin::DisplaySelectedEmbeddedBackendFileName(
	xhtagent::agent___ &Agent,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xhtcllbk::args Args;
	xhtcllbk::retriever__ Retriever;
qRB
	Args.Init();
	xhtcllbk::Split( str::string( Agent.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Agent.SetContent( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}
