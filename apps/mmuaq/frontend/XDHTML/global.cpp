/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "global.h"

#include "core.h"
#include "registry.h"

using namespace global;

namespace {
	qCDEF(char *, XSLAffix_, "Global" );
}

#define AC( name ) BASE_AC( global, name )

AC( About )
{
qRH
	str::string XML, XSL;
	base::rContentRack Rack;
	str::string AboutTranslation;
qRB
	XML.Init();
	Rack.Init( XSLAffix_, XML, Session );

	Rack().PushTag( "About" );
	core::About( Session, Rack );
	Rack().PopTag();

	Rack.reset();

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, "About" ), sclxdhtml::GetRegistry(), XSL );	// Potentialy outside session, so we use the global registry...

	AboutTranslation.Init();
	scllocale::GetTranslation("About...", Session.Language(), AboutTranslation );

	Session.Alert( XML, XSL, AboutTranslation );
qRR
qRT
qRE
}

AC( Test )
{
qRH
	str::string Message;
qRB
	Message.Init( "Test : " );
	Message.Append( Id );

	Session.Log( Message );
qRR
qRT
qRE
}

