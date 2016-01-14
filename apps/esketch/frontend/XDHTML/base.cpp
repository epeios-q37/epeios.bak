/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "base.h"

#include "core.h"

using namespace base;

void base::AddActionCallback(
	const char *Name,
	action_callback__ &Callback )
{
	core::Core.AddActionCallback( Name, Callback );
}

namespace {

	stsfsm::automat AllowedActionsOnWhenNotConnectedToBackend_;

	static inline bso::bool__ IsActionAllowedWhenNotConnectedToBackend_( const char *Action )
	{
		return stsfsm::GetId( str::string( Action ), AllowedActionsOnWhenNotConnectedToBackend_ ) == 0;
	}
}

bso::bool__ base::action_helper_callback__::SCLXHTMLOnBeforeAction(
	core::session___ &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !IsActionAllowedWhenNotConnectedToBackend_(Action) ) {
			Session.AlertT( "ActionNeedsBackend" );
			return false;
		} else
			return true;
	} else
		return true;
}

bso::bool__ base::action_helper_callback__::SCLXHTMLOnClose( core::session___ &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

void base::AddAllowedActionsOnWhenNotConnectedToBackend( const char *Action )
{
	if ( stsfsm::Add( Action, 0, AllowedActionsOnWhenNotConnectedToBackend_ ) != stsfsm::UndefinedId )
		qRGnr();
}

void base::AddAllowedActionsOnWhenNotConnectedToBackend(
	const action_callback__ *FirstCallback,
	... )
{
	const action_callback__ *Callback = FirstCallback;

	va_list Callbacks;
	va_start( Callbacks, FirstCallback );

	while ( Callback != NULL ){
		AddAllowedActionsOnWhenNotConnectedToBackend( Callback->Name() );

		Callback = va_arg( Callbacks, const action_callback__ * );
	}

	va_end( Callbacks );
}

Q37_GCTOR( base )
{
	AllowedActionsOnWhenNotConnectedToBackend_.Init();
}

