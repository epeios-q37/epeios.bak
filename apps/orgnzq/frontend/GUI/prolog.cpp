/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "prolog.h"

#include "core.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	E_CDEF(char *, XSLAffix_, "Prolog" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			sclxdhtml::prolog::GetLayout( Session, Writer );
		}
	}

	namespace casting_ {
		void Get(
			core::rSession &Session,
			xml::dWriter &Writer )
		{
			sclxdhtml::prolog::GetCasting( Session, Writer );
		}
	}
}

void prolog::SetLayout( core::rSession &Session )
{
	core::SetDocumentLayout( XSLAffix_, layout_::Get, Session );
}

void prolog::SetCasting( core::rSession &Session )
{
	core::SetDocumentCasting( XSLAffix_, casting_::Get, Session );
}

void prolog::Display( core::rSession &Session )
{
	SetLayout( Session );

	SetCasting( Session );

	Session.SwitchTo( core::pProlog );
}

#define AC( name ) BASE_AC( prolog, name )

AC( SwitchProjectType )
{
	SetCasting( Session );
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
