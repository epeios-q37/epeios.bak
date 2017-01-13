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

	void GetUsernameAndPassword_(
		str::dString &Username,
		str::dString &Password )
	{
		sclmisc::MGetValue( registry::parameter::imap::Username, Username );
		sclmisc::MGetValue( registry::parameter::imap::Password, Password );
	}

	void DumpPending_(
		bso::sBool Concerned,
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
		muaima::eCode Code = muaima::c_Undefined;

		while ( ( Code = Console.GetCode()) != muaima::c_None ) {
			if ( Verbose || ( Concerned &&  ( Code != muaima::cOK ) ) ) {
				fdr::rIDriver &Driver = Console.GetResponseDriver();

				cio::COut << muaima::GetLabel( Code );

				if ( !Driver.EndOfFlow() )
					cio::COut << ": ";

				misc::Dump( Driver );
				cio::COut << txf::nl << txf::commit;
			} else
				Console.SkipResponse();
		}
	}

	void Dump_(
		eStatus Status,
		bso::sBool Concerned,
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
		DumpPending_( Concerned, Verbose, Console );
		Status = muaima::GetCompletionStatus( Console );

		if ( Verbose || ( Status != muaima::sOK ) ) {
			cio::COut << muaima::GetLabel(Status) << ": ";
			misc::Dump( Console.GetResponseDriver() );
			cio::COut << txf::nl << txf::commit;
		} else
			Console.SkipResponse();

		if ( ( Status == muaima::sNO ) || ( Status == muaima::sBAD ) )
			qRAbort();
	}

	void Login_(
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
	qRH
		str::wString Username, Password;
	qRB
		tol::Init( Username, Password );

		GetUsernameAndPassword_( Username, Password );

		Dump_( Login( Username, Password, Console  ), false, Verbose, Console );
	qRR
	qRT
	qRE
	}
}

namespace{
	class rConsole_
	: public misc::rVerboseIODriver,
	  public muaima::rConsole
	{
		bso::sBool Connected_ = true;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( IsConnected() )
					muaima::Logout( *this );
			}

			rConsole::reset( P );
			misc::rVerboseIODriver::reset( P );
			Connected_ = false;
		}
		qCVDTOR( rConsole_ );
		void Init( bso::sBool Activate )
		{
			Connected_ = false;
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Activate );
			muaima::rConsole::Init( *this );
			Dump_( muaima::Connect( *this ), false, Activate, *this );
			Connected_ = true;
			Login_( Activate, *this );
		}
	};
}

void imap::Capability( void )
{
qRH
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	Console.Init( Verbose );

	Dump_( muaima::Capability( Console ), true, Verbose, Console );
qRR
qRT
qRE
}

namespace {
	void GetReference_( str::dString &Reference )
	{
		sclmisc::MGetValue( registry::parameter::imap::Reference, Reference );
	}

	void GetMailbox_( str::dString &Mailbox )
	{
		sclmisc::MGetValue( registry::parameter::imap::Mailbox, Mailbox );
	}

	void GetReferenceAndMailbox_(
		str::dString &Reference,
		str::dString &Mailbox )
	{
		GetReference_( Reference );
		GetMailbox_( Mailbox );
	}
}

void imap::List( void )
{
qRH
	str::wString Reference, Mailbox;
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	tol::Init( Reference, Mailbox );
	GetReferenceAndMailbox_( Reference, Mailbox );

	Console.Init( Verbose );

	Dump_( muaima::List( Reference, Mailbox, Console ), true, Verbose, Console );
qRR
qRT
qRE
}

void imap::LSub( void )
{
qRH
	str::wString Reference, Mailbox;
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	tol::Init( Reference, Mailbox );
	GetReferenceAndMailbox_( Reference, Mailbox );

	Console.Init( Verbose );

	Dump_( muaima::LSub( Reference, Mailbox, Console ), true, Verbose, Console );
qRR
qRT
qRE
}

void imap::Select( void )
{
qRH
	str::wString Mailbox;
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	tol::Init( Mailbox );
	GetMailbox_( Mailbox );

	Console.Init( Verbose );

	Dump_( muaima::Select( Mailbox, Console ), true, Verbose, Console );
qRR
qRT
qRE
}

void imap::Test( void )
{
qRH
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	Console.Init( Verbose );

	Dump_( muaima::List( str::wString( "inbox" ), str::wString( "" ), Console ), true, Verbose, Console );
qRR
qRT
qRE
}
