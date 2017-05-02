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

#include "prolog.h"

#include "core.h"
#include "login.h"
#include "registry.h"

#include "xdhdws.h"

#include "sclfrntnd.h"

namespace {
	qCDEF( char *, XSLAffix_, "Prolog" );

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
qRH
	base::rLayoutRack Rack;
qRB
	Rack.Init( XSLAffix_, Session );

	layout_::Get( Session, Rack() );

	sclxdhtml::SetDocumentLayout( XSLAffix_, Session.Registry(), Rack, Session );
qRR
qRT
qRE
}

void prolog::SetCasting( core::rSession &Session )
{
	qRH
		base::rCastingRack Rack;
	qRB
		Rack.Init( XSLAffix_, Session );
	casting_::Get( Session, Rack() );

	sclxdhtml::SetDocumentCasting( XSLAffix_, Session.Registry(), Rack, Session );
	qRR
		qRT
		qRE
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

