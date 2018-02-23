/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'mmuaqcli'.

    'mmuaqcli' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mmuaqcli' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mmuaqcli'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

#include "frdfrntnd.h"

#include "sclc.h"
#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

# define NAME_MC			"mmuaqcli"
# define NAME_LC			"mmuaqcli"
# define NAME_UC			"MMUAQCLI"
# define WEBSITE_URL		"http://q37.info"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

namespace {
	namespace {
		using namespace frdfrntnd;

		class rRack_ {
		private:
			frdfrntnd::rKernel Kernel_;
			sclfrntnd::sReportingCallback ReportingCallback_;
			void Connect_( void )
			{
			qRH
				fblfrd::incompatibility_informations IncompatibilityInformations;
				sclfrntnd::rFeatures Features;
			qRB
				IncompatibilityInformations.Init();
				if ( !Frontend.Connect( fblfrd::compatibility_informations__( MUAINF_LC_AFFIX, MMUAQ_API_VERSION ), IncompatibilityInformations ) )
					qRGnr();

			qRR
			qRE
			qRT
			}

			void Login_( void )
			{
			qRH
				str::wString UserID, Password;
			qRB
				tol::Init( UserID, Password );
				if ( !sclmisc::OGetValue( registry::parameter::login::UserID, UserID )
					 || ( UserID.Amount() == 0 ) 
					 || !sclmisc::OGetValue( registry::parameter::login::Password, Password ) )
					sclc::ReportAndAbort("BadOrMissingRegistryLoginEntries" );

				if ( !Frontend.Login( UserID, Password ) )
					sclmisc::ReportAndAbort( "LoginFailed" );
			qRR
			qRT
			qRE
			}
		public:
			frdfrntnd::rFrontend Frontend;
//			frdfrntnd::rMyObject Object;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, ReportingCallback_, Kernel_, Frontend );	//, Object  );
			}
			qCDTOR( rRack_ );
			void Init( void )
			{
			qRH
				sclfrntnd::rFeatures Features;
			qRB
				Features.Init();
				sclfrntnd::GuessBackendFeatures( Features );

				Kernel_.Init( Features, plgn::EmptyAbstracts );
				ReportingCallback_.Init();
				Frontend.Init( Kernel_, sclmisc::GetBaseLanguage(), ReportingCallback_ );
				Connect_();
				Login_();
//				Object.Init( Frontend );
			qRR
			qRT
			qRE
			}
			void GetKernelFeatures(
				str::dString &Plugin,
				str::dString &PluginParameters )
			{
				return Kernel_.GetFeatures( Plugin, PluginParameters );
			}
		};
	}

	void AboutBackend_( void )
	{
	qRH
		str::wString ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
		rRack_ Rack;
	qRB
		Rack.Init();

		tol::Init( ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );
		Rack.GetKernelFeatures( Plugin, PluginParameters );
		Rack.Frontend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

		cio::COut << Plugin << " '" << PluginParameters << "' : " << txf::nl << BackendInformations << txf::nl;
	qRR
	qRT
	qRE
	}

	void Check_( void )
	{
	qRH
		rRack_ Rack;
	qRB
		Rack.Init();

		Rack.Frontend.Check();
	qRR
	qRE
	qRT
	}

	/*
	void Test_( void )
	{
	qRH
		rRack_ Rack;
	qRB
		Rack.Init();

		Rack.Object.TestMessage();
	qRR
	qRE
	qRT
	}
	*/

#define G( name )	sclmisc::MGetValue( registry::parameter::name, name )

	namespace {
		// Get protocol from registry.
		eProtocol GetProtocol_( void )
		{
			eProtocol Protocol = p_Undefined;
		qRH
			str::wString Pattern;
		qRB
			tol::Init( Pattern);
			sclmisc::MGetValue( registry::parameter::Protocol, Pattern );

			Protocol = GetProtocol( Pattern );

			if ( Protocol == p_Undefined )
				sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::Protocol ) ;
		qRR
		qRT
		qRE
			return Protocol;
		}
	}

	void CreateAgent_( void )
	{
	qRH
		str::wString Name, HostPort, Username, Password;
		rRack_ Rack;
	qRB
		tol::Init( Name, HostPort, Username, Password );
		G( Name );
		G( HostPort );
		G( Username );
		G( Password );

		Rack.Init();
		Rack.Frontend.CreateAgent( Name, GetProtocol_(), HostPort, Username, Password );
	qRR
	qRE
	qRT
	}

	void UpdateAgent_( void )
	{
	qRH
		agent::sId Agent = agent::Undefined;
		str::wString Name, HostPort, Username, Password;
		bso::sBool PasswordAvailable = false;
		rRack_ Rack;
	qRB
		*Agent = sclmisc::MGetUInt( registry::parameter::Agent );

		tol::Init( Name, HostPort, Username, Password );
		G( Name );
		G( HostPort );
		G( Username );

		PasswordAvailable = sclmisc::OGetValue( registry::parameter::Password, Password );

		Rack.Init();
		Rack.Frontend.UpdateAgent( Agent, Name, HostPort, Username, PasswordAvailable, Password );
	qRR
	qRE
	qRT
	}

	void RemoveAgent_( void )
	{
	qRH
		agent::sId Agent = agent::Undefined;
		rRack_ Rack;
	qRB
		*Agent = sclmisc::MGetUInt( registry::parameter::Agent );

		Rack.Init();
		Rack.Frontend.RemoveAgent( Agent );
	qRR
	qRE
	qRT
	}

#undef G

	namespace agents_ {
		void Display(
			const dAgents &Agents,
			const str::dStrings &Names,
			const fbltyp::dBooleans &Enabled,
			txf::sWFlow &Flow )
		{
			sdr::sRow Row = Agents.First();

			if ( !tol::HaveSameAmount( Agents, Names, Enabled ) )
				qRGnr();

			while ( Row != qNIL ) {
				Flow << *Agents(Row);
				
				if ( Enabled( Row ) )
					Flow << ':';
				else
					Flow << '!';

				Flow << ' ' << Names( Row ) << txf::nl;

				Row = Agents.Next( Row );
			}
		}
	}
	void Agents_( void )
	{
	qRH
		rRack_ Rack;
		agent::wIds Agents;
		str::wStrings Names;
		fbltyp::wBooleans Enabled;
	qRB
		Rack.Init();

		tol::Init( Agents, Names, Enabled );
		Rack.Frontend.GetAgents( Agents, Names, Enabled );

		agents_::Display( Agents, Names, Enabled, cio::COut );
	qRR
	qRE
	qRT
	}

	void Agent_( void )
	{
	qRH
		rRack_ Rack;
		agent::sId Agent = agent::Undefined;
		str::wString Name, HostPort, Username;
		bso::sBool Enabled = false;
		eProtocol Protocol = p_Undefined;
	qRB
		*Agent = sclmisc::MGetUInt( registry::parameter::Agent );

		tol::Init( Rack, Name, HostPort, Username );
		Rack.Frontend.GetAgent( Agent, Name, Protocol, HostPort, Username, Enabled );

		cio::COut << Name;
		
		if ( Enabled )
			cio::COut << ':';
		else
			cio::COut << '!';

		cio::COut << ' ' << GetLabel( Protocol ) << ' ' << Username << '@' << HostPort << txf::nl;
	qRR
	qRE
	qRT
	}

	void CreateFolder_( void )
	{
	qRH
		folder::sId Root = folder::Undefined, Inbox = folder::Undefined, Parent = folder::Undefined;
		str::wString Name;
		rRack_ Rack;
	qRB
		Rack.Init();
		Rack.Frontend.GetRootAndInboxFolders( Root, Inbox );

		*Parent = sclmisc::OGetUInt( registry::parameter::Parent, **Root );

		Name.Init();
		sclmisc::MGetValue( registry::parameter::Name, Name );

		Rack.Frontend.CreateFolder( Parent, Name );
	qRR
	qRE
	qRT
	}

	void UpdateFolder_( void )
	{
	qRH
		folder::sId Folder = folder::Undefined;
		str::wString Name;
		rRack_ Rack;
	qRB
		*Folder = sclmisc::MGetUInt( registry::parameter::Folder );

		Name.Init();
		sclmisc::MGetValue( registry::parameter::Name, Name );


		Rack.Init();
		Rack.Frontend.UpdateFolder( Folder, Name );
	qRR
	qRE
	qRT
	}

	namespace folders_ {
		namespace {
			void Display_(
				const char *Label,
				txf::sWFlow &Flow )
			{
			qRH
				str::wString Message;
			qRB
				Message.Init();
				Flow << "! " << sclmisc::GetBaseTranslation( Label, Message );
			qRR
			qRT
			qRE
			}
		}

		void Display(
			const dFolders &Folders,
			const str::dStrings &Names,
			sFolder Root,
			sFolder Inbox,
			txf::sWFlow &Flow )
		{
			folder::sId Folder = folder::Undefined;
			sdr::sRow Row = Folders.First();

			if ( Folders.Amount() != Names.Amount() )
				qRGnr();

			while ( Row != qNIL ) {
				Folder = Folders( Row );

				Flow << **Folder;

				if ( Folder == Root )
					Display_( "RootFolderName", Flow );
				else if ( Folder == Inbox )
					Display_( "InboxFolderName", Flow );
				else
					Flow << ": " << Names( Row );
				
				Flow << txf::nl;

				Row = Folders.Next( Row );
			}
		}
	}

	void Folders_( void )
	{
	qRH
		rRack_ Rack;
		sFolder Root = folder::Undefined, Inbox = folder::Undefined, Folder = folder::Undefined;
		wFolders Folders;
		str::wStrings Names;
	qRB
		Rack.Init();
		Rack.Frontend.GetRootAndInboxFolders( Root, Inbox );

		*Folder = sclmisc::OGetUInt( registry::parameter::Folder, **Root );

		Folders.Init();
		Rack.Frontend.GetFolders( Folder, Folders );

		Names.Init();
		Rack.Frontend.GetFoldersNames( Folders, Names );

		folders_::Display( Folders, Names, Root, Inbox, cio::COut );
	qRR
	qRE
	qRT
	}

	void MoveFolder_( void )
	{
	qRH
		rRack_ Rack;
		sFolder Root = folder::Undefined, Inbox = folder::Undefined, Folder = folder::Undefined, Parent = folder::Undefined;
	qRB
		Rack.Init();
		Rack.Frontend.GetRootAndInboxFolders( Root, Inbox );

		*Parent = sclmisc::OGetUInt( registry::parameter::Parent, **Root );
		*Folder = sclmisc::MGetUInt( registry::parameter::Folder );

		Rack.Frontend.MoveFolderTo( Folder, Parent );
	qRR
	qRE
	qRT
	}

	namespace  mails_ {
		void Display(
			const mail::dIds &Mails,
			const str::dStrings &Subjects,
			const agent::dIds &Agents,
			txf::sWFlow &Flow )
		{
			if ( Mails.Amount() != Subjects.Amount() )
				qRGnr();

			if ( Mails.Amount() != Agents.Amount() )
				qRGnr();

			sdr::sRow Row = Mails.First();

			while ( Row != qNIL ) {
				Flow << *Mails(Row) << " (" << *Agents(Row) << "): " << Subjects( Row ) << txf::nl;

				Row = Mails.Next( Row );
			}
		}
	}

	void Mails_( void )
	{
	qRH
		rRack_ Rack;
		sFolder Root = folder::Undefined, Inbox = folder::Undefined, Folder = folder::Undefined;
		mail::wIds Mails;
		str::wStrings Subjects;
		agent::wIds Agents;
	qRB
		Rack.Init();
		Rack.Frontend.GetRootAndInboxFolders( Root, Inbox );

		*Folder = sclmisc::OGetUInt( registry::parameter::Folder, **Root );

		tol::Init( Mails, Subjects, Agents );
		Mails.Init();
		Rack.Frontend.GetMailsFields( Folder, Mails, Subjects, Agents );

		mails_::Display( Mails, Subjects, Agents, cio::COut );
	qRR
	qRE
	qRT
	}

	void Mail_( void )
	{
	qRH
		mail::sId Mail = mail::Undefined;
		rRack_ Rack;
		str::wString Content;
	qRB
		*Mail = sclmisc::MGetUInt( registry::parameter::Mail );

		Rack.Init();
		Content.Init();
		Rack.Frontend.GetMail( Mail, Content );

		cio::COut << Content;
	qRR
	qRE
	qRT
	}

	void MoveMail_( void )
	{
	qRH
		folder::sId Folder = folder::Undefined;
		mail::sId Mail = mail::Undefined;
		rRack_ Rack;
	qRB
		*Folder = sclmisc::MGetUInt( registry::parameter::Folder );
		*Mail = sclmisc::MGetUInt( registry::parameter::Mail );

		Rack.Init();
		Rack.Frontend.MoveMailTo( Mail, Folder );
	qRR
	qRE
	qRT
	}


}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH
qRB
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( AboutBackend );
	C( Check );
	C( CreateAgent );
	C( UpdateAgent );
	C( RemoveAgent );
	C( Agents );
	C( Agent );
	C( CreateFolder );
	C( UpdateFolder );
	C( MoveFolder );
	C( Folders );
	C( Mails );
	C( Mail );
	C( MoveMail );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR
qRT
qRE
	return ExitValue;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

