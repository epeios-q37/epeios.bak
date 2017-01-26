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

#include "muaagt.h"

#include "muaima.h"
#include "muapo3.h"

using namespace muaagt;

void muaagt::dAgents::Disable_( sRow Agent )
{
qRH
	wMetaData MetaData;
qRB
	MetaData.Init();

	MetaDatas_.Recall( Agent, MetaData );

	MetaData.SetDisabledState( true );

	MetaDatas_.Store( MetaData, Agent );
qRR
qRT
qRE
}

eProtocol muaagt::dAgents::InitAndAuthenticateIfEnabled(
	sRow AgentRow,
	csdbnc::rIODriver &Driver )
{
	eProtocol Protocol = p_Undefined;
qRH
	wAgent Agent;
	qCBUFFERr Buffer;
qRB
	Agent.Init( p_Undefined);
	Core_.Recall( AgentRow, Agent );

	if ( IsEnabled_( AgentRow ) ) {
	if ( Driver.Init(Agent.HostPort.Convert(Buffer), SCK_INFINITE, qRPU) ) {
		switch ( Agent.Protocol() ) {
		case muaagt::pPOP3:
			if ( muapo3::Authenticate( Agent.Username, Agent.Password, Driver, qRPU ) )
				Protocol = muaagt::pPOP3;
			break;
		case muaagt::pIMAP:
			if ( muaima::Connect( Authenticate( Agent.Username, Agent.Password, Driver, qRPU ) )

	}
qRR
qRT
qRE
	return Protocol;
}



