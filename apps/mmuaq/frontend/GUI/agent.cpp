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

#include "agent.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {

	qCDEF( char *, XSLAffix_, "Agent" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.DumpAgentStatusAttribute( "Status", Rack );

		Rack().PutAttribute("Password", Session.User.Agent().ShowPassword ? "Visible" : "Hidden" );
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

		Session.User.DumpCurrentAgent( Rack );
	qRR
	qRT
	qRE
	}
}

void agent::SetLayout(
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
qRR
qRT
qRE
}

void agent::SetCasting(
	const char *Id,
	core::rSession &Session )
{
	SetCasting_( Id, Session );
}

#define AC( name ) BASE_AC( agent, name )


AC( SubmitAgent )
{
qRH
	str::wString Name, Protocol, HostPort, Username, Password;
qRB
	tol::Init( Name, Protocol, HostPort, Username, Password );

	Session.GetContent( "AgentName", Name );
	Session.GetContent( "AgentProtocol", Protocol );
	Session.GetContent( "AgentHostPort", HostPort );
	Session.GetContent( "AgentUsername", Username );
	Session.GetContent( "AgentPassword", Password );

	Session.User.UpdateAgent( Name, frdfrntnd::GetProtocol( Protocol ), HostPort, Username, Session.GetBooleanContent( "AgentPasswordToggle" ), Password );

	core::SetAgentsLayout( Session );
qRR
qRT
qRE
}

AC( DiscardAgent )
{
	Session.User.DiscardAgent();

	core::SetAgentsLayout( Session );
}

AC( ToggleAgentPassword )
{
	Session.User.SetAgentShowPassword( Session.GetBooleanContent( "AgentPasswordToggle" ) );

	core::SetAgentCasting( Session );
}

