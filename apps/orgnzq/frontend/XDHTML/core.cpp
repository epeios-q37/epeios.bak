/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core.h"

#include "registry.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclfrntnd.h"

using namespace core;

core::core___ core::Core;

sclxdhtml::rActionHelper core::OnNotConnectedAllowedActions;
sclxdhtml::rActionHelper core::OnFieldDefiningAllowedActions;

namespace {
	sclfrntnd::rKernel Kernel_;
}

sclfrntnd::rKernel &core::Kernel( void )
{
	return Kernel_;
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

void core::sDump::Corpus(
	core::rSession &Session,
	xml::dWriter &Writer )
{
	Session.DumpCorpus( Writer );
}

void core::sDump::Common(
	core::rSession &Session,
	xml::dWriter &Writer )
{
	if ( Session.User.HasPanel() )
		if ( Session.User.Panel().GetFocus() != frdinstc::t_Undefined )
			Writer.PutAttribute( "Focus", frdinstc::GetLabel( Session.User.Panel().GetFocus() ) );
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

namespace {
	void InitActionHelpers_( void )
	{
		tol::Init( OnNotConnectedAllowedActions, OnFieldDefiningAllowedActions );
	}
}

qGCTOR( core )
{
	InitActionHelpers_();
}