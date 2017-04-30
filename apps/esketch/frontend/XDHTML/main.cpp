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

namespace{
	qCDEF( char *, XSLAffix_, "Main" );
}

namespace layout_ {
	void Get(
		core::rSession &Session,
		xml::dWriter &Writer )
	{
	}
}

void main::SetLayout( core::rSession &Session )
{
qRH
	base::rLayoutRack Rack;
qRB
	Rack.Init( XSLAffix_, Session );

	layout_::Get( Session, Rack() );

	base::SetDocumentLayout( XSLAffix_, Session.Registry(), Rack, Session );
qRR
qRT
qRE
}

namespace casting_ {
	void Get(
		core::rSession &Session,
		xml::dWriter &Writer )
	{
		Writer.PushTag( "Test" );

		Writer.PutAttribute( "Enabled", ( Session.User.TestButtonIsVisible() ? "true" : "false" ) );
	}
}

void main::SetCasting( core::rSession &Session )
{
qRH
	base::rCastingRack Rack;
qRB
	Rack.Init( XSLAffix_, Session );
	casting_::Get( Session, Rack() );

	base::SetDocumentCasting( XSLAffix_, Session.Registry(), Rack, Session );
qRR
qRT
qRE
}

void main::Display( core::rSession &Session )
{
qRH
qRB
	SetLayout( Session );

	SetCasting( Session );

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
	SetCasting( Session );
}

AC( HideTestButton )
{
	Session.User.TestButtonIsVisible() = false;
	SetCasting(Session );
}

AC( Testing )
{
	Session.User.TestMessage();
}

