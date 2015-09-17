/*
	'main.cpp' by Claude SIMON (http://q37.info/contact/).

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

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

E_CDEF( char *, XSLAffix_, "Main" );

using namespace frdinstc;

static void GetContent_(
	const sclrgstry::registry_ &Registry,
	core::session___ &Session,
	str::string_ &XML )
{
qRH
	base::content_rack___ Rack;
	int i = 0;
qRB
	Rack.Init( XSLAffix_, XML, Session );
qRR
qRT
qRE
}

void main::SetLayout( core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.SetDocument( XML, XSL );

	SetCasting( Session );

	fields::SetLayout( Session );

	Session.SwitchTo( core::pMain );
qRR
qRT
qRE
}

static void GetContext_(
	core::session___ &Session,
	str::string_ &XML )
{
qRH
	base::context_rack___ Rack;
	str::string Buffer;
qRB
	Rack.Init( XSLAffix_, XML, Session );

	Rack().PushTag( "Facetious" );

	Buffer.Init();
	Rack().PutAttribute("Enabled", (Session.User.FacetiousButtonIsVisible() || ( Session.GetProperty( "Freeze", "checked", Buffer ) == "true" ) ? "true" : "false" ) );
qRR
qRT
qRE
}

void main::SetCasting( core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContext_( Session,  XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

	Session.SetDocumentCasting( XML, XSL );
qRR
qRT
qRE
}

BASE_AC( main::show_facetious_button__ )
{
	Session.User.FacetiousButtonIsVisible() = true;
	SetCasting( Session );
}

BASE_AC( main::hide_facetious_button__ )
{
	Session.User.FacetiousButtonIsVisible() = false;
	SetCasting(Session );
}

BASE_AC( main::surrender__ )
{
	Session.AlertT( "mainSurrenderMessage" );
}
