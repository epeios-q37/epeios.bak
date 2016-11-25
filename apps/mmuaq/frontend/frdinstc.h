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

// FRontenD INSTanCes

#ifndef FRDINSTC__INC
# define FRDINSTC__INC

# include "frdfrntnd.h"

namespace frdinstc {

	using namespace frdfrntnd;

	SCLF_I( Agent, Id);
	SCLF_I( Folder, Id );

	using fbltyp::dString;
	using fbltyp::dStrings;

	class rUser_
	{
	private:
		qRMV( frdfrntnd::rFrontend, F_,  Frontend_ );
		mmuaq::fStatics S_( void )
		{
			return F_().Statics;
		}
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
		void LoadSetupOfId( const dString &Id )
		{
			S_().LoadSetupOfId_1( Id );
		}
		void LoadSetupContent( const dString &Content )
		{
			S_().LoadSetupContent_1( Content );
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
		bso::sBool Login(
			const dString &Username,
			const dString &Password )
		{
			bso::sBool Success = false;

			S_().MUALogin_1( Username, Password, Success );

			return Success;
		}
		void GetAgents(
			dAgents &Agents,
			dStrings &Names )
		{
			S_().MUAGetAgents_1( Agents, Names );
		}
		void GetAgent(
			sAgent Agent,
			dString &Name,
			dString &HostPort,
			dString &Username )
		{
			S_().MUAGetAgent_1( *Agent, Name, HostPort, Username );
		}
		sAgent CreateAgent(
			const dString &Name,
			const dString &HostPort,
			const dString &Username,
			const dString &Password )
		{
			sAgent Agent = UndefinedAgent;

			S_().MUAUpdateAgent_1( *Agent, Name, HostPort, Username, true, Password, *Agent );

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

			S_().MUAUpdateAgent_1( *Agent, Name, HostPort, Username, PasswordIsSet, Password, *Agent );
		}
		void GetMailsFields(
			sFolder Folder,
			fbltyp::dIds &Ids,
			dStrings &Subjects )
		{
			S_().MUAGetMailsFields_1( *Folder, Ids, Subjects );
		}
		void GetFolders(
			sFolder Folder,
			dFolders &Folders )
		{
			S_().MUAGetFolders_1( *Folder, Folders );
		}
		void GetFoldersNames(
			const dFolders &Folders,
			dStrings &Names )
		{
			S_().MUAGetFoldersNames_1( Folders, Names );
		}
	};

	class rUser
	{
	private:
		rUser_ Core_;
		sAgent CurrentAgent_;
		bso::sBool AgentEdition_;
		bso::sBool ShowAgentPassword_;
		sFolder CurrentFolder_;
	public:
		void reset( bso::bool__ P = true )
		{	
			tol::reset( P, Core_, AgentEdition_, ShowAgentPassword_ );
			CurrentAgent_ = UndefinedAgent;
			CurrentFolder_ = UndefinedFolder;
		}
		E_CVDTOR( rUser );
		void Init( frdfrntnd::rFrontend &Frontend )
		{
			if ( Frontend.IsConnected() )
				Core_.Init( Frontend );

			CurrentAgent_ = UndefinedAgent;
			AgentEdition_ = ShowAgentPassword_ = false;
			CurrentFolder_ = UndefinedFolder;
		}
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
			if ( Core_.Login( Username, Password ) ) {
				return true;
			} else 
				return false;

		}
		void DumpAgents( xml::dWriter &Writer );
		void DumpCurrentAgent( xml::dWriter &Writer );
		// Write only attributes (only usable on a start tag).
		void DumpMails( xml::dWriter &Writer );
		void DumpFolders( xml::dWriter &Writer );
		const str::dString &GetAgentStatus( str::dString &Status );
		void SelectFolder( sFolder Folder )
		{
			CurrentFolder_ = Folder;
		}
		void PutAgentStatusAttribute(
			const char *Name,
			xml::dWriter &Writer );
		void DefineAgent( void )
		{
			CurrentAgent_ = UndefinedAgent;
			AgentEdition_ = true;
		}
		void DiscardAgent( void )
		{
			CurrentAgent_ = UndefinedAgent;
			AgentEdition_ = false;
		}
		void UpdateAgent(
			const dString &Name,
			const dString &HostPort,
			const dString &Username,
			bso::sBool PasswordIsSet,
			const dString &Password )
		{
			if ( CurrentAgent_ != UndefinedAgent )
				Core_.UpdateAgent( CurrentAgent_, Name, HostPort, Username, PasswordIsSet, Password );
			else 
				CurrentAgent_ = Core_.CreateAgent( Name, HostPort, Username, Password );

			AgentEdition_ = false;
		}
		void SelectAgent( sAgent Agent )
		{
			CurrentAgent_ = Agent;
			AgentEdition_ = false;
		}
		void EditAgent( void )
		{
			AgentEdition_ = true;
		}
		qRWDISCLOSEr( bso::sBool, ShowAgentPassword );
	};
}

#endif
