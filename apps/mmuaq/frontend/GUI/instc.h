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

// INSTanCes

#ifndef INSTC_INC_
# define INSTC_INC_

# include "frdfrntnd.h"

namespace instc {
	using namespace frdfrntnd;

	// To avoid ambiguity with 'mail' module.
	namespace mail {
		using namespace frdfrntnd::mail;
	}

	class rUser_
	{
	private:
		qRMV( frdfrntnd::rFrontend, F_,  Frontend_ );
		mmuaq::rMUAMyObject Object_;
	public:
		void reset( bso::bool__ P = true )
		{	
			Frontend_ = NULL;

			Object_.reset( P );
		}
		E_CVDTOR( rUser_ );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			Object_.Init( Frontend.MyObject );
		}
		void Ping( void )
		{
			F_().Ping();
		}
		void Crash( void )
		{
			F_().Crash();
		}
		str::string_ &ToUpper( dString &String )
		{
			str::string Result;

			Result.Init();

			Object_.ToUC( String, Result );

			String = Result;

			return String;
		}
		void TestMessage( void )
		{
			Object_.Test();
		}
	};

	namespace agent_ {
		struct sTracker
		{
		public:
			sAgent Current;
			bso::sBool Edition;
			bso::sBool ShowPassword;
			void reset( bso::sBool = true )
			{	
				Current = UndefinedAgent;
				Edition = false;
				ShowPassword = false;
			}
			qCDTOR( sTracker );
			void Init( void )
			{	
				Current = UndefinedAgent;
				Edition = false;
				ShowPassword = false;
			}
		};
	}

	namespace shared_ {
		template <typename id> struct sTracker
		{
		public:
			id
				Current,
				Dragged;
			void reset( bso::sBool P = true )
			{
				Current = Dragged = fbltyp::UndefinedId;
			}
			qCDTOR( sTracker );
			void Init( void )
			{
				Current = Dragged = fbltyp::UndefinedId;
			}
		};
	}

	namespace folder_
	{
		qENUM( State )
		{
			sViewing,
			sEdition,
			sCreation,	// In this state, the current folder is the one to which the created folder will be attached.
			s_amount,
			s_Undefined
		};

		const char *GetLabel( eState State );

		typedef shared_::sTracker<sFolder> sTracker_;

		struct sTracker
		: public sTracker_
		{
		public:
			sFolder
				Root,
				Inbox;
			eState State;
			void reset( bso::sBool P = true )
			{
				sTracker_::reset( P );
				Root = Inbox = UndefinedFolder;
				State = s_Undefined;
			}
			qCDTOR( sTracker );
			void Init( void )
			{
				sTracker_::Init();
				Root = Inbox = UndefinedFolder;
				State = s_Undefined;
			}
		};
	}

	namespace mail_ {
		typedef shared_::sTracker<sMail> sTracker;
	}

	class rUser
	{
	private:
		rUser_ Core_;
		qRMV( rFrontend, F_, Frontend_ );
		agent_::sTracker Agent_;
		folder_::sTracker Folder_;
		mail_::sTracker Mail_;
	public:
		void reset( bso::bool__ P = true )
		{	
			tol::reset( P, Core_, Frontend_, Agent_, Folder_, Mail_ );
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			Frontend_ = &Frontend;

			if ( F_().IsConnected() )
				Core_.Init(F_() );

			tol::Init( Agent_, Folder_, Mail_ );
		}
		qRODISCLOSEr( agent_::sTracker, Agent );
		qRODISCLOSEr( folder_::sTracker, Folder );
		qRODISCLOSEr( mail_::sTracker, Mail );
		str::string_ &ToUpper( str::string_ &String )
		{
			return Core_.ToUpper( String );
		}
		void TestMessage( void )
		{
			Core_.TestMessage();
		}
		bso::sBool Login(
			const str::dString &Username,
			const str::dString &Password )
		{
			if ( F_().Login( Username, Password ) ) {
				F_().GetRootAndInboxFolders( Folder_.Root, Folder_.Inbox );
				Folder_.Current = Folder_.Inbox;
				Folder_.State = folder_::sViewing;
				return true;
			} else 
				return false;

		}
		void DumpAgents( xml::dWriter &Writer );
		void DumpCurrentAgent( xml::dWriter &Writer );
		// Write only attributes (only usable on a start tag).
		void DumpMails( xml::dWriter &Writer );
		void DumpMail( xml::dWriter &Writer );
		void DumpCurrentFolderAttributes( xml::dWriter &Writer )
		{
			if ( Folder_.Current != folder::Undefined ) {
				Writer.PutAttribute("CurrentFolder", **Folder_.Current );

				Writer.PutAttribute( "CurrentFolderState", folder_::GetLabel( Folder_.State ) );
			}
		}
		void DumpFolders( xml::dWriter &Writer );
		const str::dString &GetAgentStatus( str::dString &Status );
		// Returns true if the state changed.
		void Check( void )
		{
			F_().Check();
			Mail_.Init();
		}
		bso::sBool SelectFolder( sFolder Folder )
		{
			bso::sBool Changed = Folder_.State != folder_::sViewing;

			Mail_.Current = mail::Undefined;

			Folder_.Current = Folder;
			Folder_.State = folder_::sViewing;

			return Changed;
		}
		void NewFolder( void )
		{
			if ( Folder_.Current == folder::Undefined )
				qRGnr();

			Folder_.State = folder_::sCreation;
		}
		bso::sBool EditFolder( void )
		{
			if ( Folder_.Current == folder::Undefined )
				qRGnr();

			if ( ( Folder_.Current == Folder_.Root ) || ( Folder_.Current == Folder_.Inbox ) )
				return false;
			else {
				Folder_.State = folder_::sEdition;
				return true;
			}
		}
		void ApplyFolder( const str::dString &Name )
		{
			if ( Folder_.Current == folder::Undefined )
				qRGnr();

			switch ( Folder_.State ) {
			case folder_::sCreation:
				Folder_.Current = F_().CreateFolder( Folder_.Current, Name );
				break;
			case folder_::sEdition:
				F_().UpdateFolder( Folder_.Current, Name );
				break;
			default:
				qRGnr();
				break;
			}

			Folder_.State = folder_::sViewing;
		}
		void DiscardFolder( void )
		{
			if ( Folder_.Current == folder::Undefined )
				qRGnr();

			Folder_.State = folder_::sViewing;
		}
		void SelectMail( sMail Mail )
		{
			Mail_.Current = Mail;
		}
		void DragMail( sMail Mail )
		{
			if ( Mail_.Dragged != UndefinedMail )
				qRGnr();

			Mail_.Dragged = Mail;
		}
		void EndMailDragging( void )
		{
			Mail_.Dragged = UndefinedMail;
		}
		bso::sBool MailDragInProgress( void ) const
		{
			return Mail_.Dragged != UndefinedMail;
		}
		void DragFolder( sFolder Folder )
		{
			if ( Folder_.Dragged != UndefinedFolder )
				qRGnr();

			Folder_.Dragged = Folder;
		}
		void EndFolderDragging( void )
		{
			Folder_.Dragged = UndefinedFolder;
		}
		bso::sBool FolderDragInProgress( void ) const
		{
			return Folder_.Dragged != UndefinedFolder;
		}
		bso::sBool DropToFolder( sFolder Folder )
		{
			if ( Mail_.Dragged != UndefinedMail ) {
				F_().MoveMailTo( Mail_.Dragged, Folder );
				Mail_.Dragged = UndefinedMail;
				return false;
			} else if ( Folder_.Dragged != UndefinedFolder ) {
				F_().MoveFolderTo( Folder_.Dragged, Folder );
				Folder_.Dragged = UndefinedFolder;
				return true;
			} else
				qRGnr();

			return false;	// To avoid a warning.
		}
		void DumpAgentStatusAttribute(
			const char *Name,
			xml::dWriter &Writer );
		void DefineAgent( void )
		{
			Agent_.Current = UndefinedAgent;
			Agent_.Edition = true;
		}
		void DiscardAgent( void )
		{
			Agent_.Current = UndefinedAgent;
			Agent_.Edition = false;
		}
		void UpdateAgent(
			const dString &Name,
			frdfrntnd::eProtocol Protocol,
			const dString &HostPort,
			const dString &Username,
			bso::sBool PasswordIsSet,
			const dString &Password )
		{
			if ( Agent_.Current != UndefinedAgent )
				F_().UpdateAgent( Agent_.Current, Name, HostPort, Username, PasswordIsSet, Password );
			else 
				Agent_.Current = F_().CreateAgent( Name, Protocol, HostPort, Username, Password );

			Agent_.Edition = false;
		}
		void SelectAgent( sAgent Agent )
		{
			Agent_.Current = Agent;
			Agent_.Edition = false;
		}
		void EditAgent( void )
		{
			Agent_.Edition = true;
		}
		void RemoveAgent( void )
		{
			if ( Agent_.Current == UndefinedAgent )
				qRGnr();

			F_().RemoveAgent( Agent_.Current );
			Agent_.Current = frdfrntnd::agent::Undefined;
		}
		void SetAgentShowPassword( bso::sBool Value )
		{
			Agent_.ShowPassword = Value;
		}
	};
}

#endif
