/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef XDHUTL__INC
# define XDHUTL__INC

# define XDHUTL_NAME		"XDHUTL"

# if defined( E_DEBUG ) && !defined( XDHUTL_NODBG )
#  define XDHUTL_DBG
# endif

// XD(HTML) Upstream TooLs

# include "xdhcmn.h"

# include "err.h"

namespace xdhutl {

	typedef ntvstr::char__ nchar__;
	typedef ntvstr::string___ nstring___;

	E_ENUM( action ) {
		aOpenFile,
		aOpenFiles,
		aSaveFile,
		a_amount,
		a_Undefined,
		a_User	// To report an action which is not a predefined one, but a user one.
	};

	const char *GetLabel( action__ Action );

	action__ GetAction( const nstring___ &Pattern );

	inline bso::bool__ IsPredefined( action__ Action )
	{
		return ( Action < a_amount );
	}

	inline bso::bool__ IsKeyEvent( const str::string_ &Event )
	{
		return  !str::Compare( Event, str::string( "key" ), 0, 0, 3 );
	}

	inline bso::bool__ IsKeyEvent( const char *Event )
	{
		return IsKeyEvent(str::string( Event ) );
	}

	void BuildKeyShortcut(
		const str::string_ &Keys,
		str::string_ &Shortcut );

	class event_abstract_ {
	public:
		struct s {
			str::string_::s Event;
			str::string_::s EventKeys;
			action__ Action;
			str::string_::s UserAction;
			xdhcmn::args_::s Args;
		} &S_;
		str::string_ Event;
		str::string_ EventKeys;	// For key-related events.
		str::string_ UserAction;
		xdhcmn::args_ Args;	// Only for NON-user action (i.e. 'Action' != 'a_User').
		event_abstract_( s &S )
		: S_( S ),
		  Event( S.Event ),
		  EventKeys( S.EventKeys ),
		  UserAction( S.UserAction ),
		  Args( S.Args )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.Action = a_Undefined;

			Event.reset( P );
			EventKeys.reset( P );
			UserAction.reset( P );
			Args.reset( P );
		}
		void plug( qAS_ &AS )
		{
			Event.plug( AS );
			EventKeys.plug( AS );
			UserAction.plug( AS );
			Args.plug( AS );
		}
		event_abstract_ &operator =( const event_abstract_ &EA )
		{
			S_.Action = EA.S_.Action;

			Event = EA.Event;
			EventKeys = EA.EventKeys;
			UserAction = EA.UserAction;
			Args = EA.Args;

			return *this;
		}
		void Init( void )
		{
			S_.Action = a_Undefined;

			Event.Init();
			EventKeys.Init();
			UserAction.Init();
			Args.Init();
		}
		E_RODISCLOSE_( action__, Action );
	};

	E_AUTO( event_abstract );

	typedef ctn::E_CONTAINER_( event_abstract_ ) event_abstracts_;
	E_AUTO( event_abstracts );

	inline void GetTagDefaultEvent(
		const str::string_ &Name,
		str::string_ &Event )
	{
		if ( Name == "SELECT" )
			Event.Append( "change" );
		else
			Event.Append( "click" );
	}

	void Fill(
		const str::string_ &DefaultEvent,
		const xdhcmn::args_ &Definition,
		event_abstract_ &Abstract );

	void FillMono(
		const str::string_ &DefaultEvent,
		const xdhcmn::args_ &Definition,	// Contains only one event abstract.
		event_abstracts_ &Abstracts );

	void FillMulti(
		const str::string_ &DefaultEvent,
		const xdhcmn::args_ &Defintions,	// Each entry is a event handler.
		event_abstracts_ &Abstracts );

	void FillEventAbstracts(
		const str::string_ &TagName,
		const xdhcmn::args_ &Events,
		event_abstracts_ &Abstracts );

	bso::bool__ HasEvent(
		const char *Event,
		const event_abstracts_ &Abstracts );

	sdr::row__ Find(
		const str::string_ &Event,
		const str::string_ &Keys,	// Only for keyboard-related events.
		const event_abstracts_ &Abstracts	);	// Returns the 'row' in 'Abstracts' corresponding to 'Event' ; 'qNIL' if not found.

	void GetEventsAbstracts(
		const str::string_ &TagName,
		const str::string_ &Events,
		event_abstracts_ &Abstracts );

	void SplitWidgetFeatures(
		const xdhcmn::args_ &Features,
		str::string_ &Type,
		str::string_ &Parameters,
		str::string_ &ContentRetrievingMethod,
		str::string_ &FocusingMethod );

	void GetWidgetTypeAndParameters(
		const xdhcmn::args_ &Features,
		str::string_ &Type,
		str::string_ &Parameters );

	void GetWidgetContentRetrievingMethod(
		const xdhcmn::args_ &Features,
		str::string_ &Method );

	void GetWidgetFocusingMethod(
		const xdhcmn::args_ &Features,
		str::string_ &Method );


}

#endif
