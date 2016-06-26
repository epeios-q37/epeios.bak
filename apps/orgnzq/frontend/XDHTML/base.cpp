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

#include "base.h"

#include "core.h"

#include "field.h"

using namespace base;

void base::Register(
	const char *Name,
	cAction &Callback )
{
	core::Core.AddActionCallback( Name, Callback );
}

namespace {
	void UpdateEntry_(
		core::rSession &Session,
		const char *Action )

	{
	qRH
		str::wString Content;
	qRB
		Content.Init();

		Session.GetContent( "EditableEntry", Content );

		Session.User.UpdateEntry( Content );

		if ( !core::OnFieldDefiningAllowedActions.Search( Action ) )
			Session.User.UpdateField();
	qRR
	qRT
	qRE
	}
}
 

bso::bool__ base::sActionHelper::SCLXHTMLOnBeforeAction(
	core::rSession &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !core::OnNotConnectedAllowedActions.Search( Action) ) {
			Session.AlertT( "ActionNeedsBackend" );
			return false;
		} else
			return true;
	} else if ( Session.User.Focus() == frdinstc::tField ) {
		if ( !Session.User.IsDraggingInProgress() )
			UpdateEntry_( Session, Action );
		return true;
	} else
		return true;
}

bso::bool__ base::sActionHelper::SCLXHTMLOnClose( core::rSession &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

void base::DumpFocus_(
	const core::rSession &Session,
	xml::dWriter &Writer )
{
	if ( Session.User.GetFocus() != frdinstc::t_Undefined )
		Writer.PutAttribute( "Focus", frdinstc::GetLabel( Session.User.GetFocus() ) );
}


Q37_GCTOR( base )
{
}

