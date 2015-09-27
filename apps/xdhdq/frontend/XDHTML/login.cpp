/*
	'login.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "login.h"

#include "core.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	E_CDEF( char *, XSLAffix_, "Login" );

	void GetContext_(
		core::session___ &Session,
		str::string_ &XML )
	{
	qRH
		base::context_rack___ Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::login::GetContext( Session, Rack );
	qRR
	qRT
	qRE
	}

	void SetCasting_( core::session___ &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::XSLCastingFile, XSLAffix_ ), sclxdhtml::GetRegistry() , XSL );	// Outside session, so we use the global registry...

		Session.FillCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::session___ &Session,
		str::string_ &XML )
	{
	qRH
		base::content_rack___ Rack;
		TOL_CBUFFER___ Buffer;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		sclxdhtml::login::GetContent( Session, Rack );
	qRR
	qRT
	qRE
	}
}

void login::SetLayout( core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContent_( sclxdhtml::GetRegistry(), Session, XML );	// Outside session, so we use the global registry...

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::XSLLayoutFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.FillDocument( XML, XSL );

	SetCasting_( Session );

	Session.SwitchTo( core::pLogin );
qRR
qRT
qRE
}

BASE_AC( login::switch_backend_type__ )
{
	SetCasting_( Session );
}

BASE_AC( login::display_embedded_backend_filename__ )
{
	sclxdhtml::login::DisplaySelectedEmbeddedBackendFilename( Session, Id );
}

BASE_AC( login::connect__ )
{
qRH
	TOL_CBUFFER___ Buffer;
	fblfrd::incompatibility_informations IncompatibilityInformations;
	sclfrntnd::backend_type__ BackendType = sclfrntnd::bt_Undefined;
	str::string BackendFeature;
qRB
	if ( core::Core.Mode() == xdhcmn::mMonoUser ) {
		BackendFeature.Init();
		BackendType = sclxdhtml::login::GetBackendFeatures( Session, BackendFeature );
		sclxdhtml::Connect( BackendType, BackendFeature );
	}

	IncompatibilityInformations.Init();
	if ( !Session.Connect( fblfrd::compatibility_informations__( XDDINF_LC_AFFIX, XDHDQ_API_VERSION ), IncompatibilityInformations ) )
		qRGnr();

	main::SetLayout( Session );
qRR
qRE
qRT
}

BASE_AC( login::dismiss__ )
{
	prolog::SetLayout( Session );
}

