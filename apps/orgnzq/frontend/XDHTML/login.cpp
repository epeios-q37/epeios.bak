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

#include "login.h"

#include "core.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {

	E_CDEF( char *, XSLAffix_, "Login" );

	void GetContext_(
		core::rSession &Session,
		sclxdhtml::login::eBackendVisibility Visibility,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::login::GetContext( Session, Visibility, Rack );
	qRR
	qRT
	qRE
	}

	void SetCasting_(
		core::rSession &Session,
		sclxdhtml::login::eBackendVisibility Visibility )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session, Visibility, XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), sclxdhtml::GetRegistry() , XSL );	// Outside session, so we use the global registry...

		Session.FillDocumentCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetContent_(
		const sclrgstry::dRegistry &Registry,
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

void login::SetLayout(
	core::rSession &Session,
	sclxdhtml::login::eBackendVisibility Visibility )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContent_( sclxdhtml::GetRegistry(), Session, XML );	// Outside session, so we use the global registry...

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.FillDocument( XML, XSL );

	SetCasting_( Session, Visibility );

	Session.SwitchTo( core::pLogin );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( login, name )

AC( SwitchBackendType )
{
	SetCasting_( Session, sclxdhtml::login::bvShow );	// Show, because this action could not be made if the backend-related form is hidden.
}

AC( DisplayEmbeddedBackendFilename )
{
	sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename( Session, Id );
}

AC( Connect )
{
qRH
	fblfrd::incompatibility_informations IncompatibilityInformations;
	sclfrntnd::features___ Features;
	str::wString UserID, Password;
qRB
	if ( core::Core.Mode() == xdhcmn::mMonoUser ) {
		Features.Init();
		sclxdhtml::login::GetBackendFeatures( Session, Features );
		core::Kernel().Init( Features, plgn::EmptyAbstracts );
	}

	IncompatibilityInformations.Init();
	if ( !Session.Connect( fblfrd::compatibility_informations__( OGZINF_LC_AFFIX, ORGNZQ_API_VERSION ), IncompatibilityInformations ) )
		qRGnr();

	tol::Init( UserID, Password );
	if ( !Session.User.Login( Session.GetContent("LoginUserID", UserID ), Session.GetContent( "LoginPassword", Password ) ) ) {
		Session.AlertT( "UnableToLogin" );
		Session.Disconnect();
	} else {
		main::SetLayout( Session );
		main::SetRecordsLayout( Session );
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

