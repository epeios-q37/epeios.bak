/*
	'xdhjst.cpp' by Claude SIMON (http://zeusw.org/).

	'xdhjst' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define XDHJST__COMPILATION

#include "xdhjst.h"

#include "xdhjsr.h"

#include "sclmisc.h"

#include "stsfsm.h"

using namespace xdhjst;

#define C( name, entry )\
	case sn##name:\
		sclmisc::MGetValue( xdhjsr::script::entry, Buffer );\
		break

const str::string_ &xdhjst::GetTaggedScript(
	script_name__ Script,
	str::string_ &Buffer )
{
	switch ( Script ) {
	C( Log, dialog::Alert );
	C( DialogAlert, dialog::Alert );
	C( DialogConfirm, dialog::Confirm );
	C( DocumentSetter, DocumentSetter );
	C( ChildrenSetter, ChildrenSetter );
	C( CastingDefiner, casting::Definer );
	C( CastingHandler, casting::Handler );
	C( PropertySetter, property::Setter );
	C( PropertyGetter, property::Getter );
	C( AttributeSetter, attribute::Setter );
	C( AttributeGetter, attribute::Getter );
	C( AttributeRemover, attribute::Remover );
	C( WidgetContentRetriever, widget::ContentRetriever );
	C( ContentSetter, content::Setter );
	C( ContentGetter, content::Getter );
	C( WidgetFocusing, widget::Focusing );
	C( Focusing, Focusing );
	default:
		ERRFwk();
		break;
	}

	return Buffer;
}

namespace {
	void AppendTag_(
		const char *Name,
		const nstring___ &Value,
		str::strings_ &Names,
		str::strings_ &Values )
	{
	ERRProlog
		str::string NameForRawValue;
		str::string EscapedValue;
		TOL_CBUFFER___ Buffer;
	ERRBegin
		Names.Append( str::string( Name ) );
		EscapedValue.Init();
		xdhcbk::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
		Values.Append( EscapedValue );

		NameForRawValue.Init( Name );
		NameForRawValue.Append( '_' );

		Names.Append( NameForRawValue );
		Values.Append( str::string( Value.UTF8( Buffer ) ) );
	ERRErr
	ERREnd
	ERREpilog
	}

	void SubstituteTags_(
		str::string_ &Script,	// Script with tags. When returning, tags are substitued.
		va_list ValueList,
		... )	// The list of the tag name, as 'const char *', with 'NULL' as end marker.
	{
	ERRProlog
		str::strings Names, Values;
		va_list NameList;
		const bso::char__ *Name = NULL;
	ERRBegin
		Names.Init();
		Values.Init();

		va_start( NameList, ValueList );

		Name = va_arg( NameList, const bso::char__ * );

		while ( Name != NULL ) {
			AppendTag_( Name, va_arg( ValueList, const nchar__ * ), Names, Values );

			Name = va_arg( NameList, const bso::char__ * );
		}

		tagsbs::SubstituteLongTags( Script, Names, Values );
	ERRErr
	ERREnd
		va_end( NameList );
	ERREpilog
	}
}

#define D( name )\
	E_CDEF( char *, name##_, #name )

D( Message );
D( Id );
D( Name );
D( Value );
D( Method );
D( XML );
D( XSL );
D( Title );
D( CloseText );
D( Cast );

#define S( name, ... )\
	case sn##name:\
	SubstituteTags_( TaggedScript, List, __VA_ARGS__ );\
	break\

void xdhjst::GetScript(
	script_name__ ScriptName,
	str::string_ &Script,
	va_list List )
{
ERRProlog
	str::string TaggedScript;
ERRBegin
	TaggedScript.Init();
	GetTaggedScript( ScriptName, TaggedScript );

	switch ( ScriptName ) {
	S( Log, Message_, NULL );
	S( DialogAlert, XML_, XSL_, Title_, CloseText_, NULL );
	S( DialogConfirm, XML_, XSL_, Title_, CloseText_, NULL );
	S( AttributeSetter, Id_, Name_, Value_, NULL  );
	S( AttributeGetter, Id_, Name_, NULL  );
	S( AttributeRemover, Id_, Name_, NULL  );
	S( PropertySetter, Id_, Name_, Value_, NULL );
	S( PropertyGetter, Id_, Name_, NULL );
	S( DocumentSetter, Id_, XML_, XSL_, NULL );
	S( ContentSetter, Id_, Value_, NULL );
	S( ContentGetter, Id_, NULL );
	S( CastingDefiner, XML_, XSL_, NULL );
	S( CastingHandler, Id_, Cast_, NULL );
	S( ChildrenSetter, Id_, XML_, XSL_, NULL );
	S( WidgetContentRetriever, Id_, Method_, NULL );
	S( WidgetFocusing, Id_, Method_, NULL );
	S( Focusing, Id_, NULL );
	default:
		ERRFwk();
		break;
	}

	Script.Append( TaggedScript );
ERRErr
ERREnd
ERREpilog
}

const str::string_ &xdhjst::GetScript(
	script_name__ ScriptName,
	str::string_ *Buffer,
	... )
{
ERRProlog
	va_list List;
ERRBegin
	va_start( List, Buffer );
	
	GetScript( ScriptName, *Buffer, List );
ERRErr
ERREnd
	va_end( List );
ERREpilog
	return *Buffer;
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
	ERRProlog
		str::string Buffer;
	ERRBegin
		Buffer.Init();
		Id = stsfsm::GetId( Pattern.UTF8( Buffer ), Automat, UndefinedValue, Amount );
	ERRErr
	ERREnd
	ERREpilog
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

const char *xdhjst::GetLabel( action__ Action )
{
	switch ( Action ) {
		A( OpenFile );
		A( OpenFiles );
		A( SaveFile );
	default:
		ERRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

action__ xdhjst::GetAction( const nstring___ &Pattern )
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
	ERRProlog
		xdhcbk::retriever__ Retriever;
		str::string PredefinedAction;
	ERRBegin
		if ( ActionWithParameters.Amount() < 2 )
			ERRFwk();

		Retriever.Init( ActionWithParameters );

		PredefinedAction.Init();
		Retriever.GetString( PredefinedAction );
		Action = GetAction( PredefinedAction );

		Retriever.GetString( UserAction );

		if ( Retriever.Availability() != strmrg::aNone )
			Retriever.GetTable( Args );
	ERRErr
	ERREnd
	ERREpilog
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
			ERRDta();
			break;
		}

		return a_Undefined;	// To avoid a 'warning'.
	}
}

void xdhjst::Fill(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definition,
	event_abstract_ &Abstract )
{
ERRProlog
	xdhcbk::args ActionWithParameters;
	xdhcbk::retriever__ Retriever;
ERRBegin
	Retriever.Init( Definition );

	switch ( Definition.Amount() ) {
	case 2:
		Retriever.GetString( Abstract.Event );
	case 1:
		ActionWithParameters.Init();
		Retriever.GetTable( ActionWithParameters );
		Abstract.S_.Action = GetAction_( ActionWithParameters, Abstract.UserAction, Abstract.Args );
		break;
	default:
		ERRFwk();
		break;
	}

	if ( Abstract.Event.Amount() == 0 )
		Abstract.Event = DefaultEvent;
ERRErr
ERREnd
ERREpilog
}

void xdhjst::FillMono(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definition,
	event_abstracts_ &Abstracts )
{
ERRProlog
	event_abstract Abstract;
ERRBegin
	Abstract.Init();

	Fill( DefaultEvent, Definition, Abstract );

	Abstracts.Append( Abstract );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::FillMulti(
	const str::string_ &DefaultEvent,
	const xdhcbk::args_ &Definitions,
	event_abstracts_ &Abstracts )
{
ERRProlog
	xdhcbk::args Definition;
	strmrg::retriever__ Retriever;
ERRBegin
	Retriever.Init( Definitions );

	while ( Retriever.Availability() != strmrg::aNone ) {
		Definition.Init();

		Retriever.GetTable( Definition );

		FillMono( DefaultEvent, Definition, Abstracts );
	}
ERRErr
ERREnd
ERREpilog
}

void xdhjst::FillEventAbstracts(
	const str::string_ &TagName,
	const xdhcbk::args_ &Events,
	event_abstracts_ &Abstracts )

{
ERRProlog
	str::string DefaultEvent;
	TOL_CBUFFER___ Buffer;
ERRBegin
	DefaultEvent.Init();
	GetTagDefaultEvent( TagName, DefaultEvent );

	FillMulti( DefaultEvent, Events, Abstracts );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ xdhjst::HasEvent(
	const char *Event,
	const event_abstracts_ &Abstracts )
{
	ctn::E_CITEM( event_abstract_ ) Abstract;
	sdr::row__ Row = Abstracts.First();

	Abstract.Init( Abstracts );

	while ( ( Row != E_NIL ) && ( Abstract( Row ).Event != Event ) )
		Row = Abstracts.Next( Row );

	return Row != E_NIL;
}


sdr::row__ xdhjst::Find(
	const str::string_ &Event,
	const event_abstracts_ &Abstracts )
{
	ctn::E_CITEM( event_abstract_ ) Abstract;
	sdr::row__ Row = Abstracts.First();

	Abstract.Init( Abstracts );

	while ( ( Row != E_NIL ) && ( Abstract( Row ).Event != Event ) )
		Row = Abstracts.Next( Row );

	return Row;
}

void xdhjst::GetEventsAbstracts(
	const str::string_ &Events,
	event_abstracts_ &Abstracts )
{
ERRProlog
	str::string Name;
	xdhcbk::args Definitions;
	TOL_CBUFFER___ Buffer;
ERRBegin
	if ( Events.Amount() != 0 ) {
		Definitions.Init();
		xdhcbk::Split( Events, Definitions );
		FillEventAbstracts( Name, Definitions, Abstracts );
	}
ERRErr
ERREnd
ERREpilog
}

void xdhjst::SplitWidgetFeatures(
	const xdhcbk::args_ &Features,
	str::string_ &Type,
	str::string_ &Parameters,
	str::string_ &ContentRetrievingMethod,
	str::string_ &FocusingMethod )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
ERRBegin
	Retriever.Init( Features );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Type );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( Parameters );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( ContentRetrievingMethod );

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FocusingMethod );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::GetWidgetTypeAndParameters(
	const xdhcbk::args_ &Features,
	str::string_ &Type,
	str::string_ &Parameters )
{
ERRProlog
	str::string ContentRetrievingMethod, FocusingMethod;
ERRBegin
	ContentRetrievingMethod.Init();
	FocusingMethod.Init();
	SplitWidgetFeatures( Features, Type, Parameters, ContentRetrievingMethod, FocusingMethod );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::GetWidgetContentRetrievingMethod(
	const xdhcbk::args_ &Features,
	str::string_ &Method )
{
ERRProlog
	str::string Type, Parameters, OtherMethod;
ERRBegin
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	SplitWidgetFeatures( Features, Type, Parameters, Method, OtherMethod );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::GetWidgetFocusingMethod(
	const xdhcbk::args_ &Features,
	str::string_ &Method )
{
ERRProlog
	str::string Type, Parameters, OtherMethod;
ERRBegin
	Type.Init();
	Parameters.Init();
	OtherMethod.Init();

	SplitWidgetFeatures( Features, Type, Parameters, OtherMethod, Method );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::scripter::HandleWidgetFeatures(
	const str::string_ &Id,
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
ERRProlog
	str::string SubScript, Type, Parameters;
	str::strings TagNames, TagValues;
	TOL_CBUFFER___ Buffer;
ERRBegin
	SubScript.Init();
	sclmisc::MGetValue( xdhjsr::script::widget::Instantiation, SubScript );

	Type.Init();
	Parameters.Init();
	GetWidgetTypeAndParameters( Digest, Type, Parameters );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Type", Type, TagNames, TagValues );
	AppendTag_( "Parameters", Parameters, TagNames, TagValues );

	tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

	Script.Append( SubScript );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::scripter::HandleWidgetDigest(
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::args Features;
	str::string Id, Type, Parameters, SubScript;
	str::strings TagNames, TagValues;
	xdhcbk::retriever__ Retriever;
ERRBegin
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Features.Init();
	Retriever.GetTable( Features );

	SubScript.Init();
	sclmisc::MGetValue( xdhjsr::script::widget::Instantiation, SubScript );

	Type.Init();
	Parameters.Init();
	GetWidgetTypeAndParameters( Features, Type, Parameters );

	TagNames.Init();
	TagValues.Init();

	AppendTag_( "Id", Id, TagNames, TagValues );
	AppendTag_( "Type", Type, TagNames, TagValues );
	AppendTag_( "Parameters", Parameters, TagNames, TagValues );

	tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

	Script.Append( SubScript );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::scripter::HandleWidgetDigests(
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
ERRBegin
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleWidgetDigest( Digest, Script );
	}
ERRErr
ERREnd
ERREpilog
}

namespace {
	void SetupEvent_(
		const event_abstract_ &Abstract,
		const str::string_ &Id,
		str::string_ &Script )
	{
	ERRProlog
		str::string SubScript;
		str::strings TagNames, TagValues;
	ERRBegin
		if ( Abstract.Event == "drop" )
			scripter::SetAttribute( Id, "ondragover", "event.preventDefault();", Script );

		if ( Abstract.Event == "dragstart" )
			scripter::SetAttribute( Id, "draggable", "true", Script );

		SubScript.Init();
		sclmisc::MGetValue( xdhjsr::script::EventHandler, SubScript );

		TagNames.Init();
		TagValues.Init();

		AppendTag_("Id", Id, TagNames, TagValues );
		AppendTag_("Event", Abstract.Event, TagNames, TagValues );

		tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

		Script.Append( SubScript );
	ERRErr	
	ERREnd
	ERREpilog
	}

	void SetupEvents_(
		const event_abstracts_ &Abstracts,
		const str::string_ &Id,
		str::string_ &Script )
	{
		ctn::E_CITEM( event_abstract_ ) Abstract;
		sdr::row__ Row = Abstracts.First();

		Abstract.Init( Abstracts );

		while ( Row != E_NIL ) {
			SetupEvent_( Abstract( Row ), Id, Script );

			Row = Abstracts.Next( Row) ;
		}
	}

}

void xdhjst::scripter::HandleEventsDigest(
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
	str::string Id, Name;
	xdhcbk::args Events;
	event_abstracts Abstracts;
ERRBegin
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Name.Init();
	Retriever.GetString( Name );

	Events.Init();
	Retriever.GetTable( Events );

	Abstracts.Init();
	FillEventAbstracts( Name, Events, Abstracts );

	SetupEvents_( Abstracts, Id, Script );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::scripter::HandleEventsDigests(
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
ERRBegin
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleEventsDigest( Digest, Script );
	}
ERRErr
ERREnd
ERREpilog
}

namespace{
	void SetupCast_(
		const str::string_ &Id,
		const str::string_ &Cast,
		str::string_ &Script )
	{
	ERRProlog
		str::string SubScript;
		str::strings TagNames, TagValues;
	ERRBegin
		SubScript.Init();
		sclmisc::MGetValue( xdhjsr::script::casting::Handler, SubScript );

		TagNames.Init();
		TagValues.Init();

		AppendTag_("Id", Id, TagNames, TagValues );
		AppendTag_("Cast", Cast, TagNames, TagValues );

		tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

		Script.Append( SubScript );
	ERRErr
	ERREnd
	ERREpilog
	}

	void SetupCasts_(
		const str::string_ &Id,
		const str::strings_ &Casts,
		str::string_ &Script )
	{
	ERRProlog
		ctn::E_CMITEM( str::string_ ) Cast;
		sdr::row__ Row = E_NIL;
	ERRBegin
		Cast.Init( Casts );
		Row = Casts.First();
	
		while ( Row != E_NIL ) {
			SetupCast_(Id, Cast( Row ), Script );

			Row = Casts.Next( Row );
		}
	ERRErr
	ERREnd
	ERREpilog
	}


}

void xdhjst::scripter::HandleCastsDigest(
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
	str::string Id;
	str::strings Casts;
ERRBegin
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Casts.Init();
	Retriever.GetStrings( Casts );

	SetupCasts_( Id, Casts, Script );
ERRErr
ERREnd
ERREpilog
}

void xdhjst::scripter::HandleCastsDigests(
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
ERRProlog
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
ERRBegin
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleCastsDigest( Digest, Script );
	}
ERRErr
ERREnd
ERREpilog
}

Q37_GCTOR( xdhjsp )
{
	InitAndFillActionAutomat_();
}

