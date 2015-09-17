/*
	'prolog.cpp' by Claude SIMON (http://q37.info/contact/).

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

#include "prolog.h"

#include "core.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

E_CDEF(char *, XSLAffix_, "Prolog" );

static void GetContent_(
	const sclrgstry::registry_ &Registry,
	core::session___ &Session,
	str::string_ &XML )
{
qRH
	base::content_rack___ Rack;
	TOL_CBUFFER___ Buffer;
qRB
	Rack.Init( XSLAffix_, XML, Session );

	sclxdhtml::prolog::GetContent( Session, Rack );
qRR
qRT
qRE
}

void prolog::SetLayout( core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContent_( sclxdhtml::GetRegistry(), Session, XML );	// Outside session, so we use the global registry...

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::XSLLayoutFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.SetDocument( XML, XSL );

	SetCasting( Session );
	Session.SwitchTo( core::pProlog );
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
qRB
	Rack.Init( XSLAffix_, XML, Session );

	sclxdhtml::prolog::GetContext( Session, Rack );
qRR
qRT
qRE
}

void prolog::SetCasting( core::session___ &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContext_( Session,  XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::XSLCastingFile, XSLAffix_ ), sclxdhtml::GetRegistry(), XSL );	// Outside session, so we use the global registry...

	Session.SetDocumentCasting( XML, XSL );
qRR
qRT
qRE
}

BASE_AC( prolog::switch_project_type__ )
{
	SetCasting( Session );
}

BASE_AC( prolog::display_project_filename__ )
{
	sclxdhtml::prolog::DisplaySelectedProjectFilename( Session, Id );
}

BASE_AC( prolog::load_project__ )
{
	sclxdhtml::prolog::LoadProject( Session );

	login::SetLayout( Session );
}

