/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XDHUTL__COMPILATION

#include "xdhutl.h"

using namespace xdhutl;

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

const char *xdhutl::GetLabel( action__ Action )
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

action__ xdhutl::GetAction( const nstring___ &Pattern )
{
	return GetId_( Pattern, ActionAutomat_, a_Undefined, a_amount );
}

namespace {
	static action__ GetPredefinedAction_(
		const xdhcmn::digest_ &ActionWithParameters,
		str::string_ &UserAction,
		xdhcmn::digest_ &Args )
	{
		action__ Action = a_Undefined;
	qRH
		xdhcmn::retriever__ Retriever;
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
		const xdhcmn::digest_ &ActionWithParameters,
		str::string_ &UserAction,
		xdhcmn::digest_ &Args )
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

void xdhutl::BuildKeyShortcut(
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

void xdhutl::FillEventAbstract(
	const str::string_ &DefaultEvent,
	const str::dString &DefaultEventKeys,
	const xdhcmn::digest_ &Description,
	event_abstract_ &Abstract )
{
qRH
	xdhcmn::digest ActionWithParameters;
	xdhcmn::retriever__ Retriever;
	str::string Keys;
	bso::bool__ RetrieveKeys = false;
	str::string KeyShortcut;
qRB
	Retriever.Init( Description );
	Keys.Init();

	switch ( Description.Amount() ) {
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

	if ( Abstract.Event.Amount() == 0 ) {
		Abstract.Event = DefaultEvent;
		Abstract.EventKeys = DefaultEventKeys;
	}
qRR
qRT
qRE
}

void xdhutl::FillEventAbstractsMono(
	const str::string_ &DefaultEvent,
	const str::dString &DefaultEventKeys,
	const xdhcmn::digest_ &Description,
	event_abstracts_ &Abstracts )
{
qRH
	event_abstract Abstract;
qRB
	Abstract.Init();

	FillEventAbstract( DefaultEvent, DefaultEventKeys, Description, Abstract );

	Abstracts.Append( Abstract );
qRR
qRT
qRE
}

amount__ xdhutl::FillEventAbstractsMulti(
	const str::string_ &DefaultEvent,
	const str::dString &DefaultEventKeys,
	const xdhcmn::digest_ &Descriptions,
	event_abstracts_ &Abstracts )
{
	amount__ Amount = 0;
qRH
	xdhcmn::digest Definition;
	strmrg::retriever__ Retriever;
qRB
	Retriever.Init( Descriptions );

	while ( Retriever.Availability() != strmrg::aNone ) {
		if ( Amount == AmountMax )
			qRLmt();

		Amount++;

		Definition.Init();

		Retriever.GetTable( Definition );

		FillEventAbstractsMono( DefaultEvent, DefaultEventKeys, Definition, Abstracts );
	}
qRR
qRT
qRE
	return Amount;
}

amount__ xdhutl::FillEventAbstracts(
	const str::string_ &TagName,
	const xdhcmn::digest_ &Descriptions,
	event_abstracts_ &Abstracts )

{
	amount__ Amount = 0;
qRH
	str::string DefaultEvent, DefaultEventKeys;
	TOL_CBUFFER___ Buffer;
qRB
	tol::Init( DefaultEvent, DefaultEventKeys );
	GetTagDefaultEvent( TagName, DefaultEvent, DefaultEventKeys );

	Amount = FillEventAbstractsMulti( DefaultEvent, DefaultEventKeys, Descriptions, Abstracts );
qRR
qRT
qRE
	return Amount;
}

amount__ xdhutl::FillEventAbstracts(
	const xdhcmn::digest_ &Descriptions,
	str::string_ &Id,
	event_abstracts_ &Abstracts )
{
	amount__ Amount = 0;
qRH
	xdhcmn::retriever__ Retriever;
	xdhcmn::digest EventsDescriptions;
	str::string TagName;
qRB
	Retriever.Init( Descriptions );

	Retriever.GetString( Id );

	TagName.Init();
	Retriever.GetString( TagName );

	EventsDescriptions.Init();
	Retriever.GetTable( EventsDescriptions );

	Amount = FillEventAbstracts( TagName, EventsDescriptions, Abstracts );
qRR
qRT
qRE
	return Amount;
}

void xdhutl::FillEventAbstracts(
	const xdhcmn::digest_ &Descriptions,
	str::strings_ &Ids,
	event_abstracts_ &Abstracts )
{
qRH
	xdhcmn::retriever__ Retriever;
	xdhcmn::digest SubDescriptions;
	str::string Id;
	TOL_CBUFFER___ Buffer;
	amount__ Amount = 0;
qRB
	Retriever.Init( Descriptions );

	while ( Retriever.Availability() != strmrg::aNone ) {
		SubDescriptions.Init();
		Retriever.GetTable( SubDescriptions );

		Id.Init();
		Amount = FillEventAbstracts( SubDescriptions, Id, Abstracts );

		while ( Amount-- )
			Ids.Append( Id );
	}
qRR
qRT
qRE
}

bso::bool__ xdhutl::Exists(
	const char *Event,
	const event_abstracts_ &Abstracts )
{
	sdr::row__ Row = Abstracts.First();

	while ( ( Row != qNIL ) && ( Abstracts( Row ).Event != Event ) )
		Row = Abstracts.Next( Row );

	return Row != qNIL;
}

sdr::row__ xdhutl::Find(
	const str::string_ &Event,
	const str::string_ &RawKeys,
	const event_abstracts_ &Abstracts )
{
	sdr::row__ Row = qNIL;
qRH
	str::string Keys;
qRB
	Row = Abstracts.First();

	Keys.Init();
	BuildKeyShortcut( RawKeys, Keys );

	while ( ( Row != qNIL )
		    && ( ( Abstracts( Row ).Event != Event )
			     || ( IsKeyEvent( Event ) && Abstracts( Row ).EventKeys != Keys ) ) )
		Row = Abstracts.Next( Row );

	qRR
qRT
qRE
	return Row;
}

bso::bool__ xdhutl::FetchEventAbstract(
	const xdhcmn::digest_ &Digest,
	str::string_ &Id,
	event_abstract_ &AbstractBuffer )
{
	bso::bool__ Found = false;
qRH
	xdhcmn::retriever__ Retriever;
	str::string  Name, Type, Keys;
	xdhcmn::digest Events;
	xdhutl::event_abstracts Abstracts;
	sdr::row__ Row = qNIL;
qRB
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Name.Init();
	Retriever.GetString( Name );

	Type.Init();
	Retriever.GetString( Type );

	Keys.Init();
	Retriever.GetString( Keys );

	Events.Init();
	Retriever.GetTable( Events );

	Abstracts.Init();
	xdhutl::FillEventAbstracts( Name, Events, Abstracts );

	Row = xdhutl::Find( Type, Keys, Abstracts );

	if ( Row == qNIL ) {
		if ( !xdhutl::IsKeyEvent( Type ) )
			qRGnr();
	} else {
		Found = true;

		AbstractBuffer = Abstracts( Row );	// To set the position of 'Handler'.
	}
qRR
qRT
qRE
	return Found;
}

bso::bool__ xdhutl::FetchEventAbstract(
	const str::string_ &RawDigest,
	str::string_ &Id,
	event_abstract_ &Abstract )
{
	bso::bool__ Found = false;
qRH
	xdhcmn::digest Digest;
qRB
	Digest.Init();
	xdhcmn::Split( RawDigest, Digest );

	Found = FetchEventAbstract( Digest, Id, Abstract );
qRR
qRT
qRE
	return Found;
}

void xdhutl::ExtractWidgetFeatures(
	const xdhcmn::digest_ &Description,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod,
	str::dString &SelectionMethod )
{
qRH
	xdhcmn::retriever__ Retriever;
qRB
	Retriever.Init( Description );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Type );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Parameters );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( ContentRetrievingMethod );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FocusingMethod );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetFeatures(
	const str::string_ &RawDescription,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod,
	str::dString &SelectionMethod )

{
qRH
	xdhcmn::digest Description;
qRB
	Description.Init();
	xdhcmn::Split( RawDescription, Description );

	ExtractWidgetFeatures( Description, Type, Parameters, ContentRetrievingMethod, FocusingMethod, SelectionMethod );
qRR
qRT
qRE
}


void xdhutl::ExtractWidgetTypeAndParameters(
	const xdhcmn::digest_ &Description,
	str::string_ &Type,
	str::string_ &Parameters )
{
qRH
	str::string ContentRetrievingMethod, FocusingMethod, SelectionMethod;
qRB
	ContentRetrievingMethod.Init();
	FocusingMethod.Init();
	SelectionMethod.Init();
	ExtractWidgetFeatures( Description, Type, Parameters, ContentRetrievingMethod, FocusingMethod, SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetContentRetrievingMethod(
	const xdhcmn::digest_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, FocusingMethod, SelectionMethod;
qRB
	Type.Init();
	Parameters.Init();
	FocusingMethod.Init();
	SelectionMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, Method, FocusingMethod, SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetContentRetrievingMethod(
	const str::string_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, FocusingMethod, SelectionMethod;
qRB
	Type.Init();
	Parameters.Init();
	FocusingMethod.Init();
	SelectionMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, Method, FocusingMethod, SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetFocusingMethod(
	const xdhcmn::digest_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, ContentMethod, SelectionMethod;
qRB
	Type.Init();
	Parameters.Init();
	ContentMethod.Init();
	SelectionMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, ContentMethod, Method, SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetFocusingMethod(
	const str::string_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, ContentMethod, SelectionMethod;
qRB
	Type.Init();
	Parameters.Init();
	ContentMethod.Init();
	SelectionMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, ContentMethod, Method, SelectionMethod );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetSelectionMethod(
	const xdhcmn::digest_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, ContentMethod, FocusingMethod;
qRB
	Type.Init();
	Parameters.Init();
	ContentMethod.Init();
	FocusingMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, ContentMethod, FocusingMethod, Method );
qRR
qRT
qRE
}

void xdhutl::ExtractWidgetSelectionMethod(
	const str::string_ &Description,
	str::string_ &Method )
{
qRH
	str::string Type, Parameters, ContentMethod, FocusingMethod;
qRB
	Type.Init();
	Parameters.Init();
	ContentMethod.Init();
	FocusingMethod.Init();

	ExtractWidgetFeatures( Description, Type, Parameters, ContentMethod, FocusingMethod, Method );
qRR
qRT
qRE
}

namespace {
	void ExtractWidgetTypeAndParameters_(
		const xdhcmn::digest_ &XDescription,	// Contains also the element id.
		str::string_ &Id,
		str::string_ &Type,
		str::string_ &Parameters )
	{
	qRH
		xdhcmn::digest Description;
		xdhcmn::retriever__ Retriever;
	qRB
		Retriever.Init( XDescription );
		
		Retriever.GetString( Id );

		Description.Init();
		Retriever.GetTable( Description );

		ExtractWidgetTypeAndParameters( Description, Type, Parameters );
	qRR
	qRT
	qRE
	}
}

void xdhutl::ExtractWidgetsTypesAndParametersSets(
	const xdhcmn::digest_ &Descriptions,
	str::strings_ &Ids,
	str::strings_ &Types,
	str::strings_ &ParametersSets )
{
qRH
	xdhcmn::digest XDescription;
	str::string Id, Type, Parameters;
	xdhcmn::retriever__ Retriever;
qRB
	Retriever.Init( Descriptions );

	while ( Retriever.Availability() != strmrg::aNone ) {

		XDescription.Init();
		Retriever.GetTable( XDescription );

		Id.Init();
		Type.Init();
		Parameters.Init();

		ExtractWidgetTypeAndParameters_( XDescription, Id, Type, Parameters );

		Ids.Append( Id );
		Types.Append( Type );
		ParametersSets.Append( Parameters );
	}
qRR
qRT
qRE
}

void xdhutl::GetTags(
	const xdhcmn::digest_ &Digest,
	str::string_ &Id,
	str::dStrings &Tags )
{
qRH
	xdhcmn::retriever__ Retriever;
qRB
	Retriever.Init( Digest );

	Retriever.GetString( Id );
	Retriever.GetStrings( Tags );
qRR
qRT
qRE
}

namespace  {
	void Fill_(
		const str::dString &Id,
		const str::dStrings &Tags,
		str::dStrings &Ids,
		str::dStrings &TagsSet )
	{
		sdr::sRow Row = Tags.First();

		while ( Row != qNIL ) {
			if ( Ids.Append( Id ) != TagsSet.Append( Tags( Row ) ) )
				qRFwk();

			Row = Tags.Next( Row );
		}
	}
}

void xdhutl::GetTags(
	const xdhcmn::digest_ &Digest,
	str::strings_ &Ids,
	str::strings_ &TagsSet )
{
qRH
	xdhcmn::retriever__ Retriever;
	xdhcmn::digest Description;
	str::string Id;
	str::wStrings Tags; 
qRB
	Retriever.Init( Digest );

	while ( Retriever.Availability() != strmrg::aNone ) {
		Description.Init();
		Retriever.GetTable( Description );

		Id.Init();
		Tags.Init();
		GetTags( Description, Id, Tags );

		Fill_( Id, Tags, Ids, TagsSet );
	}
qRR
qRT
qRE
}

Q37_GCTOR( xdhutl )
{
	InitAndFillAutomats_();
}