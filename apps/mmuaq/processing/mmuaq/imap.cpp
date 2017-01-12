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
				cio::COut << txf::nl << txf::commit;
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
		DumpPending_( Awaited, KeepAnswer, Session );
		Status = muaima::base::GetCompletionStatus( Session );

		if ( KeepAnswer ) {
			cio::COut << muaima::base::GetLabel(Status) << ": ";
			misc::Dump( Session.GetResponseDriver() );
			cio::COut << txf::nl << txf::commit;
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

namespace{
	class rVerboseIODriver_
	: public misc::rVerboseIODriver
	{
	public:
		void Init( bso::sBool Activate )
		{
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Activate );
		}
	};
}

void imap::Capability( void )
{
qRH
	bso::sBool Verbose = false;
	rVerboseIODriver_ VerboseDriver;
	muaima::rSession Session;
qRB
	Verbose = misc::IsVerboseActivated();

	VerboseDriver.Init( Verbose );

	Session.Init( VerboseDriver );

	Dump_( muaima::base::Connect( Session ), muaima::c_Undefined, Verbose, Session );

	Login_( Verbose, Session );

	Dump_( muaima::base::Capability( Session ), muaima::cCapability, Verbose, Session );

	Dump_( muaima::base::Logout( Session ), muaima::c_Undefined, Verbose, Session );
qRR
qRT
qRE
}

void imap::Select( void )
{
qRH
	bso::sBool Verbose = false;
	rVerboseIODriver_ VerboseDriver;
	muaima::rSession Session;
qRB
	Verbose = misc::IsVerboseActivated();

	VerboseDriver.Init( Verbose );

	Session.Init( VerboseDriver );

	Dump_( muaima::base::Connect( Session ), muaima::c_Undefined, Verbose, Session );

	Login_( Verbose, Session );

	Dump_( muaima::base::Select( str::wString( "inbox" ), Session ), muaima::cCapability, Verbose, Session );

	Dump_( muaima::base::Logout( Session ), muaima::c_Undefined, Verbose, Session );
qRR
qRT
qRE
}
