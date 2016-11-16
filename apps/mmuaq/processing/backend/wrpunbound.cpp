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

#include "wrpunbound.h"
#include "registry.h"
#include "dir.h"
#include "fnm.h"
#include "muainf.h"

#include "sclmisc.h"

using namespace wrpunbound;

using common::rStuff;

#define DEC( name, version )\
	static inline void name##_##version(\
		fblbkd::backend___ &BaseBackend,\
		fblbkd::rRequest &Request )

DEC( Test, 1 )
{
qRH
qRB
	REPORT( TestMessage );
qRR
qRT
qRE
}

DEC( Login, 1 )
{
qRH
	fbltyp::strings Labels;
	fbltyp::id8s Ids;
	muaacc::sRow Account = qNIL;
	bso::sBool New = false;
qRB
	STUFFb;
	AUTHENTICATIONb;

	const str::dString &Username = Request.StringIn();
	const str::dString &Password = Request.StringIn();

	Account = Authentication.Authenticate( Username, Password );

	if ( Account != qNIL )
		Stuff.SetAccount( Account );

	Request.BooleanOut() = Account != qNIL;
qRR 
qRT
qRE
}

namespace get_agents_ {
	void Get(
		const muaacc::dAgents &Agents,
		fbltyp::dIds &Ids,
		fbltyp::dStrings &Labels )
	{
		muaacc::sARow Row = Agents.First();

		while ( Row != qNIL ) {
			Ids.Append( *Row );
			Labels.Append( Agents.Labels( Row ) );

			Row = Agents.Next( Row );
		}
	}
}

DEC( GetAgents, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	fbltyp::dIds &Ids = Request.IdsOut();
	fbltyp::dStrings &Labels = Request.StringsOut();

	get_agents_::Get( Account.Agents, Ids, Labels );
qRR 
qRT
qRE
}

namespace get_agent_ {
	void Get(
		muaacc::sARow Row,
		const muaacc::dAgents &Agents,
		str::dString &Label,
		str::dString &HostPort,
		str::dString &Username )
	{
		if ( !Agents.Exists( Row ) )
			REPORT( UnknownAgent );

		Agents.Labels.Recall( Row, Label );

		const muaacc::dAgent &Agent = Agents.Agents( Row );

		HostPort = Agent.HostPort;
		Username = Agent.Username;
	}
}

DEC( GetAgent, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	const fbltyp::sId &Id = Request.IdIn();

	str::dString
		&Label = Request.StringOut(),
		&HostPort = Request.StringOut(),
		&Username = Request.StringOut();

	get_agent_::Get( *Id, Account.Agents, Label, HostPort, Username );
qRR 
qRT
qRE
}

namespace create_agent_ {
	muaacc::sARow Create(
		const str::dString &RawLabel,
		const str::dString &RawHostPort,
		const str::dString &Username,
		const str::dString &Password,
		muaacc::dAgents &Agents )
	{
		muaacc::sARow Row = qNIL;
	qRH
		str::wString Label, HostPort;
	qRB
		Label.Init( RawLabel );
		HostPort.Init( RawHostPort );

		Label.StripCharacter( ' ' );

		if ( Label.Amount() == 0 )
			REPORT( AgentNameCanNotBeEmpty );

		HostPort.StripCharacter( ' ' );

		if ( Agents.Search( Label ) != qNIL )
			REPORT( AgentWithSuchNameExists, Label );

		Row = Agents.New( Label, HostPort, Username, Password );
	qRR
	qRT
	qRE
		return Row;
	}
}

DEC( CreateAgent, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	const str::dString
		&Label = Request.StringIn(),
		&HostPort = Request.StringIn(),
		&Username = Request.StringIn(),
		&Password = Request.StringIn();

	Request.IdOut() = *create_agent_::Create( Label, HostPort, Username, Password, Account.Agents );
qRR 
qRT
qRE
}

#define D( name, version )	MUAINF_UC_SHORT #name "_" #version, ::name##_##version

void wrpunbound::Inform( fblbkd::backend___ &Backend )
{
	Backend.Add( D( Test, 1 ),
		fblbkd::cEnd,
		fblbkd::cEnd );

	Backend.Add( D( Login, 1 ),
			fblbkd::cString,	// Username.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cBoolean,	// Success.
		fblbkd::cEnd );

	Backend.Add( D( GetAgents, 1 ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids.
			fblbkd::cStrings,	// Labels.
		fblbkd::cEnd );

	Backend.Add( D( GetAgent, 1 ),
			fblbkd::cId,		// Id.
		fblbkd::cEnd,
			fblbkd::cString,	// Label.
			fblbkd::cString,	// HostPort.
			fblbkd::cString,	// Username.
		fblbkd::cEnd );

	Backend.Add(D( CreateAgent, 1 ),
			fblbkd::cString,	// Label.
			fblbkd::cString,	// HostPort.
			fblbkd::cString,	// Username.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cId,		// Id.
		fblbkd::cEnd );

}

