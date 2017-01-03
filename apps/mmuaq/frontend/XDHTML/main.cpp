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

#include "main.h"

#include "core.h"
#include "config.h"
#include "folders.h"
#include "mails.h"
#include "registry.h"
#include "sclfrntnd.h"

using namespace main;

namespace {
	qCDEF( char *, XSLAffix_, "Main" );
	qCDEF(char *, FoldersFrameId_, "Folders" );
	qCDEF(char *, MailsFrameId_, "Mails" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}

	void SetCasting_( core::rSession &Session )
	{
	qRH
		str::string XML, XSL;
	qRB
		XML.Init();
		GetContext_( Session,  XML );

		XSL.Init();
		sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

		Session.FillDocumentCastings( XML, XSL );
	qRR
	qRT
	qRE
	}

	void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );
	qRR
	qRT
	qRE
	}
}

void main::SetLayout( core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillDocument( XML, XSL );

	SetCasting_( Session );

	folders::SetLayout( FoldersFrameId_, Session );
	mails::SetLayout( MailsFrameId_, Session );

	Session.SwitchTo( core::pMain );
qRR
qRT
qRE
}

void main::SetFoldersLayout( core::rSession &Session )
{
	folders::SetLayout( FoldersFrameId_, Session );
}

void main::SetFoldersCasting( core::rSession &Session )
{
	folders::SetCasting( FoldersFrameId_, Session );
}

void main::SetMailsLayout( core::rSession &Session )
{
	mails::SetLayout( MailsFrameId_, Session );
}

void main::SetMailsCasting( core::rSession &Session )
{
	mails::SetCasting( MailsFrameId_, Session );
}

#define AC( name ) BASE_AC( main, name )

AC( Check )
{
	Session.User.Check();
	SetLayout( Session );
}

AC( Configuration )
{
	config::SetLayout( Session );
}
