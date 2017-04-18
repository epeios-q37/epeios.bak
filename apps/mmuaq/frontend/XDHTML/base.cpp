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

#include "base.h"

#include "core.h"
#include "config.h"
#include "login.h"
#include "prolog.h"
#include "main.h"

using namespace base;

E_CDEF(char *, base::Name, BASE_NAME);

void base::Register(
	const char *Name,
	cAction &Callback )
{
	core::Core.AddActionCallback( Name, Callback );
}

bso::bool__ base::sActionHelper::SCLXOnBeforeAction(
	core::rSession &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !core::OnNotConnectedAllowedActions.Search( Action ) ) {
			Session.AlertT( "ActionNeedsBackend" );
			return false;
		} else
			return true;
	} else if ( ( Session.User.Folder().State == instc::folder_::sEdition ) || ( Session.User.Folder().State == instc::folder_::sCreation ) ) {
		if ( core::OnFolderEditionIgnoredActions.Search( Action ) )
			return true;
		else
			return core::folder::Apply( Session );
	} else
		return true;
}

void base::sActionHelper::SCLXOnRefresh( core::rSession &Session )
{
	switch ( Session.Page() ) {
	case core::pProlog:
		prolog::SetLayout( Session );
		break;
	case core::pLogin:
		login::SetLayout( Session );
		break;
	case core::pMain:
		main::SetLayout( Session );
		break;
	case core::pConfig:
		config::SetLayout( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

bso::bool__ base::sActionHelper::SCLXOnClose( core::rSession &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

void base::sCorpusCallback::XDHDWSDump( xml::dWriter &Writer )
{
	Writer.PushTag( "Folders" );
	Writer.PutAttribute( "Root", **S_().User.Folder().Root, **instc::UndefinedFolder );
	Writer.PutAttribute( "Inbox", **S_().User.Folder().Inbox, **instc::UndefinedFolder );
	Writer.PopTag();
}

Q37_GCTOR( base )
{
}

