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

#include "muainf.h"
#include "muapo3.h"

#include "sclmisc.h"

#include "csdbnc.h"
#include "dir.h"
#include "fnm.h"

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
	fbltyp::strings Names;
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
		fbltyp::dStrings &Names )
	{
		muaacc::sARow Row = Agents.First();

		while ( Row != qNIL ) {
			Ids.Append( *Row );
			Names.Append( Agents.Names( Row ) );

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
	fbltyp::dStrings &Names = Request.StringsOut();

	get_agents_::Get( Account.Agents, Ids, Names );
qRR 
qRT
qRE
}

namespace get_agent_ {
	void Get(
		muaacc::sARow Row,
		const muaacc::dAgents &Agents,
		str::dString &Name,
		str::dString &HostPort,
		str::dString &Username )
	{
		if ( !Agents.Exists( Row ) )
			REPORT( UnknownAgent );

		Agents.Names.Recall( Row, Name );

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
		&Name = Request.StringOut(),
		&HostPort = Request.StringOut(),
		&Username = Request.StringOut();

	get_agent_::Get( *Id, Account.Agents, Name, HostPort, Username );
qRR 
qRT
qRE
}

namespace update_agent_ {
	inline bso::sBool CheckName_(
		const str::dString &Name,
		const muaacc::dAgents &Agents,
		muaacc::sARow Candidate )
	{
		muaacc::sARow Target = Agents.Search( Name );

		return ( Target == qNIL ) || ( Target == Candidate );
	}

	muaacc::sARow Update(
		muaacc::sARow Row,
		const str::dString &RawName,
		const str::dString &RawHostPort,
		const str::dString &RawUsername,
		bso::sBool PasswordIsSet,
		const str::dString &Password,
		muaacc::dAgents &Agents )
	{
	qRH
		str::wString Name, HostPort, Username;
	qRB
		Name.Init( RawName );
		HostPort.Init( RawHostPort );
		Username.Init( RawUsername );

		Name.StripCharacter( ' ' );
		if ( Name.Amount() == 0 )
			REPORT( AgentNameCanNotBeEmpty );

		HostPort.StripCharacter( ' ' );
		if ( HostPort.Amount() == 0 )
			REPORT( HostPortCanNotBeEmpty );

		Username.StripCharacter( ' ' );
		if ( Username.Amount() == 0 )
			REPORT( UsernameCanNotBeEmpty );

		if ( HostPort.Search(':') == qNIL )
			HostPort.Append( ":110" );

		if ( Row == qNIL ) {
			if ( !PasswordIsSet )
				qRGnr();

			if ( !CheckName_( Name, Agents, qNIL ) )
				REPORT( AgentWithSuchNameExists, Name );

			Row = Agents.New( Name, HostPort, Username, Password );
		} else if ( !CheckName_( Name, Agents, Row ) )
				REPORT( AgentWithSuchNameExists, Name );
		else if ( PasswordIsSet )
			Agents.Update( Row, Name, HostPort, Username );
		else
			Agents.Update( Row, Name, HostPort, Username, Password );
	qRR
	qRT
	qRE
		return Row;
	}
}

DEC( UpdateAgent, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	muaacc::sARow Row = *Request.IdIn();

	const str::dString
		&Name = Request.StringIn(),
		&HostPort = Request.StringIn(),
		&Username = Request.StringIn();

	bso::sBool PasswordIsSet = Request.BooleanIn();

	const str::dString &Password = Request.StringIn();

	Request.IdOut() = *update_agent_::Update( Row, Name, HostPort, Username, PasswordIsSet, Password, Account.Agents );
qRR 
qRT
qRE
}

namespace get_mails_ {
	void GetMail(
		const muaacc::dAgent &Agent,
		str::wStrings &Mails )
	{
	qRH
		csdbnc::rIODriver Driver;
		qCBUFFERr Buffer;
		muapo3::hBody Body;
	qRB
		Driver.Init( Agent.HostPort.Convert( Buffer ), SCK_INFINITE, err::h_Default );

		muapo3::Retrieve( 0, Driver, true, Body );
	qRR
	qRT
	qRE
	}
}

DEC( GetMails, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;
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
			fblbkd::cStrings,	// Names.
		fblbkd::cEnd );

	Backend.Add( D( GetAgent, 1 ),
			fblbkd::cId,		// Id.
		fblbkd::cEnd,
			fblbkd::cString,	// Name.
			fblbkd::cString,	// HostPort.
			fblbkd::cString,	// Username.
		fblbkd::cEnd );

	Backend.Add(D( UpdateAgent, 1 ),
			fblbkd::cId,		// If of the agent. New one is created if undefined.
			fblbkd::cString,	// Name.
			fblbkd::cString,	// HostPort.
			fblbkd::cString,	// Username.
			fblbkd::cBoolean,	// 'true' if following password is set.
			fblbkd::cString,	// Password.
		fblbkd::cEnd,
			fblbkd::cId,		// Id.
		fblbkd::cEnd );

}

