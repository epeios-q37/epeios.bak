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

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{}
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
}

void main::SetLayout( core::rSession &Session )
{
	Session.SetDocumentLayout( XSLAffix_, layout_::Get );
}

void main::Display( core::rSession &Session )
{
	SetLayout( Session );

	Session.SwitchTo( base::pMain );
}

#define AC( name ) BASE_AC( main, name )

namespace {
	void Submission_(
		const char *Id,
		core::rSession &Session )
	{
	qRH;
		str::wString Content;
	qRB;
		Content.Init();
		Session.GetValue( "Pattern", Content );
		Session.User.ToUpper( Content );
		Session.SetContent( "Pattern", Content );
	qRR;
	qRT;
	qRE;
	}
}

AC( Submission )
{
	Submission_( Id, Session );
}

AC( ShowTestButton )
{
	Session.User.TestButtonIsVisible() = true;
}

AC( HideTestButton )
{
	Session.User.TestButtonIsVisible() = false;
}

AC( Testing )
{
	Session.User.TestMessage();
}

