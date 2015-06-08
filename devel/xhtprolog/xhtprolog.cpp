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

#define XHTPROLOG__COMPILATION

#include "xhtprolog.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace xhtprolog;

void xhtprolog::GetContent(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer)
{
	// Rien  fournir.
}

static frdbse::project_type__ GetProjectType_( xhtagent::agent___ &Agent )
{
	frdbse::project_type__ ProjectType = frdbse::pt_Undefined;
qRH
	str::string Value;
qRB
	Value.Init();
	ProjectType = frdbse::GetProjectType( Agent.GetContent( ProjectTypeId, Value ) );
qRR
qRT
qRE
	return ProjectType;
}

void xhtprolog::GetContext(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer)
{
	Writer.PushTag( "ProjectType ");

	Writer.PutValue( frdbse::GetLabel( GetProjectType_( Agent ) ) );

	Writer.PopTag();
}

frdbse::project_type__ xhtprolog::GetProjectFeatures(
	xhtagent::agent___ &Agent,
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

void xhtprolog::DisplaySelectedProjectFileName(
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
		Agent.SetContent( UserProjectId, FileName );
qRR
qRT
qRE
}
