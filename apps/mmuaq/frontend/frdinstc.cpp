/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "frdinstc.h"

#include "flf.h"
#include "fnm.h"

using namespace frdinstc;

using fbltyp::wString;
using fbltyp::wStrings;

namespace {
	void DumpAgents_(
		const dAgents &Ids,
		dStrings &Labels,
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Ids.First();

		while ( Row != qNIL ) {
			Writer.PushTag( "Agent ");
			Writer.PutAttribute( "id", *Ids( Row ) );
			Writer.PutValue( Labels( Row ) );
			Writer.PopTag();

			Row = Ids.Next( Row );
		}
	}
}

void frdinstc::rUser::DumpAgents(
	sAgent Agent,
	xml::dWriter &Writer )
{
qRH
	wAgents Agents;
	wStrings Labels;
qRB
	Core_.GetAgents( Agents, Labels );

	if ( Agents.Amount() != Labels.Amount() )
		qRGnr();

	Writer.PushTag( "Agents" );
	Writer.PutAttribute( "Amount", Agents.Amount() );

	Writer.PutAttribute( "Current", **Agent, **UndefinedAgent );

	DumpAgents_( Agents, Labels, Writer );

	Writer.PopTag();
qRR
qRT
qRE
}

void frdinstc::rUser::DumpAgent(
	sAgent Agent,
	xml::dWriter &Writer )
{
qRH
	wString Label, HostPort, Username; 
qRB
	if ( Agent == UndefinedAgent )
		qRGnr();

	tol::Init( Label, HostPort, Username );
	Core_.GetAgent( Agent, Label, HostPort, Username );

	Writer.PushTag( "Agent" );

	Writer.PutAttribute( "Label", Label );
	Writer.PutAttribute( "HostPort", HostPort );
	Writer.PutAttribute( "Username", Username );

	Writer.PopTag();
qRR
qRT
qRE
}

const str::dString &frdinstc::rUser::GetAgentStatus( str::dString &Status )
{
	if ( CurrentAgent_ != frdinstc::UndefinedAgent ) {
		if ( AgentEdition_  )
			Status = "Edit";
		else
			Status = "Show";
	} else if ( AgentEdition_ )
		Status = "Create";

	return Status;
}

void frdinstc::rUser::PutAgentStatusAttribute(
	const char *Name,
	xml::dWriter &Writer )
{
qRH
	str::wString Status;
qRB
	Status.Init();

	GetAgentStatus( Status );

	if ( Status.Amount() != 0 )
		Writer.PutAttribute( Name, Status );
qRR
qRT
qRE
}
