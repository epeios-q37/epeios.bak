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

#include "mails.h"

#include "core.h"
#include "main.h"
#include "mail.h"
#include "registry.h"

#include "sclfrntnd.h"

namespace {

	qCDEF( char *, XSLAffix_, "Mails" );
	qCDEF( char *, MailFrameId_, "Mail" );

	void GetContext_(
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContextRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Rack().PutAttribute( "CurrentMail", **Session.User.Mail().Current, **instc::UndefinedMail );
	qRR
	qRT
	qRE
	}

	static void GetContent_(
		const sclrgstry::registry_ &Registry,
		core::rSession &Session,
		str::string_ &XML )
	{
	qRH
		base::rContentRack Rack;
	qRB
		Rack.Init( XSLAffix_, XML, Session );

		Session.User.DumpAgents( Rack );

		Session.User.DumpMails( Rack );
	qRR
	qRT
	qRE
	}
}

void mails::SetLayout(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init(); 
	GetContent_( Session.Registry(), Session, XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags( rgstry::tentry___( registry::definition::XSLLayoutFile, XSLAffix_ ), Session.Registry(), XSL );

	Session.FillElement( Id, XML, XSL );

	SetCasting( Id, Session );

//	Session.SwitchTo( core::fMails );
qRR
qRT
qRE
}

void mails::SetCasting(
	const char *Id,
	core::rSession &Session )
{
qRH
	str::string XML, XSL;
qRB
	XML.Init();
	GetContext_( Session,  XML );

	XSL.Init();
	sclxdhtml::LoadXSLAndTranslateTags(rgstry::tentry___( registry::definition::XSLCastingFile, XSLAffix_ ), Session.Registry() , XSL );

	Session.FillElementCastings( Id, XML, XSL );
qRR
qRT
qRE
}

#define AC( name ) BASE_AC( mails, name )

AC( SelectMail )
{
	instc::sMail Mail = instc::UndefinedMail;

	Session.GetNumericalContent( Id, **Mail );

	Session.User.SelectMail( Mail );

	main::SetMailsCasting( Session );
	mail::SetLayout( MailFrameId_, Session );
}

AC( DragMail )
{
	instc::sMail Mail = instc::UndefinedMail;

	Session.GetNumericalContent( Id, **Mail );

	Session.User.DragMail( Mail );

	main::SetFoldersCasting( Session );
}

AC( EndMailDragging )
{
	Session.User.EndMailDragging();

	main::SetFoldersCasting( Session);
}

