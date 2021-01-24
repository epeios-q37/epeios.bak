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

namespace {
	qCDEF( char *, XMLAffix_, "Prolog" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{
			sclx::prolog::GetLayout( Session, Writer );
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
/*
	Session.Execute( "var s = document.createElement('link'); s.setAttribute('rel','stylesheet');s.setAttribute('href','http://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css');document.head.appendChild(s);" );
	Session.Execute("document.write('<script type=\"text/javascript\" src=\"https://code.jquery.com/jquery-1.12.4.js\"></script>')");
	Session.Execute( "var s = document.createElement('script'); s.setAttribute('src','https://code.jquery.com/ui/1.12.1/jquery-ui.js');document.head.appendChild(s);" );
*/
	Session.SwitchTo( base::pProlog );
	sclx::prolog::HandlePresetSwitching( Session );
}

#define A( name ) BASE_ADef( prolog, name )

A( SwitchPreset )
{
	sclx::prolog::HandlePresetSwitching( Session );
}

A( DisplayProjectFilename )
{
	sclx::prolog::DisplaySelectedProjectFilename( Session, Id );
}

A( LoadPreset )
{
	sclx::prolog::LoadPreset( Session );

	login::Display( Session );
}

