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

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			sclxdhtml::login::GetLayout( Session, Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			sclxdhtml::login::GetCasting( Session, Session.BackendVisibility(), Writer );
		}
	}
}

void login::SetLayout( core::rSession &Session )
{
	core::SetDocumentLayout( XSLAffix_, layout_::Get, Session );
}

void login::SetCasting( core::rSession &Session )
{
	core::SetDocumentCasting( XSLAffix_, casting_::Get, Session );
}

void login::Display( core::rSession &Session )
{
	SetLayout( Session );

	SetCasting( Session );

	Session.SwitchTo( core::pLogin );
}

#define AC( name ) BASE_AC( login, name )

AC( SwitchBackendType )
{
	SetCasting( Session );
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
	if ( !Session.Connect( fblfrd::compatibility_informations__( OGZINF_LC_AFFIX, ORGNZQ_API_VERSION ), IncompatibilityInformations ) )
		qRGnr();

	tol::Init( UserID, Password );
	if ( !Session.User.Login( Session.GetValue("LoginUserID", UserID ), Session.GetValue( "LoginPassword", Password ) ) ) {
		Session.AlertT( "UnableToLogin" );
		Session.Disconnect();
	} else {
		main::Display( Session );
		main::SetRecordsLayout( Session );
	}
qRR
	Session.Disconnect();
qRE
qRT
}

AC( Dismiss )
{
	prolog::Display( Session );
}

