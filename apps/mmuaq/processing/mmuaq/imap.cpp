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

#include "sclm.h"

#include "csdbnc.h"

using namespace imap;

namespace {
	using namespace muaima;

	void GetUsernameAndPassword_(
		str::dString &Username,
		str::dString &Password )
	{
		sclm::MGetValue( registry::parameter::imap::Username, Username );
		sclm::MGetValue( registry::parameter::imap::Password, Password );
	}

	muaima::eStatus DumpPendingResponses_(
		bso::sBool Concerned,
		bso::sBool Verbose,
		muaima::rConsole &Console )
	{
		muaima::eResponseCode Code = muaima::rc_Undefined;

		while ( ( Code = Console.GetPendingResponseCode() ) != muaima::rc_None ) {
			if ( Verbose || ( Concerned &&  ( Code != muaima::rcOK ) ) ) {
				fdr::rRDriver &Driver = Console.GetResponseDriver();

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

		if ( ( Status == muaima::sNO ) || ( Status == muaima::sBAD ) || ( Status == muaima::sDisconnected ) )
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
			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, Verbose ? flx::cOut : flx::cNone );
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
		sclm::MGetValue( registry::parameter::imap::Reference, Reference );
	}

	void GetMailbox_( str::dString &Mailbox )
	{
		sclm::MGetValue( registry::parameter::imap::Mailbox, Mailbox );
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
	void GetSequenceSet_( str::dString &SequenceSet )
	{
		sclm::MGetValue( registry::parameter::imap::SequenceSet, SequenceSet );
	}

	void GetItems_( str::dString &Items )
	{
		sclm::MGetValue( registry::parameter::imap::Items, Items );
	}

	void GetMailboxSequenceSetAndItems_(
		str::dString &Mailbox,
		str::dString &SequenceSet,
		str::dString &Items )
	{
		GetMailbox_( Mailbox );
		GetSequenceSet_( SequenceSet );
		GetItems_( Items );
	}
}

namespace {
	inline muaima::eFlavor GetFlavor_( void )
	{
		if ( sclm::OGetBoolean( registry::parameter::imap::UID, false ) )
			return muaima::fUID;
		else
			return muaima::fRegular;
	}
}

void imap::Fetch( void )
{
qRH
	str::wString Mailbox, SequenceSet, Items;
	bso::sBool Verbose = false;
	rConsole_ Console;
qRB
	Verbose = misc::IsVerboseActivated();

	tol::Init( Mailbox, SequenceSet, Items );
	GetMailboxSequenceSetAndItems_( Mailbox, SequenceSet, Items );

	Console.Init( Verbose );

	muaima::Select( Mailbox, Console );
	DumpResponses_( false, Verbose, Console );

	muaima::Fetch( GetFlavor_(), SequenceSet, Items, Console );
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

			misc::rVerboseIODriver::Init( registry::parameter::imap::HostPort, misc::IsVerboseActivated() ? flx::cInAndOut : flx::cNone );

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
}

namespace {
	eStatus Handle_(
		eStatus Status,
		const str::dString &Message )
	{
		if ( Status != sOK ) {
			cio::COut << muaima::GetLabel( Status ) << ": " << Message << txf::nl;
			qRAbort();
		}

		return Status;
	}
}

void imap::Folders( void )
{
qRH
	rSession_ Session;
	rFolders Folders;
	str::wString Folder, Message;
qRB
	Message.Init();

	Folder.Init();
	sclm::OGetValue( registry::parameter::imap::Folder, Folder );

	Session.Init();

	if ( !Session.GetFolders( Folder, Folders ) )
		Handle_( Folders.EndStatus( Message ), Message );

	Folder.Init();

	while ( Folders.GetFolder( Folder ) ) {
		cio::COut << Folder << txf::nl << txf::commit;
		Folder.Init();
	}

	Handle_( Folders.EndStatus( Message ), Message );
qRR
qRT
qRE
}

namespace rfc822_ {
	void Handle( muaima::eRFC822Part Part )
	{
	qRH
		rSession_ Session;
		muaima::rRFC822 RFC822;
		str::wString Folder, Message;
	qRB
		Folder.Init();
		sclm::MGetValue( registry::parameter::imap::Folder, Folder );

		Message.Init();
		Session.Init();

		RFC822.Init();
		if ( Session.GetRFC822( Part, GetFlavor_(), Folder, sclm::MGetU32( registry::parameter::MailID ), RFC822, qRPU ) ) {
			misc::Dump( RFC822.GetDriver() );

			Handle_( RFC822.EndStatus( Message ), Message );
		} else if ( Handle_( Session.EndStatus( Message ), Message ) == sOK ) {
			Message.Init();
			sclm::GetBaseTranslation( misc::message::NoCorrespondingMail, Message );
			cio::COut << Message << txf::nl;
		}
	qRR
	qRT
	qRE
	}
}

void imap::RFC822( void )
{
	rfc822_::Handle( muaima::rpAll );
}

void imap::RFC822Size( void )
{
	rfc822_::Handle( muaima::rpSize );
}

void imap::RFC822Header( void )
{
	rfc822_::Handle( muaima::rpHeader );
}

void imap::RFC822Text( void )
{
	rfc822_::Handle( muaima::rpText );
}

void imap::UID( void )
{
qRH
	rSession_ Session;
	muaima::rUID UID;
	str::wString Folder, Message;
qRB
	Folder.Init();
	sclm::MGetValue( registry::parameter::imap::Folder, Folder );

	Message.Init();
	Session.Init();

	UID.Init();
	if ( Session.GetUID( Folder, sclm::MGetU32( registry::parameter::MailID ), UID, qRPU ) ) {
		misc::Dump( UID.GetDriver() );
		Handle_( UID.EndStatus( Message ), Message );
	} else if ( Handle_( Session.EndStatus( Message ), Message ) == sOK ) {
		Message.Init();
		sclm::GetBaseTranslation( misc::message::NoCorrespondingMail, Message );
		cio::COut << Message << txf::nl;
	}
qRR
qRT
qRE
}

void imap::MailAmount( void )
{
qRH
	rSession_ Session;
	bso::sUInt Amount = 0;
	str::wString Folder, Message;
qRB
	Folder.Init();
	sclm::MGetValue( registry::parameter::imap::Folder, Folder );

	Message.Init();
	Session.Init();

	if ( Session.GetMailAmount( Folder, Amount, qRPU ) )
		cio::COut << Amount << txf::nl;

	Handle_( Session.EndStatus( Message ), Message );
qRR
qRT
qRE
}

void imap::Test( void )
{
qRH
	rSession_ Session;
	str::wString Message;
	bso::sUInt Amount = 0;
qRB
	Message.Init();
	Session.Init();

	Session.GetMailAmount( str::wString("inbox"), Amount );

	cio::COut << Amount << txf::nl;
qRR
qRT
qRE
}
