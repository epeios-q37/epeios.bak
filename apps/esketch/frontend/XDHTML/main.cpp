/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

namespace {
	qCDEF( char *, XSLAffix_, "Main" );

	void GetCasting_(
		core::rSession &Session,
		base::rCastingRack &Rack )
	{
		Rack().PushTag( "Test" );

		Rack().PutAttribute( "Enabled", (Session.User.TestButtonIsVisible() ? "true" : "false" ) );
	}

	void SetCasting_( core::rSession &Session )
	{
	qRH
		base::rCastingRack Rack;
		str::string XSL;
	qRB
		Rack.Init( XSLAffix_, Session);
		GetCasting_( Session, Rack );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

		Session.FillDocumentCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetLayout_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rLayoutRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}
}

void main::Display( core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetLayout_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillDocument( XML, XSL );

	SetCasting_( Session );

	Session.SwitchTo( core::pMain );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( main, name )

AC( Submission )
{
	str::string Test;
	TOL_CBUFFER___ Buffer;

	Test.Init( Session.GetContent( "Pattern", Buffer ) );

	Session.User.ToUpper( Test );

	Session.SetContent("Pattern", Test );
}

AC( ShowTestButton )
{
	Session.User.TestButtonIsVisible() = true;
	SetCasting_( Session );
}

AC( HideTestButton )
{
	Session.User.TestButtonIsVisible() = false;
	SetCasting_(Session );
}

AC( Testing )
{
	Session.User.TestMessage();
}

