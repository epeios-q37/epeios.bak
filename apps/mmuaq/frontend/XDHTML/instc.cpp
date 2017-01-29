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

#include "instc.h"

#include "flf.h"
#include "fnm.h"

using namespace instc;

using fbltyp::wString;
using fbltyp::wStrings;

#define C( name ) case folder_::s##name : return #name; break

const char *instc::folder_::GetLabel( folder_::eState State )
{
	switch ( State ) {
	C( Viewing );
	C( Creation );
	C( Edition );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a warning.
}

namespace dump_agents_ {
	void Dump(
		const dAgents &Ids,
		dStrings &Labels,
		const fbltyp::dBooleans &Enabled,
		xml::dWriter &Writer )
	{
		if ( !tol::HaveSameAmount( Ids, Labels, Enabled ) )
			qRFwk();

		sdr::sRow Row = Ids.First();

		while ( Row != qNIL ) {
			Writer.PushTag( "Agent");
			Writer.PutAttribute( "id", *Ids( Row ) );
			if ( !Enabled( Row ) )
				Writer.PutAttribute("Disabled", "true" );
			Writer.PutValue( Labels( Row ) );
			Writer.PopTag();

			Row = Ids.Next( Row );
		}
	}
}

void instc::rUser::DumpAgents( xml::dWriter &Writer )
{
qRH
	wAgents Agents;
	wStrings Names;
	fbltyp::wBooleans Enabled;
qRB
	tol::Init( Agents, Names, Enabled );

	F_().GetAgents( Agents, Names, Enabled );

	if ( Agents.Amount() != Names.Amount() )
		qRGnr();

	Writer.PushTag( "Agents" );
	Writer.PutAttribute( "Amount", Agents.Amount() );

	Writer.PutAttribute( "Current", **Agent_.Current, **UndefinedAgent );

	dump_agents_::Dump( Agents, Names, Enabled, Writer );

	Writer.PopTag();
qRR
qRT
qRE
}

void instc::rUser::DumpCurrentAgent( xml::dWriter &Writer )
{
qRH
	wString Name, HostPort, Username; 
	frdfrntnd::eProtocol Protocol = frdfrntnd::p_Undefined;
	bso::sBool Enabled = false;
qRB
	if ( Agent_.Current != UndefinedAgent ) {

		tol::Init( Name, HostPort, Username );
		F_().GetAgent( Agent_.Current, Name, Protocol, HostPort, Username, Enabled );

		Writer.PushTag( "Agent" );

		Writer.PutAttribute( "Name", Name );
		Writer.PutAttribute( "Protocol", frdfrntnd::GetLabel( Protocol ) );
		Writer.PutAttribute( "HostPort", HostPort );
		Writer.PutAttribute( "Username", Username );

		if ( !Enabled )
			Writer.PutAttribute("Disabled", "true" );

		Writer.PopTag();
	}
qRR
qRT
qRE
}

namespace dump_mails_{
	void Dump(
		const fbltyp::dIds &Ids,
		fbltyp::dStrings &Subjects,
		const dAgents &Agents,
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Ids.First();

		if ( Ids.Amount() != Subjects.Amount() )
			qRGnr();

		if ( Ids.Amount() != Agents.Amount() )
			qRGnr();

		Writer.PushTag("Mails");

		while ( Row != qNIL ) {
			Writer.PushTag("Mail" );

			Writer.PutAttribute("id", *Ids( Row ) );
			Writer.PutAttribute("Subject", Subjects( Row ) );
			Writer.PutAttribute("Agent", *Agents( Row ) );

			Writer.PopTag();

			Row = Ids.Next( Row );
		}

		Writer.PopTag();
	}
}

void instc::rUser::DumpMails( xml::dWriter &Writer )
{
qRH
	fbltyp::wIds Ids;
	fbltyp::wStrings Subjects;
	wAgents Agents;
qRB
	tol::Init( Ids, Subjects, Agents );

	if ( Folder_.Current != UndefinedFolder )
		F_().GetMailsFields( Folder_.Current, Ids, Subjects, Agents );

	dump_mails_::Dump( Ids, Subjects, Agents, Writer );
qRR
qRT
qRE
}

void instc::rUser::DumpMail( xml::dWriter &Writer )
{
qRH
	str::wString Content;
qRB
	if ( Mail_.Current != UndefinedMail ) {
		Content.Init();
		F_().GetMail( Mail_.Current, Content );
		Writer.PutValue( Content, "Mail" );
	}
qRR
qRT
qRE
}

namespace dump_folders_ {
	// Predeclaration ; defined below.
	void Dump(
		sFolder Folder,
		rFrontend &Frontend,
		xml::dWriter &Writer );

	void Dump_(
		const dFolders &Folders,
		const dStrings &Names,
		rFrontend &Frontend,
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Folders.First();

		if ( Folders.Amount() != Names.Amount() )
			qRGnr();

		Writer.PushTag( "Folders" );

		Writer.PutAttribute("Amount", Folders.Amount() );

		while ( Row != qNIL ) {
			Writer.PushTag("Folder");

			Writer.PutAttribute("id", *Folders( Row ) );
			Writer.PutAttribute("Name", Names( Row ) );

			Dump( Folders( Row ), Frontend, Writer );

			Writer.PopTag();

			Row = Folders.Next( Row );
		}

		Writer.PopTag();
	}

	void Dump(
		sFolder Folder,
		rFrontend &Frontend,
		xml::dWriter &Writer )
	{
	qRH
		wFolders Folders;
		wStrings Names;
	qRB
		tol::Init( Folders, Names );

		Frontend.GetFolders( Folder, Folders );
		Frontend.GetFoldersNames( Folders, Names );

		Dump_( Folders, Names, Frontend, Writer );
	qRR
	qRT
	qRE
	}
}

void instc::rUser::DumpFolders( xml::dWriter &Writer )
{
	dump_folders_::Dump( UndefinedFolder, F_(), Writer );
}

const str::dString &instc::rUser::GetAgentStatus( str::dString &Status )
{
	if ( Agent_.Current != instc::UndefinedAgent ) {
		if ( Agent_.Edition  )
			Status = "Edit";
		else
			Status = "Show";
	} else if ( Agent_.Edition )
		Status = "Create";

	return Status;
}

void instc::rUser::DumpAgentStatusAttribute(
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
