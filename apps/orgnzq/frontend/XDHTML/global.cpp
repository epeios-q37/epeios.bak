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

#include "global.h"

#include "core.h"
#include "registry.h"

E_CDEF(char *, XSLAffix_, "Global" );

#define AC( name ) BASE_AC( global, name )

AC( About )
{
qRH
	str::string XSL;
	base::rLayoutRack Rack;
	str::string AboutTranslation;
qRB
	Rack.Init( XSLAffix_, Session );

	Rack().PushTag( "About" );
	core::About( Session, Rack );
	Rack().PopTag();

	Rack.reset();

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, "About" ), Session.IsConnected() ? Session.Registry() : sclxdhtml::GetRegistry(), XSL );

	AboutTranslation.Init();
	scllocale::GetTranslation("About...", Session.Language(), AboutTranslation );

	Session.Alert( Rack.Target(), XSL, AboutTranslation );
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