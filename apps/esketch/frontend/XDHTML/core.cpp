/*
	'core.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "registry.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclfrntnd.h"

using namespace core;

core::core___ core::Core;

void core::instances_core___::Init( frdfrntnd::frontend___ &Frontend )
{
qRH
qRB
	User.Init( Frontend );
qRR
qRT
qRE
}

bso::bool__ core::session___::XDHCBKLaunch(
	const char *Id,
	const char *Action )
{
	return Core.Launch( *this, Id, Action );
}

void core::session___::SCLXDHTMLRefresh( page__ Page )
{
	switch ( Page ) {
	case pProlog:
		prolog::SetLayout( *this );
		break;
	case pLogin:
		login::SetLayout( *this );
		break;
	case pMain:
		main::SetLayout( *this );
		break;
	default:
		qRGnr();
		break;
	}
}

#define V( value, tag )	Writer.PutValue( value, #tag );

static void About_(
	sclfrntnd::frontend___ &Frontend,
	xml::writer_ &Writer )
{
qRH
	str::string ProtocolVersion, BackendLocation, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations;
qRB
	ProtocolVersion.Init();
	BackendLocation.Init();
	BackendLabel.Init();
	APIVersion.Init();
	BackendInformations.Init();
	BackendCopyright.Init();
	SoftwareInformations.Init();

	sclfrntnd::GetBackendLocation( BackendLocation );
	Frontend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations, BackendCopyright, SoftwareInformations );

	V( ProtocolVersion, ProtocolVersion );
	V( BackendLocation, Location );
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
	session___ &Session,
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
