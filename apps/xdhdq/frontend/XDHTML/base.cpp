/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHDq' software.

    'XDHDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "base.h"

#include "core.h"
#include "login.h"
#include "main.h"
#include "prolog.h"

using namespace base;

using namespace instc;

E_CDEF(char *, base:: Name, NAME);

void base::Register(
	const char *Name,
	cAction &Callback )
{
	core::Core.AddActionCallback( Name, Callback );
}

namespace {
	void RetrieveContent_(
		core::rSession &Session,
		const char *Action )
	{
	qRH;
		eField Field = f_Undefined;
		str::wString Content;
	qRB;
		Field = Session.User.GetField();

		if ((  Field != f_Undefined ) && ( strcmp( Action, main::HideTestButton.Name ) ) ) {
			Content.Init();
			Session.GetValue( instc::GetLabel( Field ), Content );

			Session.User.SetContent( Field, Content );

			Session.User.ResetField();
		}
	qRR;
	qRT;
	qRE;
	}
}

bso::bool__ base::sActionHelper::SCLXOnBeforeAction(
	core::rSession_ &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !core::OnNotConnectedAllowedActions.Search( Action) ) {
			Session.AlertT( "ActionNeedsBackend" );
			return false;
		} else {
			RetrieveContent_( Session, Action );
			return true;
		}
	} else {
		RetrieveContent_( Session, Action );
		return true;
	}
}

void base::sActionHelper::SCLXOnRefresh( core::rSession_ &Session )
{
	switch ( Session.Page() ) {
	case core::pProlog:
		prolog::Display( Session );
		break;
	case core::pLogin:
		login::Display( Session );
		break;
	case core::pMain:
		main::Display( Session );
		break;
	default:
		qRGnr();
		break;
	}
}

bso::bool__ base::sActionHelper::SCLXOnClose( core::rSession_ &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

Q37_GCTOR( base )
{
}

