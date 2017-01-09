/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq'.

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

#include "imap.h"

#include "misc.h"
#include "registry.h"

#include "muaima.h"

#include "sclmisc.h"

#include "csdbnc.h"

using namespace imap;

namespace {
	using namespace muaima;
	using namespace base;

	void Init_( csdbnc::rIODriver &Server )
	{
	qRH
		str::wString HostPort;
		qCBUFFERr Buffer;
	qRB
		HostPort.Init();
		sclmisc::MGetValue( registry::parameter::imap::HostPort, HostPort );

		if ( !Server.Init( HostPort.Convert( Buffer ), SCK_INFINITE, qRPU ) )
			sclmisc::ReportAndAbort("UnableToConnect", HostPort );
	qRR
	qRT
	qRE
	}

	void GetUsernameAndPassword_(
		str::dString &Username,
		str::dString &Password )
	{
		sclmisc::MGetValue( registry::parameter::imap::Username, Username );
		sclmisc::MGetValue( registry::parameter::imap::Password, Password );
	}

	void DumpPending_(
		muaima::eCode Awaited,
		bso::sBool KeepAnswer,
		muaima::rSession &Session )
	{
		muaima::eCode Code = muaima::c_Undefined;

		while ( ( Code = Session.GetCode()) != muaima::c_None ) {
			if ( KeepAnswer || ( Code == Awaited ) ) {
				if ( KeepAnswer  )
					cio::COut << muaima::GetLabel(Code) << ": ";
				misc::Dump( Session.GetResponseDriver() );
				cio::COut << txf::nl;
			} else
				Session.SkipResponse();
		}
	}

	void Dump_(
		eStatus Status,
		muaima::eCode Awaited,
		bso::sBool KeepAnswer,
		muaima::rSession &Session )
	{
		if ( Status == s_Pending ) {
			DumpPending_( Awaited, KeepAnswer, Session );
			Status = Session.GetPendingStatus();
		}

		if ( KeepAnswer ) {
			Status = muaima::base::GetCompletionStatus( Session );
			cio::COut << muaima::base::GetLabel(Status) << ": ";
			misc::Dump( Session.GetResponseDriver() );
			cio::COut << txf::nl;
		} else
			Session.SkipResponse();

	}

	void Login_(
		bso::sBool KeepAnswer,
		muaima::rSession &Session )
	{
	qRH
		str::wString Username, Password;
	qRB
		tol::Init( Username, Password );

		GetUsernameAndPassword_( Username, Password );

		Dump_( Login( Username, Password, Session  ), muaima::c_Undefined, KeepAnswer, Session );
	qRR
	qRT
	qRE
	}
}

void imap::Capability( void )
{
qRH
	csdbnc::rIODriver Server;	
	muaima::rSession Session;
	bso::sBool KeepAnswer = false;
qRB
	KeepAnswer = sclmisc::BGetBoolean( registry::parameter::KeepAnswer, false );

	Init_( Server );

	Session.Init( Server );

	Dump_( muaima::base::Connect( Session ), muaima::c_Undefined, KeepAnswer, Session );

	Login_( KeepAnswer, Session );

	Dump_( muaima::base::Capability( Session ), muaima::cCapability, KeepAnswer, Session );

	Dump_( muaima::base::Logout( Session ), muaima::c_Undefined, KeepAnswer, Session );
qRR
qRT
qRE
}
