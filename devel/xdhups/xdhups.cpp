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

#define XDHUPS__COMPILATION

#include "xdhups.h"

using namespace xdhups;

extern "C" typedef xdhcbk::retrieve retrieve;

bso::bool__ xdhups::agent___::Init(
	xdhcbk::mode__ Mode,
	const str::string_ &ModuleFileName,
	const char *Identification )
{
	bso::bool__ Success = false;
qRH
	xdhcbk::shared_data__ Data;
	fnm::name___ Location;
	TOL_CBUFFER___ Buffer;
qRB
	Location.Init();
	Data.Init( Mode, Identification, fnm::GetLocation( ModuleFileName, Location ).UTF8( Buffer ) );

	Library_.Init( ModuleFileName );

	retrieve *Retrieve = dlbrry::GetFunction<retrieve *>( E_STRING( XDHCBK_RETRIEVE_FUNCTION_NAME ), Library_ );

	if ( Retrieve == NULL )
		qRReturn;

   Callback_ = Retrieve();

	if ( Callback_ == NULL )
		qRFwk();

	Callback_->Initialize( Data );

	Success = true;
qRR
qRT
qRE
	return Success;
}

namespace {
	stsfsm::automat ActionAutomat_;

	template <typename type> inline type GetId_(
		const nstring___ &Pattern,
		const stsfsm::automat_ &Automat,
		type UndefinedValue,
		bso::uint__ Amount )
	{
		type Id = UndefinedValue;
	qRH
		str::string Buffer;
	qRB
		Buffer.Init();
		Id = stsfsm::GetId( Pattern.UTF8( Buffer ), Automat, UndefinedValue, Amount );
	qRR
	qRT
	qRE
		return Id;
	}

	static void InitAndFillActionAutomat_( void )
	{
		ActionAutomat_.Init(),
		stsfsm::Fill( ActionAutomat_, a_amount, GetLabel );
	}

	void InitAndFillAutomats_( void )
	{
		InitAndFillActionAutomat_();
	}
}

#define A( name )	case a##name : return #name; break

const char *xdhups::GetLabel( action__ Action )
{
	switch ( Action ) {
		A( OpenFile );
		A( OpenFiles );
		A( SaveFile );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

action__ xdhups::GetAction( const nstring___ &Pattern )
{
	return GetId_( Pattern, ActionAutomat_, a_Undefined, a_amount );
}

namespace {
	static action__ GetPredefinedAction_(
		const xdhcbk::args_ &ActionWithParameters,
		str::string_ &UserAction,
		xdhcbk::args_ &Args )
	{
		action__ Action = a_Undefined;
	qRH
		xdhcbk::retriever__ Retriever;
		str::string PredefinedAction;
	qRB
		if ( ActionWithParameters.Amount() < 2 )
			qRFwk();

		Retriever.Init( ActionWithParameters );

		PredefinedAction.Init();
		Retriever.GetString( PredefinedAction );
		Action = GetAction( PredefinedAction );

		Retriever.GetString( UserAction );

		if ( Retriever.Availability() != strmrg::aNone )
			Retriever.GetTable( Args );
	qRR
	qRT
	qRE
		return Action;
	}

	action__ GetAction_(
		const xdhcbk::args_ &ActionWithParameters,
		str::string_ &UserAction,
		xdhcbk::args_ &Args )
	{
		strmrg::retriever__ Retriever;

		Retriever.Init( ActionWithParameters );

		switch ( ActionWithParameters.Amount() ) {
		case 1:
			Retriever.GetString( UserAction );
			return a_User;
		case 2:
		case 3:
			return GetPredefinedAction_( ActionWithParameters, UserAction, Args );
			break;
		default:
			qRFwk();
			break;
		}

		return a_Undefined;	// To avoid a 'warning'.
	}
}

inline void HandleKeyShortcutModifier_(
	char Char,
	const str::string_ &Pattern,
	str::string_ &Shortcut )
{
	sdr::row__ CharPos = qNIL, PlusPos = qNIL;

	if ( ( ( CharPos = Pattern.Search(Char) ) != qNIL ) 
		 && ( ( PlusPos = Pattern.Search( '+' ) ) != qNIL )
		 && ( *CharPos < *PlusPos ) )
		Shortcut.Append( Char );
	else
		Shortcut.Append( '.' );
}

void xdhups::BuildKeyShortcut(
	const str::string_ &Keys,
	str::string_ &Shortcut )
{
	sdr::row__ PlusPos = Keys.Search( '+' );

	if ( PlusPos != qNIL ) {
		if ( Keys.Amount() != 1 ) {

			if ( PlusPos == Keys.Last() )
				qRFwk();

			HandleKeyShortcutModifier_( 'A', Keys, Shortcut );	// Alt
			HandleKeyShortcutModifier_( 'S', Keys, Shortcut );	// Shift
			HandleKeyShortcutModifier_( 'C', Keys, Shortcut );	// Control
			HandleKeyShortcutModifier_( 'M', Keys, Shortcut );	// Meta

			Shortcut.Append( Keys, Keys.Next( PlusPos ) ); 
		} else // In this case, the shortcut is simply a '+', os the '+' is not the separator next to modifiers.
			Shortcut.Append( Keys );
	} else
		Shortcut.Append( Keys );
}

void xdhups::Fill(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definition,
	event_abstract_ &Abstract )
{
qRH
	xdhcbk::args ActionWithParameters;
	xdhcbk::retriever__ Retriever;
	str::string Keys;
	bso::bool__ RetrieveKeys = false;
	str::string KeyShortcut;
	sdr::row__ PlusPos = qNIL;
qRB
	Retriever.Init( Definition );
	Keys.Init();

	switch ( Definition.Amount() ) {
	case 3:
		RetrieveKeys = true;
	case 2:
		Retriever.GetString( Abstract.Event );
	case 1:
		ActionWithParameters.Init();
		Retriever.GetTable( ActionWithParameters );
		Abstract.S_.Action = GetAction_( ActionWithParameters, Abstract.UserAction, Abstract.Args );
		break;
	default:
		qRFwk();
		break;
	}

	if ( RetrieveKeys ) {
		if ( !IsKeyEvent( Abstract.Event ) )
			qRFwk();

		Keys.Init();
		Retriever.GetString( Keys );

		KeyShortcut.Init();

		BuildKeyShortcut( Keys, KeyShortcut );

		Abstract.EventKeys = KeyShortcut;
	}

	if ( Abstract.Event.Amount() == 0 )
		Abstract.Event = DefaultEvent;
qRR
qRT
qRE
}
void xdhups::FillMono(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definition,
	event_abstracts_ &Abstracts )
{
qRH
	event_abstract Abstract;
qRB
	Abstract.Init();

	Fill( DefaultEvent, Definition, Abstract );

	Abstracts.Append( Abstract );
qRR
qRT
qRE
}

void xdhups::FillMulti(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definitions,
	event_abstracts_ &Abstracts )
{
qRH
	xdhcbk::args Definition;
	strmrg::retriever__ Retriever;
qRB
	Retriever.Init( Definitions );

	while ( Retriever.Availability() != strmrg::aNone ) {
		Definition.Init();

		Retriever.GetTable( Definition );

		FillMono( DefaultEvent, Definition, Abstracts );
	}
qRR
qRT
qRE
}

void xdhups::FillEventAbstracts(
	const str::string_ &TagName,
	const xdhcbk::args_ &Events,
	event_abstracts_ &Abstracts )

{
qRH
	str::string DefaultEvent;
	TOL_CBUFFER___ Buffer;
qRB
	DefaultEvent.Init();
	GetTagDefaultEvent( TagName, DefaultEvent );

	FillMulti( DefaultEvent, Events, Abstracts );
qRR
qRT
qRE
}

bso::bool__ xdhups::HasEvent(
	const char *Event,
	const event_abstracts_ &Abstracts )
{
	ctn::E_CITEM( event_abstract_ ) Abstract;
	sdr::row__ Row = Abstracts.First();

	Abstract.Init( Abstracts );

	while ( ( Row != qNIL ) && ( Abstract( Row ).Event != Event ) )
		Row = Abstracts.Next( Row );

	return Row != qNIL;
}


sdr::row__ xdhups::Find(
	const str::string_ &Event,
	const str::string_ &RawKeys,
	const event_abstracts_ &Abstracts )
{
	sdr::row__ Row = qNIL;
qRH
	ctn::E_CITEM( event_abstract_ ) Abstract;
	str::string Keys;
qRB
	Row = Abstracts.First();

	Abstract.Init( Abstracts );

	Keys.Init();
	BuildKeyShortcut( RawKeys, Keys );

	while ( ( Row != qNIL )
		    && ( ( Abstract( Row ).Event != Event )
			     || ( IsKeyEvent( Event ) && Abstract( Row ).EventKeys != Keys ) ) )
		Row = Abstracts.Next( Row );

	qRR
qRT
qRE
	return Row;
}

void xdhups::GetEventsAbstracts(
	const str::string_ &TagName,
	const str::string_ &Events,
	event_abstracts_ &Abstracts )
{
qRH
	xdhcbk::args Definitions;
	TOL_CBUFFER___ Buffer;
qRB
	if ( Events.Amount() != 0 ) {
		Definitions.Init();
		xdhcbk::Split( Events, Definitions );
		FillEventAbstracts( TagName, Definitions, Abstracts );
	}
qRR
qRT
qRE
}

void xdhups::SplitWidgetFeatures(
	const xdhcbk::args_ &Features,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod )
{
qRH
	xdhcbk::retriever__ Retriever;
qRB
	Retriever.Init( Features );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Type );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Parameters );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( ContentRetrievingMethod );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FocusingMethod );
qRR
qRT
qRE
}

void xdhups::GetWidgetTypeAndParameters(
	const xdhcbk::args_ &Features,
	str::string_ &Type,
	str::string_ &Parameters )
{
qRH
	str::string ContentRetrievingMethod, FocusingMethod;
qRB
	ContentRetrievingMethod.Init();
	FocusingMethod.Init();
	SplitWidgetFeatures( Features, Type, Parameters, ContentRetrievingMethod, FocusingMethod );
qRR
qRT
qRE
}

void xdhups::GetWidgetContentRetrievingMethod(
	const xdhcbk::args_ &Features,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, OtherMethod;
qRB
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	SplitWidgetFeatures( Features, Type, Parameters, Method, OtherMethod );
qRR
qRT
qRE
}

void xdhups::GetWidgetFocusingMethod(
	const xdhcbk::args_ &Features,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, OtherMethod;
qRB
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	SplitWidgetFeatures( Features, Type, Parameters, OtherMethod, Method );
qRR
qRT
qRE
}

