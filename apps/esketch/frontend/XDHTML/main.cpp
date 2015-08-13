/*
	'main.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

E_CDEF( char *, XSLAffix_, "Main" );

static void GetContent_(
	const sclrgstry::registry_ &Registry,
	core::session___ &Session,
	str::string_ &XML )
{
qRH
	base::content_rack___ Rack;
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
qRB
	Rack.Init( XSLAffix_, XML, Session );

	Rack().PushTag( "Test" );

	Rack().PutAttribute("Enabled", (Session.User.TestButtonIsVisible() ? "true" : "false" ) );
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

BASE_AC( main::submission__ )
{
	str::string Test;
	TOL_CBUFFER___ Buffer;

	Test.Init( Session.GetContent( "Pattern", Buffer ) );

	Session.User.ToUpper( Test );

	Session.SetContent("Pattern", Test );
}

BASE_AC( main::show_test_button__ )
{
	Session.User.TestButtonIsVisible() = true;
	SetCasting( Session );
}

BASE_AC( main::hide_test_button__ )
{
	Session.User.TestButtonIsVisible() = false;
	SetCasting(Session );
}

BASE_AC( main::testing__ )
{
	Session.User.TestMessage();
}

