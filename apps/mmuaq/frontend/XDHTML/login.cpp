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

#include "login.h"

#include "core.h"
#include "main.h"
#include "prolog.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	qCDEF( char *, XSLAffix_, "Login" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::login::GetContext( Session, Session.BackendVisibility(), Rack );
	qRR
	qRT
	qRE
	}

	void FillCasting_( core::rSession &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), sclxdhtml::GetRegistry() , XSL );	// Outside session, so we use the global registry...

		Session.FillDocumentCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	static void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
		TOL_CBUFFER___ Buffer;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::login::GetContent( Session, Rack );
	qRR
	qRT
	qRE
	}
}

void login::SetLayout( core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContent_( sclxdhtml::GetRegistry(), Session, XML );	// Outside session, so we use the global registry...

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.FillDocument( XML, XSL );

	FillCasting_( Session );

	Session.SwitchTo( core::pLogin );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( login, name )

AC( SwitchBackendType )
{
	FillCasting_( Session );
}

AC( DisplayEmbeddedBackendFilename )
{
	sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename( Session, Id );
}

AC( Connect )
{
qRH
	fblfrd::incompatibility_informations IncompatibilityInformations;
	sclfrntnd::rFeatures Features;
	str::wString UserID, Password;
qRB
	if ( core::Core.Mode() == xdhcmn::mMonoUser ) {
		Features.Init();
		sclxdhtml::login::GetBackendFeatures( Session, Features );
		core::Kernel().Init( Features, plgn::EmptyAbstracts );
	}

	IncompatibilityInformations.Init();
	if ( !Session.Connect( fblfrd::compatibility_informations__( MUAINF_LC_AFFIX, MMUAQ_API_VERSION ), IncompatibilityInformations ) )
		qRGnr();

	tol::Init( UserID, Password );

	if ( !Session.User.Login( Session.GetContent("LoginUserID", UserID ), Session.GetContent( "LoginPassword", Password ) ) ) {
		Session.AlertT( "UnableToLogin" );
		Session.Disconnect();
	} else {
		main::SetLayout( Session );
	}
qRR
	Session.Disconnect();
qRE
qRT
}

AC( Dismiss )
{
	prolog::SetLayout( Session );
}

