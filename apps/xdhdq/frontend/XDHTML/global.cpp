/*
	'global.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "global.h"

#include "core.h"
#include "registry.h"

E_CDEF(char *, XSLAffix_, "Global" );

BASE_AC( global::about__ )
{
qRH
	str::string XML, XSL;
	base::content_rack___ Rack;
	str::string AboutTranslation;
qRB
	XML.Init();
	Rack.Init( XSLAffix_, XML, Session );

	Rack().PushTag( "About" );
	core::About( Session, Rack );
	Rack().PopTag();

	Rack.reset();

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::XSLLayoutFile, "About" ), sclxdhtml::GetRegistry(), XSL );	// Potentialy outside session, so we use the global registry...

	AboutTranslation.Init();
	scllocale::GetTranslation("About...", Session.Language(), AboutTranslation );

	Session.Alert( XML, XSL, AboutTranslation );
qRR
qRT
qRE
}


BASE_AC( global::refresh__ )
{
	Session.Refresh();
}
