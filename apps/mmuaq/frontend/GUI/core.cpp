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

#include "core.h"

#include "agent.h"
#include "agents.h"
#include "config.h"
#include "folders.h"
#include "mail.h"
#include "mails.h"
#include "global.h"
#include "prolog.h"
#include "login.h"
#include "main.h"
#include "config.h"
#include "registry.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclfrntnd.h"

using namespace core;

qCDEF(char *, core::folder::NameInputId, "FolderName");

core::core___ core::Core;

sclxdhtml::rActionHelper core::OnNotConnectedAllowedActions;
sclxdhtml::rActionHelper core::OnFolderEditionIgnoredActions;

namespace {
	sclfrntnd::rKernel Kernel_;

	void InitActionHelpers_( void )
	{
		OnNotConnectedAllowedActions.Init();
		OnFolderEditionIgnoredActions.Init();
	}
}

sclfrntnd::rKernel &core::Kernel( void )
{
	return Kernel_;
}

namespace {
	void Register_( void )
	{
		agent::Register();
		agents::Register();
		config::Register();
		folders::Register();
		mail::Register();
		mails::Register();
		global::Register();
		prolog::Register();
		login::Register();
		main::Register();

		OnNotConnectedAllowedActions.Add(
			xdhcmn::CloseActionLabel,
			global::About, global::Test,
			prolog::DisplayProjectFilename, prolog::LoadProject, prolog::SwitchProjectType,	// All 'prolog'-related actions are allowed.
			login::Dismiss, login::DisplayEmbeddedBackendFilename, login::Connect, login::SwitchBackendType );	// All 'login'-related actions too.

		OnFolderEditionIgnoredActions.Add( folders::DiscardFolder );	// No 'folders::ApplyFolder', because it does nothing more as for the other actions.
	};
}

void core::core___::Init( xdhcmn::mode__ Mode )
{
	_ActionHelperCallback.Init();
	_core___::Init( Mode, _ActionHelperCallback );
	Register_();
}

void core::rInstancesCore::Init( frdfrntnd::rFrontend &Frontend )
{
qRH
qRB
	User.Init( Frontend );
qRR
qRT
qRE
}

bso::bool__ core::rSession::XDHCMNLaunch(
	const char *Id,
	const char *Action )
{
	return Core.Launch( *this, Id, Action );
}

#define V( value, tag )	Writer.PutValue( value, #tag );

static void About_(
	sclfrntnd::rFrontend &Frontend,
	xml::writer_ &Writer )
{
qRH
	str::wString ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
qRB
	ProtocolVersion.Init();
	Plugin.Init();
	PluginParameters.Init();
	BackendLabel.Init();
	APIVersion.Init();
	BackendInformations.Init();
	BackendCopyright.Init();
	SoftwareInformations.Init();

	Kernel_.GetFeatures( Plugin, PluginParameters );
	Frontend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

	V( ProtocolVersion, ProtocolVersion );
	V( Plugin, Plugin );
	V( PluginParameters, PluginParameters );
	V( BackendLabel, Label );
	V( APIVersion, APIVersion );
	V( BackendInformations, Build );
	V( BackendCopyright, Copyright );
	V( SoftwareInformations, Software );
qRR
qRT
qRE
}

void core::About(
	rSession &Session,
	xml::writer_ &Writer )
{
qRH
	str::string Build;
qRB

	V( sclxdhtml::GetLauncher(), Launcher );

	Writer.PushTag("Software" )	;
	Writer.PutValue( BASE_SOFTWARE_NAME, "Name" );
	Writer.PutValue( BASE_SOFTWARE_VERSION, "Version" );
	Writer.PopTag();

	Writer.PushTag( "Frontend" );

	V( BASE_NAME, Name );
	V( VERSION, Version );

	Build.Init();
	Build.Append( __DATE__ " " __TIME__ );
	Build.Append( " - " );
	Build.Append( cpe::GetDescription() );
	V( Build, Build );


	Writer.PushTag( "Author" );
	V( BASE_AUTHOR_NAME, Name );
	V( BASE_AUTHOR_CONTACT, Contact );
	Writer.PopTag();
	Writer.PushTag( "Copyright" );
	V( BASE_COPYRIGHT_YEARS, Years );
	Writer.PushTag( "Owner ");
	V( BASE_OWNER_NAME, Name );
	V( BASE_OWNER_CONTACT, Contact );
	Writer.PopTag();
	Writer.PopTag();
	Writer.PopTag();

	if ( Session.IsConnected() ) {
		Writer.PushTag( "Backend" );
		About_( Session, Writer );
		Writer.PopTag();
	}
qRR
qRT
qRE
}

void core::SetAgentsLayout( rSession &Session )
{
	switch ( Session.Page() ) {
	case pConfig:
		config::SetAgentsLayout( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

void core::SetAgentCasting( rSession &Session )
{
	switch ( Session.Page() ) {
	case pConfig:
		config::SetAgentCasting( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

bso::sBool core::folder::Apply( rSession &Session )
{
qRH
	str::wString Name;
qRB
	Name.Init();
	Session.GetContent( NameInputId, Name );

	Session.User.ApplyFolder( Name );

	main::SetFoldersLayout( Session );
qRR
qRT
qRE
	return true;
}


qGCTOR( core )
{
	InitActionHelpers_();
}