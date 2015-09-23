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

#define XDHUJT__COMPILATION

#include "xdhujt.h"

#include "xdhujr.h"

#include "sclmisc.h"

#include "stsfsm.h"

using namespace xdhujt;

#define C( name, entry )\
	case sn##name:\
		sclmisc::MGetValue( xdhujr::script::entry, Buffer );\
		break

const str::string_ &xdhujt::GetTaggedScript(
	script_name__ Script,
	str::string_ &Buffer )
{
	switch ( Script ) {
	C( Log, Log );
	C( DialogAlert, dialog::Alert );
	C( DialogConfirm, dialog::Confirm );
	C( ElementFiller, ElementFiller );
	C( DocumentFiller, DocumentFiller );
	C( CastingFiller, CastingFiller );
	C( PropertySetter, property::Setter );
	C( PropertyGetter, property::Getter );
	C( AttributeSetter, attribute::Setter );
	C( AttributeGetter, attribute::Getter );
	C( AttributeRemover, attribute::Remover );
	C( WidgetContentRetriever, widget::ContentRetriever );
	C( ContentSetter, content::Setter );
	C( ContentGetter, content::Getter );
	C( WidgetFocuser, widget::Focuser );
	C( Focuser, Focuser );
	default:
		qRFwk();
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
	qRH
		str::string NameForRawValue;
		str::string EscapedValue;
		TOL_CBUFFER___ Buffer;
	qRB
		Names.Append( str::string( Name ) );
		EscapedValue.Init();
		xdhcmn::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
		Values.Append( EscapedValue );

		NameForRawValue.Init( Name );
		NameForRawValue.Append( '_' );

		Names.Append( NameForRawValue );
		Values.Append( str::string( Value.UTF8( Buffer ) ) );
	qRR
	qRT
	qRE
	}

	void SubstituteTags_(
		str::string_ &Script,	// Script with tags. When returning, tags are substitued.
		va_list ValueList,
		... )	// The list of the tag name, as 'const char *', with 'NULL' as end marker.
	{
	qRH
		str::strings Names, Values;
		va_list NameList;
		const bso::char__ *Name = NULL;
	qRB
		Names.Init();
		Values.Init();

		va_start( NameList, ValueList );

		Name = va_arg( NameList, const bso::char__ * );

		while ( Name != NULL ) {
			AppendTag_( Name, va_arg( ValueList, const nchar__ * ), Names, Values );

			Name = va_arg( NameList, const bso::char__ * );
		}

		tagsbs::SubstituteLongTags( Script, Names, Values );
	qRR
	qRT
		va_end( NameList );
	qRE
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

void xdhujt::GetScript(
	script_name__ ScriptName,
	str::string_ &Script,
	va_list List )
{
qRH
	str::string TaggedScript;
qRB
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
	S( ElementFiller, Id_, XML_, XSL_, NULL );
	S( DocumentFiller, Id_, XML_, XSL_, NULL );
	S( CastingFiller, Id_, XML_, XSL_, NULL );
	S( ContentSetter, Id_, Value_, NULL );
	S( ContentGetter, Id_, NULL );
	S( WidgetContentRetriever, Id_, Method_, NULL );
	S( WidgetFocuser, Id_, Method_, NULL );
	S( Focuser, Id_, NULL );
	default:
		qRFwk();
		break;
	}

	Script.Append( TaggedScript );
qRR
qRT
qRE
}

const str::string_ &xdhujt::GetScript(
	script_name__ ScriptName,
	str::string_ *Buffer,
	... )
{
qRH
	va_list List;
qRB
	va_start( List, Buffer );
	
	GetScript( ScriptName, *Buffer, List );
qRR
qRT
	va_end( List );
qRE
	return *Buffer;
}
/*
void xdhujt::scripter::HandleWidgetsDigest(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
qRH
	xdhcbk::args Features;
	str::string Id, Type, Parameters, SubScript;
	str::strings TagNames, TagValues;
	xdhcbk::retriever__ Retriever;
qRB
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Features.Init();
	Retriever.GetTable( Features );

	SubScript.Init();
	sclmisc::MGetValue( xdhjsr::script::widget::Instantiator, SubScript );

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
qRR
qRT
qRE
}

void xdhujt::scripter::HandleWidgetsDigests(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
qRB
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleWidgetsDigest( FrameId, Digest, Script );
	}
qRR
qRT
qRE
}

namespace {
	void SetDocumentReferer_(
		const str::string_ &Id,	// If 'Id' != 'NULL', id ot a frame, otherwise we deal with the current document.
		str::string_ &Referer )
	{
		if ( Id.Amount() == 0 )
			Referer.Append( "document" );
		else
			Referer.Append( "document.getElementById(\"Id\").contentDocument" );
	}

	void SetupEvent_(
		const str::string_ &FrameId,
		const event_abstract_ &Abstract,
		const str::string_ &ElementId,
		str::string_ &Script )
	{
	qRH
		str::string SubScript;
		str::strings TagNames, TagValues;
		str::string DocumentReferer;
	qRB
		DocumentReferer.Init();
		SetDocumentReferer_( FrameId, DocumentReferer );
		AppendTag_( "Document", DocumentReferer, TagNames, TagValues );

		TagNames.Init();
		TagValues.Init();

		AppendTag_("Id", ElementId, TagNames, TagValues );

		if ( Abstract.Event == "drop" ) {
			SubScript.Init();
			sclmisc::MGetValue( xdhjsr::script::DroppableSetter, SubScript );
			tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );
			Script.Append( SubScript );
		} else if ( Abstract.Event == "dragstart" ) {
			SubScript.Init();
			sclmisc::MGetValue( xdhjsr::script::DraggableSetter, SubScript );
			tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );
			Script.Append( SubScript );
		} else {
			SubScript.Init();
			sclmisc::MGetValue( xdhjsr::script::EventSetter, SubScript );

			AppendTag_("Event", Abstract.Event, TagNames, TagValues );
			tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

			Script.Append( SubScript );
		}
	qRR	
	qRT
	qRE
	}

	void SetupEvents_(
		const str::string_ &FrameId,
		const event_abstracts_ &Abstracts,
		const str::string_ &ElementId,
		str::string_ &Script )
	{
		ctn::E_CITEM( event_abstract_ ) Abstract;
		sdr::row__ Row = Abstracts.First();

		Abstract.Init( Abstracts );

		while ( Row != qNIL ) {
			SetupEvent_( FrameId, Abstract( Row ), ElementId, Script );

			Row = Abstracts.Next( Row) ;
		}
	}

}

void xdhujt::scripter::HandleEventsDigest(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	str::string Id, Name;
	xdhcbk::args Events;
	event_abstracts Abstracts;
qRB
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Name.Init();
	Retriever.GetString( Name );

	Events.Init();
	Retriever.GetTable( Events );

	Abstracts.Init();
	FillEventAbstracts( Name, Events, Abstracts );

	SetupEvents_( FrameId, Abstracts, Id, Script );
qRR
qRT
qRE
}

void xdhujt::scripter::HandleEventsDigests(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
qRB
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleEventsDigest( FrameId, Digest, Script );
	}
qRR
qRT
qRE
}

void xdhujt::scripter::HandleEventsWidgetsDigests(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	xdhcbk::args EventsDigests, WidgetsDigests;
qRB
	Retriever.Init( Digests );

	EventsDigests.Init();
	Retriever.GetTable( EventsDigests );

	WidgetsDigests.Init();
	Retriever.GetTable( WidgetsDigests );

	xdhujt::scripter::HandleEventsDigests( FrameId, EventsDigests, Script );
	xdhujt::scripter::HandleWidgetsDigests( FrameId, WidgetsDigests, Script );
qRR
qRT
qRE
}

void xdhujt::scripter::HandleEventsWidgetsDigests(
	const str::string_ &FrameId,
	const str::string_ &RawDigests,
	str::string_ &Script )
{
qRH
	xdhcbk::args Digests;
qRB
	Digests.Init();
	xdhcbk::Split( RawDigests, Digests );

	HandleEventsWidgetsDigests( FrameId, Digests, Script );
qRR
qRT
qRE
}


namespace{
	void SetupCast_(
		const str::string_ &FrameId,
		const str::string_ &ElementId,
		const str::string_ &Cast,
		str::string_ &Script )
	{
	qRH
		str::string SubScript;
		str::strings TagNames, TagValues;
		str::string DocumentReferer;
	qRB
		DocumentReferer.Init();
		SetDocumentReferer_( FrameId, DocumentReferer );
		AppendTag_( "Document", DocumentReferer, TagNames, TagValues );

		SubScript.Init();

		TagNames.Init();
		TagValues.Init();

		AppendTag_("Id", ElementId, TagNames, TagValues );
		sclmisc::MGetValue( xdhjsr::script::CastHandler, SubScript );

		AppendTag_( "Cast", Cast, TagNames, TagValues );

		tagsbs::SubstituteLongTags( SubScript, TagNames, TagValues );

		Script.Append( SubScript );
	qRR
	qRT
	qRE
	}

	void SetupCasts_(
		const str::string_ &FrameId,
		const str::string_ &ElementId,
		const str::strings_ &Casts,
		str::string_ &Script )
	{
	qRH
		ctn::E_CMITEM( str::string_ ) Cast;
		sdr::row__ Row = qNIL;
	qRB
		Cast.Init( Casts );
		Row = Casts.First();
	
		while ( Row != qNIL ) {
			SetupCast_( FrameId, ElementId, Cast( Row ), Script );

			Row = Casts.Next( Row );
		}
	qRR
	qRT
	qRE
	}


}

void xdhujt::scripter::HandleCastsDigest(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digest,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	str::string Id;
	str::strings Casts;
qRB
	Retriever.Init( Digest );

	Id.Init();
	Retriever.GetString( Id );

	Casts.Init();
	Retriever.GetStrings( Casts );

	SetupCasts_( FrameId, Id, Casts, Script );
qRR
qRT
qRE
}

void xdhujt::scripter::HandleCastsDigests(
	const str::string_ &FrameId,
	const xdhcbk::args_ &Digests,
	str::string_ &Script )
{
qRH
	xdhcbk::retriever__ Retriever;
	xdhcbk::args Digest;
qRB
	Retriever.Init( Digests );
		
	while ( Retriever.Availability() != strmrg::aNone ) {
		Digest.Init();
		Retriever.GetTable( Digest );
		scripter::HandleCastsDigest( FrameId, Digest, Script );
	}
qRR
qRT
qRE
}
*/
