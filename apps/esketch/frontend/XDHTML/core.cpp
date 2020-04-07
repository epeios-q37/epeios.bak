/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core.h"

#include "global.h"
#include "login.h"
#include "main.h"
#include "prolog.h"
#include "registry.h"

#include "sclr.h"
#include "scll.h"
#include "sclf.h"

using namespace core;

core::rCore core::Core;

sclx::rActionHelper core::OnNotConnectedAllowedActions;

namespace {
	sclf::rKernel Kernel_;
}

sclf::rKernel &core::Kernel( void )
{
	return Kernel_;
}

namespace {
	void Register_( void )
	{
		global::Register();
		prolog::Register();
		login::Register();
		main::Register();

		OnNotConnectedAllowedActions.Add(
			xdhcmn::CloseActionLabel,
			global::OnNewSession, global::About, global::Test,
			prolog::DisplayProjectFilename, prolog::LoadProject, prolog::SwitchProjectType,	// All 'prolog'-related actions are allowed.
			login::Dismiss, login::DisplayEmbeddedBackendFilename, login::Connect, login::SwitchBackendType,	// All 'login'-related actions too.
			main::HideTestButton, main::ShowTestButton, main::UnconnectedTesting );
	};
}

BASE_AD( OnNewSession_ );

void core::rCore::Init( xdhcmn::eMode Mode )
{
	ActionHelperCallback_.Init();
	sclx::rCore<rSession>::Init( Mode, global::OnNewSession, ActionHelperCallback_ );
	Register_();
}

void core::rInstances::Init( frdfrntnd::rFrontend &Frontend )
{
qRH
qRB
	User.Init( Frontend );
qRR
qRT
qRE
}

void core::sDump::Corpus(
	core::rInstances &Session,
	xml::rWriter &Writer )
{}

void core::sDump::Common(
	core::rInstances &Session,
	xml::rWriter &Writer )
{}

#define V( value, tag )	Writer.PutValue( value, #tag );

static void About_(
	sclf::rFrontend &Frontend,
	xml::rWriter &Writer )
{
qRH
	str::wString ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
qRB
	tol::Init( ProtocolVersion, Plugin, PluginParameters, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

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
	xml::rWriter &Writer )
{
qRH
	str::string Build;
qRB

	V( sclx::GetLauncher(), Launcher );

	Writer.PushTag("Software" )	;
	Writer.PutValue( SOFTWARE_NAME, "Name" );
	Writer.PutValue( SOFTWARE_VERSION, "Version" );
	Writer.PopTag();

	Writer.PushTag( "Frontend" );

	V( NAME, Name );
	V( VERSION, Version );

	Build.Init();
	Build.Append( __DATE__ " " __TIME__ );
	Build.Append( " - " );
	Build.Append( cpe::GetDescription() );
	V( Build, Build );


	Writer.PushTag( "Author" );
	V( AUTHOR_NAME, Name );
	V( AUTHOR_CONTACT, Contact );
	Writer.PopTag();
	Writer.PushTag( "Copyright" );
	V( COPYRIGHT_YEARS, Years );
	Writer.PushTag( "Owner ");
	V( OWNER_NAME, Name );
	V( OWNER_CONTACT, Contact );
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

bso::bool__ core::sActionHelper::SCLXOnBeforeAction(
	rSession &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !core::OnNotConnectedAllowedActions.Search( Action ) ) {
			Session.AlertT("ActionNeedsBackend", "ActionNeedsBackendTitle");
			return false;
		} else
			return true;
	} else
		return true;
}

void core::sActionHelper::SCLXOnRefresh( rSession &Session )
{
	switch ( Session.Page() ) {
	case base::pProlog:
		prolog::Display( Session );
		break;
	case base::pLogin:
		login::Display( Session );
		break;
	case base::pMain:
		main::Display( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

bso::bool__ core::sActionHelper::SCLXOnClose( rSession &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

qGCTOR( core )
{
	OnNotConnectedAllowedActions.Init();
}
