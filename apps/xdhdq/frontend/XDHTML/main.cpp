/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "core.h"
#include "registry.h"
#include "sclfrntnd.h"

#include "Fields.h"

namespace{
	qCDEF( char *, XSLAffix_, "Main" );
	qCDEF( char *, FieldsFrameId_, "Fields" );

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
	fields::SetLayout( FieldsFrameId_, Session );
}

void main::Display( core::rSession &Session )
{
	SetLayout( Session );
	Session.AddClass( "Input", "hidden" );

	Session.SwitchTo( base::pMain );
}

void main::DisplayFields( core::rSession &Session )
{
	fields::Display( FieldsFrameId_, Session );
}

#define AC( name ) BASE_AC( main, name )

AC( ShowTestButton )
{
	Session.User.TestButtonIsVisible() = true;

	Session.RemoveClass( "Input", "hidden" );
}

AC( HideTestButton )
{
	Session.User.TestButtonIsVisible() = false;

	Session.AddClass( "Input", "hidden" );
}

AC( Testing )
{
	qRGnr();
}

