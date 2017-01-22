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
		muaima::eResponseCode Code = muaima::rc_Undefined;

		while ( ( Code = Console.GetPendingResponseCode() ) != muaima::rc_None ) {
			if ( Verbose || ( Concerned &&  ( Code != muaima::rcOK ) ) ) {
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

	void Handle_(
		eStatus Status,
		const str::dString &Message )
	{
		cio::COut << muaima::GetLabel( Status ) << ": " << Message << txf::nl;
	}
}

namespace{
	class rConsole_
	: public misc::rVerboseIODriver,
	  public muaima::rConsole
	{
	private:
		bso::sBool Connected_;
		bso::sBool Verbose_;
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( IsConnected() ) {
					muaima::Logout( *this );
					DumpResponses_( false, Verbose_, *this );
				}
			}

			rConsole::reset( P );
			misc::rVerboseIODriver::reset( P );
			Connected_ = false;
			Verbose_ = false;
		}
		qCVDTOR( rConsole_ );
		void Init( bso::sBool Verbose )
		{
			Connected_ = false;
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Verbose ? misc::vOut : misc::vNone );
			muaima::rConsole::Init( *this );
			muaima::Connect( *this );
			DumpResponses_( false, Verbose, *this );
			Connected_ = true;
			Verbose_ = Verbose;
			Login_( Verbose, *this );
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

namespace {
	inline muaima::eFlavor GetFlavor_( void )
	{
		if ( sclmisc::BGetBoolean( registry::parameter::imap::UID, false ) )
			return muaima::fUID;
		else
			return muaima::fRegular;
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

	muaima::Fetch( GetFlavor_(), Sequence, Items, Console );
	DumpResponses_( true, Verbose, Console );
qRR
qRT
qRE
}

namespace {
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
			str::wString Username, Password, Message;
			eStatus Status = s_Undefined;
		qRB
			tol::Init( Username, Password );

			GetUsernameAndPassword_( Username, Password );

			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, misc::IsVerboseActivated() ? misc::vInAndOut : misc::vNone );

			Message.Init();
			if ( ( Status = muaima::rSession::Init( *this, Username, Password, Message, qRPU ) ) != sOK ) {
				cio::COut << muaima::GetLabel( Status ) << ": " << Message << txf::nl;
				qRAbort();
			}
		qRR
		qRT
		qRE
		}
	};

	class cFolders_
	: public muaima::cFolders
	{
	protected:
		virtual void MUAIMAOnFolder( const str::dString &Name ) override
		{
			cio::COut << Name << txf::nl;
		}
	public:
		void reset( bso::sBool = true )
		{}
		qCVDTOR( cFolders_ );
		void Init( void )
		{}
	};

}

void imap::Folders( void )
{
qRH
	rSession_ Session;
	rFolders Folders;
	str::wString Folder, Message;
qRB
	Folder.Init();
	sclmisc::OGetValue( registry::parameter::imap::Folder, Folder );

	Session.Init();

	Session.GetFolders( Folder, Folders );

	Folder.Init();

	while ( Folders.GetFolder( Folder ) ) {
		cio::COut << Folder << txf::nl << txf::commit;
		Folder.Init();
	}

	Message.Init();
	Handle_( Folders.EndStatus( Message ), Message );

qRR
qRT
qRE
}

void imap::Test( void )
{
qRH
	rSession_ Session;
qRB
	Session.Init();

	Session.GetMail(str::wString("INBOX"), 1 );

	misc::Dump(Session.GetValueDriver() );

qRR
qRT
qRE
}
