/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "prolog.h"

#include "core.h"
#include "login.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	qCDEF( char *, XMLAffix_, "Prolog" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			sclxdhtml::prolog::GetLayout( Session, Writer );
		}
	}
}

void prolog::SetLayout( core::rSession &Session )
{
	Session.SetDocumentLayout( XMLAffix_, layout_::Get );
}

void prolog::Display( core::rSession &Session )
{
	SetLayout( Session );

	Session.SwitchTo( base::pProlog );

	sclxdhtml::prolog::HandleProjectTypeSwitching( Session );
}

#define AC( name ) BASE_AC( prolog, name )

AC( SwitchProjectType )
{
	sclxdhtml::prolog::HandleProjectTypeSwitching( Session );
}

AC( DisplayProjectFilename )
{
	sclxdhtml::prolog::DisplaySelectedProjectFilename( Session, Id );
}

AC( LoadProject )
{
	sclxdhtml::prolog::LoadProject( Session );

	login::Display( Session );
}

