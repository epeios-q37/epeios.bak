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

#define C( name ) case folder_::s##name : return #name; break

const char *frdinstc::folder_::GetLabel( folder_::eState State )
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
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Ids.First();

		while ( Row != qNIL ) {
			Writer.PushTag( "Agent");
			Writer.PutAttribute( "id", *Ids( Row ) );
			Writer.PutValue( Labels( Row ) );
			Writer.PopTag();

			Row = Ids.Next( Row );
		}
	}
}

void frdinstc::rUser::DumpAgents( xml::dWriter &Writer )
{
qRH
	wAgents Agents;
	wStrings Names;
qRB
	tol::Init( Agents, Names );

	Core_.GetAgents( Agents, Names );

	if ( Agents.Amount() != Names.Amount() )
		qRGnr();

	Writer.PushTag( "Agents" );
	Writer.PutAttribute( "Amount", Agents.Amount() );

	Writer.PutAttribute( "Current", **Agent_.Current, **UndefinedAgent );

	dump_agents_::Dump( Agents, Names, Writer );

	Writer.PopTag();
qRR
qRT
qRE
}

void frdinstc::rUser::DumpCurrentAgent( xml::dWriter &Writer )
{
qRH
	wString Name, HostPort, Username; 
qRB
	if ( Agent_.Current != UndefinedAgent ) {

		tol::Init( Name, HostPort, Username );
		Core_.GetAgent( Agent_.Current, Name, HostPort, Username );

		Writer.PushTag( "Agent" );

		Writer.PutAttribute( "Name", Name );
		Writer.PutAttribute( "HostPort", HostPort );
		Writer.PutAttribute( "Username", Username );

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
		xml::dWriter &Writer )
	{
		sdr::sRow Row = Ids.First();

		if ( Ids.Amount() != Subjects.Amount() )
			qRGnr();

		Writer.PushTag("Mails");

		while ( Row != qNIL ) {
			Writer.PushTag("Mail" );

			Writer.PutAttribute("id", *Ids( Row ) );
			Writer.PutAttribute("Subject", Subjects( Row ) );

			Writer.PopTag();

			Row = Ids.Next( Row );
		}

		Writer.PopTag();
	}


}

void frdinstc::rUser::DumpMails( xml::dWriter &Writer )
{
qRH
	fbltyp::wIds Ids;
	fbltyp::wStrings Subjects;
qRB
	tol::Init( Ids, Subjects );

	if ( Folder_.Current != UndefinedFolder )
		Core_.GetMailsFields( Folder_.Current, Ids, Subjects );

	dump_mails_::Dump( Ids, Subjects, Writer );
qRR
qRT
qRE
}

void frdinstc::rUser::DumpMail( xml::dWriter &Writer )
{
qRH
	str::wString Content;
qRB
	if ( Mail_.Current != UndefinedMail ) {
		Content.Init();
		Core_.GetMail( Mail_.Current, Content );
		Writer.PutValue( Content, "Mail" );
	}
qRR
qRT
qRE
}

namespace dump_folders_ {
	void Dump(
		sFolder Folder,
		rUser_ &Core,
		xml::dWriter &Writer );

	void Dump_(
		const dFolders &Folders,
		const dStrings &Names,
		rUser_ &Core,
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

			Dump( Folders( Row ), Core, Writer );

			Writer.PopTag();

			Row = Folders.Next( Row );
		}

		Writer.PopTag();
	}

	void Dump(
		sFolder Folder,
		rUser_ &Core,
		xml::dWriter &Writer )
	{
	qRH
		wFolders Folders;
		wStrings Names;
	qRB
		tol::Init( Folders, Names );

		Core.GetFolders( Folder, Folders );
		Core.GetFoldersNames( Folders, Names );

		Dump_( Folders, Names, Core, Writer );
	qRR
	qRT
	qRE
	}
}

void frdinstc::rUser::DumpFolders( xml::dWriter &Writer )
{
	dump_folders_::Dump( UndefinedFolder, Core_, Writer );
}

const str::dString &frdinstc::rUser::GetAgentStatus( str::dString &Status )
{
	if ( Agent_.Current != frdinstc::UndefinedAgent ) {
		if ( Agent_.Edition  )
			Status = "Edit";
		else
			Status = "Show";
	} else if ( Agent_.Edition )
		Status = "Create";

	return Status;
}

void frdinstc::rUser::DumpAgentStatusAttribute(
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
