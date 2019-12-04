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

#ifndef FRDFRNTND_INC_
# define FRDFRNTND_INC_

# include "muainf.h"
# include "frdapi.h"

# include "frdrgstry.h"

# include "sclfrntnd.h"

# include "xml.h"

namespace frdfrntnd {
	using sclfrntnd::rKernel;
	typedef sclfrntnd::rFrontend rFrontend_;
	typedef fblfrd::cFrontend cFrontend_;

	using fbltyp::dString;
	using fbltyp::dStrings;

	SCLF_I( agent, Agent, Id);
	SCLF_I( folder, Folder, Id );
	SCLF_I( mail, Mail, Id );

	qENUM( Protocol ) {
		pPOP3,
		pIMAP,
		p_amount,
		p_Undefined
	};

	inline const char *GetLabel( eProtocol Protocol )
	{
		switch ( Protocol ) {
		case pPOP3:
			return "POP3";
			break;
		case pIMAP:
			return "IMAP";
			break;
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a warning.
	}

	inline bso::sBool GetProtocolAsBoolean( eProtocol Protocol )
	{
		switch ( Protocol ) {
		case pPOP3:
			return false;
			break;
		case pIMAP:
			return true;
			break;
		default:
			qRGnr();
			break;
		}

		return false;	// To avoid a warning.
	}

	inline eProtocol GetProtocolFromBoolean( bso::sBool Value )
	{
		return ( GetProtocolAsBoolean( pPOP3 ) == Value ? pPOP3 : pIMAP );
	}

	inline eProtocol GetProtocol( const str::dString &Pattern )
	{
		if ( Pattern == GetLabel( pPOP3 ) )
			return pPOP3;
		else if ( Pattern == GetLabel( pIMAP ) )
			return pIMAP;
		else
			return p_Undefined;
	}

	class rFrontend
	: public rFrontend_,
	  public cFrontend_
	{
	protected:
		virtual void FBLFRDOnConnection( void ) override
		{
			Statics.Init( *this );
			MyObject.Init( *this );
		}
		virtual void FBLFRDOnDisconnection( void ) override
		{
			Statics.reset();
			MyObject.reset();
		}
	public:
		mmuaq::fStatics Statics;
		mmuaq::fMUAMyObjectCommon MyObject;
		void reset( bso::bool__ P = true )
		{
			rFrontend_::reset( P );
			Statics.reset( P );
			MyObject.reset( P );
		}
		qCVDTOR( rFrontend );
		void Init(
			rKernel &Kernel,
			const char *Language,
			fblfrd::reporting_callback__ &ReportingCallback )
		{
			rFrontend_::Init( Kernel, Language, *this, ReportingCallback );
		}
		void LoadSetupOfId( const dString &Id )
		{
			Statics.LoadSetupOfId_1( Id );
		}
		void LoadSetupContent( const dString &Content )
		{
			Statics.LoadSetupContent_1( Content );
		}
		bso::sBool Login(
			const dString &Username,
			const dString &Password )
		{
			bso::sBool Success = false;

			Statics.MUALogin_1( Username, Password, Success );

			return Success;
		}
		void Check( void )
		{
			Statics.MUACheck_1();
		}
		void GetAgents(
			dAgents &Agents,
			dStrings &Names,
			fbltyp::dBooleans &Enabled )
		{
			Statics.MUAGetAgents_1( Agents, Names, Enabled );
		}
		void GetAgent(
			sAgent Agent,
			dString &Name,
			eProtocol &Protocol,
			dString &HostPort,
			dString &Username,
			bso::sBool &Enabled )
		{
			bso::sBool ProtocolBool = false;
			Statics.MUAGetAgent_1( *Agent, Name, ProtocolBool, HostPort, Username, Enabled );

			Protocol = GetProtocolFromBoolean( ProtocolBool );
		}
		sAgent CreateAgent(
			const dString &Name,
			eProtocol Protocol,
			const dString &HostPort,
			const dString &Username,
			const dString &Password )
		{
			sAgent Agent = UndefinedAgent;

			Statics.MUAUpdateAgent_1( *Agent, Name, GetProtocolAsBoolean( Protocol ), HostPort, Username, true, Password, *Agent );

			return Agent;
		}
		void UpdateAgent(
			sAgent Agent,
			const dString &Name,
			const dString &HostPort,
			const dString &Username,
			bso::sBool PasswordIsSet,
			const dString &Password )
		{
			if ( Agent == UndefinedAgent )
				qRGnr();

			Statics.MUAUpdateAgent_1( *Agent, Name, false, HostPort, Username, PasswordIsSet, Password, *Agent );	// Protocol doesn't matter (is ignored) on update.
		}
		void RemoveAgent( sAgent Agent )
		{
			if ( Agent == UndefinedAgent )
				qRGnr();

			Statics.MUARemoveAgent_1( *Agent );
		}
		void GetMailsFields(
			sFolder Folder,
			fbltyp::dIds &Ids,
			dStrings &Subjects,
			dAgents &Agents )
		{
			Statics.MUAGetMailsFields_1( *Folder, Ids, Subjects, Agents );
		}
		void GetMail(
			sMail Mail,
			str::dString &Content )
		{
			Statics.MUAGetMail_1( **Mail, Content );
		}
		void MoveMailTo(
			sMail Mail,
			sFolder Folder )
		{
			Statics.MUAMoveMailTo_1( *Mail, *Folder );
		}
		void GetRootAndInboxFolders(
			sFolder &Root,
			sFolder &Inbox )
		{
			Statics.MUAGetRootAndInboxFolders_1( *Root, *Inbox );
		}
		void GetFolders(
			sFolder Folder,
			dFolders &Folders )
		{
			Statics.MUAGetFolders_1( *Folder, Folders );
		}
		void GetFoldersNames(
			const dFolders &Folders,
			dStrings &Names )
		{
			Statics.MUAGetFoldersNames_1( Folders, Names );
		}
		folder::sId CreateFolder(
			folder::sId Folder,
			const str::dString &Name )
		{
			Statics.MUACreateFolder_1( *Folder, Name, *Folder );

			return Folder;
		}
		void UpdateFolder(
			folder::sId Folder,
			const str::dString &Name )
		{
			Statics.MUAUpdateFolder_1( *Folder, Name );
		}
		void MoveFolderTo(
			sFolder Folder,
			sFolder Parent )
		{
			Statics.MUAMoveFolderTo_1( *Folder, *Parent );
		}
	};
}

#endif
