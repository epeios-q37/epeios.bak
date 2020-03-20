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

#include "global.h"

#include "core.h"
#include "login.h"
#include "main.h"
#include "prolog.h"
#include "registry.h"

qCDEF(char *, XSLAffix_, "Global" );

#define A( name ) BASE_ADef( global, name )

A( OnNewSession )
{
	switch ( Mode ) {
	case xdhcmn::mMonoUser:
		switch ( sclfrntnd::HandleProject( esketchxdh::Info ) ) {
		case sclfrntnd::phNone:
			::prolog::Display( Session );
			break;
		case sclfrntnd::phLoad:
			::login::Display( Session );
			break;
		case sclfrntnd::phLogin:
			Session.Set_BackendVisibility( sclxdhtml::bvHide );
			::login::Display( Session );
			break;
		case sclfrntnd::phRun:
			::main::Display( Session );
			break;
		default:
			qRGnr();
			break;
		}
		break;
	case xdhcmn::mMultiUser:
        Session.Set_BackendVisibility(sclxdhtml::bvHide);
        ::login::Display( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

A( About )
{
qRH
	str::string XSL;
	sclxdhtml::rRack<core::rSession,core::sDump> Rack;
	str::string AboutTranslation;
qRB
	Rack.Init( XSLAffix_, Session, esketchxdh::Info );

	Rack().PushTag( "About" );
	core::About( Session, Rack );
	Rack().PopTag();

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLFile, "About" ), sclxdhtml::GetRegistry(), XSL );	// Potentially outside session, so we use the global registry...

	AboutTranslation.Init();
	scllocale::GetTranslation("About...", Session.Language(), AboutTranslation );

	Session.Alert( Rack.Target(), XSL, AboutTranslation );
qRR
qRT
qRE
}

A( Test )
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

