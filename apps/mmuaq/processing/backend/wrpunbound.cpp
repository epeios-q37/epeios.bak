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

#define M( message )	E_CDEF( char *, message, #message )

namespace message_ {
	M( TestMessage );
	M( NotLoggedIn );
	M( AgentWithSuchNameExists );
	M( AgentNameCanNotBeEmpty );
	M( AgentNameCanNotBeLongerAs );
	M( HostPortCanNotBeEmpty );
	M( HostPortCanNotBeLongerAs );
	M( UsernameCanNotBeEmpty );
	M( UsernameCanNotBeLongerAs );
	M( PasswordCanNotBeLongerAs );
	M( FolderNameCanNotBeEmpty );
	M( FolderNameCanNotBeLongerAs );
	M( UnknownAgent );
	M( UnknownFolder );
	M( FolderNotMoveable );
	M( FolderNotRenameable );
	M( FolderWithSameNameAlreadyExistsInThisFolder );
	M( FolderCannotBeMovedToDescendant );
/*
	M(  );
*/
}

#undef M

using common::rStuff;

#define DEC( name, version )\
	static inline void name##_##version(\
		fblbkd::backend___ &BaseBackend,\
		fblbkd::rRequest &Request )

namespace shared_ {
	qCDEF( bso::sUInt, DefaultLengthLimit, 50 );

	const str::dString &Normalize(
		str::dString &Value,
		const char *EmptyMessage,
		const char *LengthMessage,
		rgstry::rEntry &Entry )
	{
	qRH
	qRB
		Value.StripCharacter( ' ' );

		if ( Value.Amount() == 0 )
			sclmisc::ReportAndAbort( EmptyMessage );

		if ( Value.Amount() > sclmisc::OGetUInt( Entry, DefaultLengthLimit ) )
			sclmisc::ReportAndAbort( LengthMessage, DefaultLengthLimit );
	qRR
	qRT
	qRE
		return Value;
	}
}

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
			Agents.GetName( Row, Names( str::NewAndInit( Names ) ) );

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

	get_agents_::Get( Account.Agents(), Ids, Names );
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

		Agents.GetName( Row, Name );

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

	get_agent_::Get( *Id, Account.Agents(), Name, HostPort, Username );
qRR 
qRT
qRE
}

namespace update_agent_ {
	namespace {
		inline bso::sBool CheckName_(
			const str::dString &Name,
			const muaagt::dAgents &Agents,
			muaagt::sRow Candidate )
		{
			muaagt::sRow Target = Agents.Search( Name );

			return ( Target == qNIL ) || ( Target == Candidate );
		}
	}

#define N( name ) shared_::Normalize( name, message_::name##CanNotBeEmpty, message_::name##CanNotBeLongerAs, registry::definition::limitation::name##Length )

	muaagt::sRow Update(
		muaagt::sRow Row,
		const str::dString &RawAgentName,
		const str::dString &RawHostPort,
		const str::dString &RawUsername,
		bso::sBool PasswordIsSet,
		const str::dString &Password,
		muaacc::dAccount &Account )
	{
	qRH
		str::wString AgentName, HostPort, Username;
	qRB
		AgentName.Init( RawAgentName );
		HostPort.Init( RawHostPort );
		Username.Init( RawUsername );

		N( AgentName );
		N( HostPort );
		N( Username );

		if ( Password.Amount() > shared_::DefaultLengthLimit )
			REPORT( PasswordCanNotBeLongerAs, shared_::DefaultLengthLimit );

		if ( HostPort.Search(':') == qNIL )
			HostPort.Append( ":110" );

		if ( Row == qNIL ) {
			if ( !PasswordIsSet )
				qRGnr();

			if ( !CheckName_( AgentName, Account.Agents(), qNIL ) )
				REPORT( AgentWithSuchNameExists, AgentName );

			Row = Account.NewAgent( AgentName, HostPort, Username, Password );
		} else if ( !CheckName_( AgentName, Account.Agents(), Row ) )
				REPORT( AgentWithSuchNameExists, AgentName );
		else if ( PasswordIsSet )
			Account.UpdateAgent( Row, AgentName, HostPort, Username );
		else
			Account.UpdateAgent( Row, AgentName, HostPort, Username, Password );
	qRR
	qRT
	qRE
		return Row;
	}

# undef N
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

	Request.IdOut() = *update_agent_::Update( Row, Name, HostPort, Username, PasswordIsSet, Password, Account );
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

DEC( GetRootAndInboxFolders, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	Request.IdOut() = *Account.Directory().Root();
	Request.IdOut() = *Account.Directory().Inbox();
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

	if ( ( Row != qNIL ) && !Account.Directory().Exists( Row ) )
		REPORT( UnknownFolder );

	tol::Init( Rows );

	Account.Directory().Folders().GetChildren( Row, Rows );

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

	Account.Directory().Folders().GetNames( Rows, Request.StringsOut() );
qRR 
qRT
qRE
}

DEC( CreateFolder, 1 )
{
qRH
	ACCOUNTh;
	str::wString Name;
qRB
	ACCOUNTb;

	muafld::sRow Parent = *Request.IdIn();

	if ( !Account.Directory().Exists( Parent ) )
		REPORT( UnknownFolder );

	Name.Init( Request.StringIn() );

	shared_::Normalize( Name, message_::FolderNameCanNotBeEmpty, message_::FolderNameCanNotBeLongerAs, registry::definition::limitation::FolderNameLength );

	if ( Account.Directory().Folders().Search( Parent, Name ) != qNIL )
		REPORT( FolderWithSameNameAlreadyExistsInThisFolder );
	else
		Request.IdOut() = *Account.CreateFolder( Name, Parent );
qRR 
qRT
qRE
}

DEC( RenameFolder, 1 )
{
qRH
	ACCOUNTh;
	str::wString NewName, CurrentName;
qRB
	ACCOUNTb;

	muafld::sRow Folder = *Request.IdIn();

	if ( !Account.Directory().Exists( Folder ) )
		REPORT( UnknownFolder );

	if ( !Account.Directory().IsRenameable( Folder ) )
		REPORT( FolderNotRenameable );

	NewName.Init( Request.StringIn() );

	shared_::Normalize( NewName, message_::FolderNameCanNotBeEmpty, message_::FolderNameCanNotBeLongerAs, registry::definition::limitation::FolderNameLength );

	CurrentName.Init();
	Account.Directory().Folders().GetName( Folder, CurrentName );

	if ( NewName != CurrentName ) {
		muafld::sRow Parent = Account.Directory().Folders().Parent( Folder );

		if ( Parent == qNIL )
			qRGnr();

		if ( Account.Directory().Folders().Search( Parent, NewName ) != qNIL )
			REPORT( FolderWithSameNameAlreadyExistsInThisFolder );
		else
			Account.RenameFolder( Folder, NewName );
	}
qRR 
qRT
qRE
}

DEC( GetMailsFields, 1 )
{
qRH
	ACCOUNTh;
	muamel::wRows Wanted, Available;
	muaagt::wRows Agents;
qRB
	ACCOUNTb;

	muafld::sRow Folder = *Request.IdIn();

	if ( !Account.Directory().Exists( Folder ) )
		REPORT( UnknownFolder );

	tol::Init( Wanted );
	Account.Directory().GetMails( Folder, Wanted );

	fblbkd::dIds &Ids = Request.IdsOut();
	fblbkd::dStrings &Subjects = Request.StringsOut();

	tol::Init( Agents, Available );
	Account.GetFields( Wanted, Subjects, Agents, Available );

	fbltyp::Convert( Available, Ids );
	fbltyp::Convert(Agents, Request.IdsOut() );
qRR 
qRT
qRE
}

DEC( GetMail, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	muamel::sRow Mail = *Request.IdIn();

	if ( !Account.Directory().Mails().Exists( Mail ) )
		qRGnr();

	Account.GetMail (Mail, Request.StringOut() );
qRR 
qRT
qRE
}

DEC( MoveMailTo, 1 )
{
qRH
	ACCOUNTh;
qRB
	ACCOUNTb;

	muamel::sRow Mail = *Request.IdIn();
	muafld::sRow Folder = *Request.IdIn();

	if ( !Account.Directory().Mails().Exists( Mail ) )
		qRGnr();

	if ( !Account.Directory().Folders().Exists( Folder ) )
		qRGnr();

	Account.MoveMailTo( Mail, Folder );
qRR 
qRT
qRE
}

DEC( MoveFolderTo, 1 )
{
qRH
	ACCOUNTh;
	str::wString Name;
qRB
	ACCOUNTb;

	muafld::sRow Folder = *Request.IdIn();
	muafld::sRow NewParent = *Request.IdIn();

	if ( !Account.Directory().Folders().Exists( Folder ) )
		qRGnr();

	if ( !Account.Directory().Folders().Exists( NewParent ) )
		qRGnr();

	if ( !Account.Directory().IsMoveable( Folder ) )
		REPORT( FolderNotMoveable );

	Name.Init();
	Account.Directory().Folders().GetName( Folder, Name );

	if ( Account.Directory().Folders().Search( NewParent, Name ) != qNIL )
		REPORT( FolderWithSameNameAlreadyExistsInThisFolder );
	else if ( Account.Directory().Folders().IsDescendant( NewParent, Folder ) )
		REPORT( FolderCannotBeMovedToDescendant );
	else
		Account.MoveFolderTo( Folder, NewParent );
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
			fblbkd::cId,		// Id of the agent. New one is created if undefined.
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

	Backend.Add( D( GetRootAndInboxFolders, 1 ),
		fblbkd::cEnd,
			fblbkd::cId,	// Root.
			fblbkd::cId,	// Inbox.
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

	Backend.Add( D( CreateFolder, 1 ),
			fblbkd::cId,		// Parent folder.
			fblbkd::cString,	// Name.
		fblbkd::cEnd,
			fblbkd::cId,		// The created folder id.
		fblbkd::cEnd );

	Backend.Add(D( RenameFolder, 1 ),
			fblbkd::cId,		// Id of the folder.
			fblbkd::cString,	// New name.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Backend.Add( D( GetMailsFields, 1 ),
			fblbkd::cId,		// Folder id.
		fblbkd::cEnd,
			fblbkd::cIds,		// Ids of the mails.
			fblbkd::cStrings,	// Subjects of the mails.
			fblbkd::cIds,		// Agents of the mails.
		fblbkd::cEnd );

	Backend.Add( D( GetMail, 1 ),
			fblbkd::cId,		// Mail id.
		fblbkd::cEnd,
			fblbkd::cString,	// Mail content.
		fblbkd::cEnd );

	Backend.Add( D( MoveMailTo, 1 ),
			fblbkd::cId,	// Mail.
			fblbkd::cId,	// Folder.
		fblbkd::cEnd,
		fblbkd::cEnd );

	Backend.Add( D( MoveFolderTo, 1 ),
			fblbkd::cId,	// Folder to move.
			fblbkd::cId,	// New parent.
		fblbkd::cEnd,
		fblbkd::cEnd );
}

