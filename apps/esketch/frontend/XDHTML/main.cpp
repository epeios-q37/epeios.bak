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

#include "main.h"

#include "core.h"
#include "registry.h"

namespace{
	qCDEF( char *, XSLAffix_, "Main" );

	namespace layout_ {
		void Get(
			core::rSession &Session,
			xml::rWriter &Writer )
		{}
	}
}

void main::SetLayout( core::rSession &Session )
{
	Session.SetDocumentLayout( XSLAffix_, layout_::Get );
}

const char *Test = R"((function() {
// Load the script
var script = document.createElement( "SCRIPT" );
script.src = 'https://code.jquery.com/jquery-1.12.4.js';
script.type = 'text/javascript';
script.onload = function()
{
	jQuery.getScript( 'https://code.jquery.com/ui/1.12.1/jquery-ui.js' )
};
document.getElementsByTagName( "head" )[0].appendChild( script );
})();)";

void main::Display( core::rSession &Session )
{
	SetLayout( Session );

	Session.Execute( "var s = document.createElement('link'); s.setAttribute('rel','stylesheet');s.setAttribute('href','http://code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css');document.head.appendChild(s);" );
	Session.Execute( Test );
/*
	Session.Execute( "document.write('<script type=\"text/javascript\" src=\"https://code.jquery.com/jquery-1.12.4.js\"></script>')" );
	Session.Execute( "var s = document.createElement('script'); s.setAttribute('src','https://code.jquery.com/ui/1.12.1/jquery-ui.js');document.head.appendChild(s);" );
*/
	Session.AddClass( "TestButton", "hide" );

	Session.SwitchTo( base::pMain );
}

#define A( name ) BASE_ADef( main, name )

str::wString Common_;

namespace {
	void Submission_(
		const char *Id,
		core::rSession &Session )
	{
	qRH;
		str::wString Content;
	qRB;
		Content.Init();
		Session.GetContent( "Pattern", Content );

		if ( Content.IsBlank() )
            Session.ConfirmT("EmptyPattern");
        else {
            Session.User.ToUpper( Content );
            Session.SetContent( "Pattern", Content );
            Common_.Init(Content);
            Session.BroadcastAction("BroadcastTest", "toto");
        }
	qRR;
	qRT;
	qRE;
	}
}

A( Submission )
{
	Submission_( Id, Session );
}

A( BroadcastTest )
{
    str::wString Output;

    Output.Init();

    Session.SetContent("Output", Common_);
//	Session.AlertB(str::wString(Id));
}

A( ShowTestButton )
{
	Session.RemoveClass( "TestButton", "hide" );
}

A( HideTestButton )
{
	Session.AddClass( "TestButton", "hide" );
}

A( ConnectedTesting )
{
	Session.User.TestMessage();
}

A( UnconnectedTesting )
{
    Session.AlertB(str::wString("Alerte de base !"));

#if 0
	if ( Session.IsConnected() )
		Session.AlertT("ConnectedTestMessage", "TestMessageTitle");
	else
		Session.AlertT("UnconnectedTestMessage", "TestMessageTitle");
#endif
}


