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

#ifndef FRDFRNTND__INC
# define FRDFRNTND__INC

# include "muainf.h"
# include "frdapi.h"

# include "frdrgstry.h"

# include "sclfrntnd.h"

# include "csducl.h"
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
		void GetAgents(
			dAgents &Agents,
			dStrings &Names )
		{
			Statics.MUAGetAgents_1( Agents, Names );
		}
		void GetAgent(
			sAgent Agent,
			dString &Name,
			dString &HostPort,
			dString &Username )
		{
			Statics.MUAGetAgent_1( *Agent, Name, HostPort, Username );
		}
		sAgent CreateAgent(
			const dString &Name,
			const dString &HostPort,
			const dString &Username,
			const dString &Password )
		{
			sAgent Agent = UndefinedAgent;

			Statics.MUAUpdateAgent_1( *Agent, Name, HostPort, Username, true, Password, *Agent );

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

			Statics.MUAUpdateAgent_1( *Agent, Name, HostPort, Username, PasswordIsSet, Password, *Agent );
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
