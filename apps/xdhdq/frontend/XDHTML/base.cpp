/*
	'base.cpp' by Claude SIMON (http://q37.info/contact/).

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

#include "base.h"

#include "core.h"

using namespace base;

void base::AddActionCallback(
	const char *Name,
	action_callback__ &Callback )
{
	core::Core.AddActionCallback( Name, Callback );
}

bso::bool__ base::action_helper_callback__::SCLXHTMLOnBeforeAction(
	core::session___ &Session,
	const char *Id,
	const char *Action )
{
	if ( Session.User.EditableField() != frdinstc::f_Undefined ) {
		str::string_ &Content = Session.User.FieldContent( Session.User.EditableField() );

		Content.Init();
		Session.GetContent( "EditableEntry", Content );

		Session.User.EditableField() = frdinstc::f_Undefined;
	}

	return true;
}

bso::bool__ base::action_helper_callback__::SCLXHTMLOnClose( core::session___ &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}
