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

	stsfsm::automat AllowedActionsOnWhenNotConnectedToBackend_;

	static inline bso::bool__ IsActionAllowedWhenNotConnectedToBackend_( const char *Action )
	{
		return stsfsm::GetId( str::string( Action ), AllowedActionsOnWhenNotConnectedToBackend_ ) == 0;
	}
}

namespace {
	void UpdateEntry_(
		core::rSession &Session,
		const char *Action )

	{
	qRH
		str::wString Content;
	qRB
		if ( !Session.User.EntryLatch() ) {
			Content.Init();

			Session.GetContent( "EditableEntry", Content );

			Session.User.UpdateEntry( Content );

			if ( Action != field::DefineNewEntry.Name )
				Session.User.UpdateField();
		}
	qRR
	qRT
	qRE
	}
}
 

bso::bool__ base::action_helper_callback__::SCLXHTMLOnBeforeAction(
	core::rSession &Session,
	const char *Id,
	const char *Action )
{
	if ( !Session.IsConnected() ) {
		if ( !IsActionAllowedWhenNotConnectedToBackend_(Action) ) {
			Session.AlertT( "ActionNeedsBackend" );
			return false;
		} else
			return true;
	} else if ( Session.User.Focus() == frdinstc::tField ) {
		UpdateEntry_( Session, Action );
		return true;
	} else
		return true;
}

bso::bool__ base::action_helper_callback__::SCLXHTMLOnClose( core::rSession &Session )
{
	return Session.ConfirmT( "ClosingConfirmation" );
}

void base::rContextRack::Init(
	const char *View,
	str::string_ &Target,
	core::rSession &Session )
{
	rContextRack_::Init( View, Target, Session );

	if ( Session.User.GetFocus() != frdinstc::t_Undefined )
		operator()().PutAttribute( "Focus", frdinstc::GetLabel( Session.User.GetFocus() ) );
}


void base::AddAllowedActionsOnWhenNotConnectedToBackend(
	const char *FirstCallback,
	... )
{
	const char *Callback = FirstCallback;

	va_list Callbacks;
	va_start( Callbacks, FirstCallback );

	while ( Callback != NULL ){
		if ( stsfsm::Add( Callback, 0, AllowedActionsOnWhenNotConnectedToBackend_ ) != stsfsm::UndefinedId )
			qRGnr();

		Callback = va_arg( Callbacks, const char *);
	}

	va_end( Callbacks );
}

Q37_GCTOR( base )
{
	AllowedActionsOnWhenNotConnectedToBackend_.Init();
}

