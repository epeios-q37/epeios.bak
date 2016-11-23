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

#include "muaimf.h"
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
	BACKENDb;
	STUFFb;
	AUTHENTICATIONb;

	const str::dString &Username = Request.StringIn();
	const str::dString &Password = Request.StringIn();

	Account = Authentication.Authenticate( Username, Password, Backend.Language() );

	if ( Account != qNIL )
		Stuff.SetAccount( Account );

	Request.BooleanOut() = Account != qNIL;
qRR 
qRT
qRE
}

namespace get_agents_ {
	void Get(
		const muaagt::dAgents &Agents,
		fbltyp::dIds &Ids,
		fbltyp::dStrings &Names )
	{
		muaagt::sRow Row = Agents.First();

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
		muaagt::sRow Row,
		const muaagt::dAgents &Agents,
		str::dString &Name,
		str::dString &HostPort,
		str::dString &Username )
	{
		if ( !Agents.Exists( Row ) )
			REPORT( UnknownAgent );

		Agents.Names.Recall( Row, Name );

		const muaagt::dAgent &Agent = Agents.Core( Row );

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
		const muaagt::dAgents &Agents,
		muaagt::sRow Candidate )
	{
		muaagt::sRow Target = Agents.Search( Name );

		return ( Target == qNIL ) || ( Target == Candidate );
	}

	muaagt::sRow Update(
		muaagt::sRow Row,
		const str::dString &RawName,
		const str::dString &RawHostPort,
		const str::dString &RawUsername,
		bso::sBool PasswordIsSet,
		const str::dString &Password,
		muaagt::dAgents &Agents )
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

	muaagt::sRow Row = *Request.IdIn();

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

namespace get_fields_ {
	void Get( fbltyp::dId8s &Ids )
	{
		int i = muaimf::f_FirstOptional;

		while ( i < muaimf::f_amount )
			Ids.Add( i++ );

	}
}

DEC( GetFields, 1 )
{
qRH
qRB
	get_fields_::Get( Request.Id8sOut() );
qRR 
qRT
qRE
}

namespace get_fileds_infos_ {
	void Get_(
		const fblbkd::dId8s &Ids,
		fblbkd::dStrings &Labels,
		fblbkd::dStrings &Wording )
	{
		sdr::sRow Row = Ids.First();

		while ( Row != qNIL );
	}
}

namespace agent_{
	void InitAndAuthenticate(
		const muaagt::dAgent &Agent,
		csdbnc::rIODriver &Driver )
	{
		muaagt::InitAndAuthenticate( Agent, Driver );
	}

	void Quit( csdbnc::rIODriver &Driver )
	{
		muapo3::Quit( Driver );
	}
}

namespace get_mails_fields_ {
	namespace {
		void Get_(
			muabsc::sIndex Index,
			fdr::rIODriver &Driver,
			fblbkd::dStrings &Subjects )
		{
		qRH
			muapo3::hBody Body;
			muaimf::wFields Fields;
			str::wString Subject;
			muaimf::sFRow Row = qNIL;
		qRB
			muapo3::GetHeader( Index, Driver, Body );

			Fields.Init();
			muaimf::Fill(Body.GetDriver(), Fields );

			Subject.Init();
			Row = Fields.Search( muaimf::fSubject );

			if ( Row == qNIL )
				qRGnr();

			Subject.Init();
			Fields.GetBody( Row, Subject );
			Subjects.Add( Subject );
		qRR
		qRT
		qRE
		}

		void Get_(
			const muabsc::dIndexes &Indexes,
			fdr::rIODriver &Driver,
			fblbkd::dIds &Ids,
			fblbkd::dStrings &Subjects )
		{
			sdr::sRow Row = Indexes.First();

			while ( Row != qNIL ) {
				Ids.Add( Indexes( Row ) );
				Get_( Indexes( Row ), Driver, Subjects );

				Row = Indexes.Next( Row );
			}
		}

		void Get_(
			const muaagt::dAgent &Agent,
			fblbkd::dIds &Ids,
			fblbkd::dStrings &Subjects )
		{
		qRH
			csdbnc::rIODriver Driver;
			muabsc::wIndexes Indexes;
		qRB
			agent_::InitAndAuthenticate( Agent, Driver );

			Indexes.Init();

			muapo3::GetIndexes( Driver, Indexes );

			Get_( Indexes, Driver, Ids, Subjects );

			agent_::Quit( Driver );
		qRR
		qRT
		qRE
		}

		void Get_(
			const muaagt::dAgents &Agents,
			fblbkd::dIds &Ids,
			fblbkd::dStrings &Subjects )
		{
			muaagt::sRow Row = Agents.First();

			while ( Row != qNIL ) {
				Get_( Agents.Core( Row ), Ids, Subjects );

				Row = Agents.Next( Row );
			}
		}
	}

	void Get(
		const muaacc::dAccount &Account,
		fblbkd::dIds &Ids,
		fblbkd::dStrings &Subjects )
	{
		Get_( Account.Agents, Ids, Subjects );
	}
}

DEC( GetMailsFields, 1 )
{
qRH
	ACCOUNTh;
	muamel::wRows Wanted, Available;
qRB
	ACCOUNTb;

	Account.Update();

	tol::Init( Wanted );
	Account.GetAllMails( Wanted );

	fblbkd::dIds &Ids = Request.IdsOut();
	fblbkd::dStrings &Subjects = Request.StringsOut();

	tol::Init( Available );
	Account.GetFields( Wanted, Subjects, Available );


	fbltyp::Convert( Available, Ids );
qRR 
qRT
qRE
}

DEC( GetFolders, 1 ) 
{
qRH
	ACCOUNTh;
	muafld::wRows Rows;
qRB
	ACCOUNTb;

	muafld::sRow Row = *Request.IdIn();

	tol::Init( Rows );

	Account.GetFolders( Row, Rows );

	fbltyp::Convert(Rows, Request.IdsOut() );
qRR 
qRT
qRE
}

DEC( GetFoldersNames, 1 )
{
qRH
	ACCOUNTh;
	muafld::wRows Rows;
qRB
	ACCOUNTb;

	tol::Init( Rows );
	fbltyp::Convert( Request.IdsIn(), Rows );

	Account.GetFoldersNames( Rows, Request.StringsOut() );
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

	Backend.Add( D( GetFields, 1 ),
		fblbkd::cEnd,
			fblbkd::cId8s,	// Ids of the fields.
		fblbkd::cEnd );

	Backend.Add( D( GetMailsFields, 1 ),
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the mails.
			fblbkd::cStrings,	// Subjects of the mails.
		fblbkd::cEnd );

	Backend.Add( D( GetFolders, 1 ),
			fblbkd::cId,	// Folder.
		fblbkd::cEnd,
			fblbkd::cIds,	// Folders.
		fblbkd::cEnd );

	Backend.Add( D( GetFoldersNames, 1 ),
			fblbkd::cIds,		// Folders.
		fblbkd::cEnd,
			fblbkd::cStrings,	// Names,
		fblbkd::cEnd );


}

