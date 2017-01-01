/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define XDHBSE__COMPILATION

#include "xdhbse.h"

using namespace xdhbse;

void xdhbse::prolog::GetContent(
	xdhagt::agent___ &Agent,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::project_type__ GetProjectType_( xdhagt::agent___ &Agent )
{
	frdbse::project_type__ ProjectType = frdbse::pt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	ProjectType = frdbse::GetProjectType( Agent.GetContent( prolog::ProjectTypeId, Value ) );
qRR
qRT
qRE
	return ProjectType;
}

void xdhbse::prolog::GetContext(
	xdhagt::agent___ &Agent,
	xml::writer_ &Writer)
{
	Writer.PushTag( "ProjectType ");

	Writer.PutValue( frdbse::GetLabel( GetProjectType_( Agent ) ) );

	Writer.PopTag();
}

frdbse::project_type__ xdhbse::prolog::GetProjectFeatures(
	xdhagt::agent___ &Agent,
	str::string_ &ProjectFeature )
{
	frdbse::project_type__ ProjectType = frdbse::pt_Undefined;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	switch ( ProjectType = GetProjectType_( Agent ) ) {
	case frdbse::ptNew:
		break;
	case frdbse::ptPredefined:
		ProjectFeature.Append( Agent.GetContent( PredefinedProjectId, Buffer ) );
		break;
	case frdbse::ptUser:
		ProjectFeature.Append( Agent.GetContent( UserProjectId, Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return ProjectType;
}

void xdhbse::prolog::DisplaySelectedProjectFileName(
	xdhagt::agent___ &Agent,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
qRB
	Args.Init();
	xdhcbk::Split( str::string( Agent.GetResult( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Agent.SetContent( UserProjectId, FileName );
qRR
qRT
qRE
}

void xdhbse::login::GetContent(
	xdhagt::agent___ &Agent,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::backend_type__ GetBackendType_( xdhagt::agent___ &Agent )
{
	frdbse::backend_type__ BackendType = frdbse::bt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	BackendType = frdbse::GetBackendType( Agent.GetContent( login::BackendTypeId, Value ) );
qRR
qRT
qRE
	return BackendType;
}

void xdhbse::login::GetContext(
	xdhagt::agent___ &Agent,
	xml::writer_ &Writer )
{
	Writer.PushTag( "BackendType" );

	Writer.PutValue( frdbse::GetLabel( GetBackendType_( Agent ) ) );

	Writer.PopTag();
}

frdbse::backend_type__ xdhbse::login::GetBackendFeatures(
	xdhagt::agent___ &Agent,
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

void xdhbse::login::DisplaySelectedEmbeddedBackendFileName(
	xdhagt::agent___ &Agent,
	const char *Id )
{
qRH
	TOL_CBUFFER___ Buffer;
	str::string FileName;
	xdhcbk::args Args;
	xdhcbk::retriever__ Retriever;
qRB
	Args.Init();
	xdhcbk::Split( str::string( Agent.GetResult( Id, Buffer ) ), Args );

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
