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

	muaima::eStatus DumpPendingResponses_(
		bso::sBool Concerned,
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
		muaima::eCode Code = muaima::c_Undefined;

		while ( ( Code = Console.GetPendingCode() ) != muaima::c_None ) {
			if ( Verbose || ( Concerned &&  ( Code != muaima::cOK ) ) ) {
				fdr::rIDriver &Driver = Console.GetResponseDriver();

				cio::COut << muaima::GetLabel( Code );

				if ( !Driver.EndOfFlow() )
					cio::COut << ": ";

				misc::Dump( Driver );
				cio::COut << txf::commit;
			} else
				Console.SkipResponse();
		}

		return Console.GetStatus();
	}

	void DumpResponses_(
		bso::sBool Concerned,
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
		eStatus Status = DumpPendingResponses_( Concerned, Verbose, Console );

		if ( Verbose || ( Status != muaima::sOK ) ) {
			cio::COut << muaima::GetLabel(Status) << ": ";
			misc::Dump( Console.GetResponseDriver() );
			cio::COut << txf::commit;
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

		Login( Username, Password, Console );
		DumpResponses_( false, Verbose, Console );
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
	private:
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
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Activate ? misc::vOut : misc::vNone );
			muaima::rConsole::Init( *this );
			muaima::Connect( *this );
			DumpResponses_( false, Activate, *this );
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

	muaima::Capability( Console );
	DumpResponses_( true, Verbose, Console );
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

	muaima::List( Reference, Mailbox, Console );
	DumpResponses_( true, Verbose, Console );
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

	muaima::LSub( Reference, Mailbox, Console ),
	DumpResponses_( true, Verbose, Console );
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

	muaima::Select( Mailbox, Console );
	DumpResponses_( true, Verbose, Console );
qRR
qRT
qRE
}

namespace {
	void GetSequence_( str::dString &Sequence )
	{
		sclmisc::MGetValue( registry::parameter::imap::Sequence, Sequence );
	}

	void GetItems_( str::dString &Items )
	{
		sclmisc::MGetValue( registry::parameter::imap::Items, Items );
	}

	void GetMailboxSequenceAndItems_(
		str::dString &Mailbox,
		str::dString &Sequence,
		str::dString &Items )
	{
		GetMailbox_( Mailbox );
		GetSequence_( Sequence );
		GetItems_( Items );
	}
}

void imap::Fetch( void )
{
qRH
	str::wString Mailbox, Sequence, Items;
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	tol::Init( Mailbox, Sequence, Items );
	GetMailboxSequenceAndItems_( Mailbox, Sequence, Items );

	Console.Init( Verbose );

	muaima::Select( Mailbox, Console );
	DumpResponses_( false, Verbose, Console );
	muaima::Fetch( Sequence, Items, Console );
	DumpResponses_( true, Verbose, Console );
qRR
qRT
qRE
}

namespace{
	class rSession_
	: public misc::rVerboseIODriver,
	  public muaima::rSession
	{
	public:
		void reset( bso::sBool P = true )
		{
			rSession::reset( P );
			misc::rVerboseIODriver::reset( P );
		}
		qCVDTOR( rSession_ );
		void Init( void )
		{
		qRH
			str::wString Username, Password;
		qRB
			tol::Init( Username, Password );

			GetUsernameAndPassword_( Username, Password );

			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, misc::IsVerboseActivated() ? misc::vOut : misc::vNone );
			muaima::rSession::Init( *this, Username, Password );
		qRR
		qRT
		qRE
		}
	};
}

void imap::Test( void )
{
qRH
//	rSession_ Session;
qRB
//	Session.Init();

	Fetch();
qRR
qRT
qRE
}
