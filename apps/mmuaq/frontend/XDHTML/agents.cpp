/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "agents.h"

#include "agent.h"
#include "core.h"
#include "registry.h"

#include "sclfrntnd.h"

namespace {

	qCDEF( char *, XSLAffix_, "Agents" );
	qCDEF( char *, AgentFrameId_, "Agent" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Rack().PushTag( "Agent" );

		Session.User.DumpAgentStatusAttribute( "Status", Rack );

		Rack().PopTag();
	qRR
	qRT
	qRE
	}

	void SetCasting_(
		const char *Id,
		core::rSession &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

		Session.FillElementCastings( Id, XML, XSL );
	qRR
	qRT
	qRE
	}

	static void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.DumpAgents( Rack );
	qRR
	qRT
	qRE
	}
}

void agents::SetLayout(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillElement( Id, XML, XSL );

	SetCasting_( Id, Session );

	agent::SetLayout( AgentFrameId_, Session );
qRR
qRT
qRE
}


void agents::SetAgentCasting( core::rSession &Session )
{
	agent::SetCasting( AgentFrameId_, Session );
}

#define AC( name ) BASE_AC( agents, name )

AC( DefineAgent )
{
	Session.User.DefineAgent();

	core::SetAgentsLayout( Session );
}

AC( SelectAgent )
{
	instc::sAgent Agent = instc::UndefinedAgent;

	Session.GetNumericalContent( Id, **Agent );

	Session.User.SelectAgent( Agent );

	core::SetAgentsLayout( Session );
}

AC( EditAgent )
{
	Session.User.EditAgent();

	core::SetAgentsLayout( Session );
}

AC( RemoveAgent )
{
	if ( Session.ConfirmT( "AgentSuppressionConfirmation" ) )
		Session.User.RemoveAgent();

	core::SetAgentsLayout( Session );
}

