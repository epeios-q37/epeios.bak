/*
	'misc' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhcefq' tool.
	Copyright (C) 2014 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "misc.h"
#include "stsfsm.h"

#include "registry.h"

#include "scltool.h"

#include "ids.h"

using namespace misc;

static ids::ids_store<sdr::row__> Ids_;	// Pour le gnrateur d'indetifiant d'lment, lorsque l'id n'est pas dfinit par l'utilisateur dans le layout.

static stsfsm::automat RenderAutomat_, ClientAutomat_;

template <typename t> static void GetName_(
	cef_domnode_t *Node,
	t &Buffer )
{
qRH
	cef_string_userfree_t Name;
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );
	Name = Node->get_name( Node );

	if ( Name == NULL )
		qRGnr();

	cef_convert_from( Name, &CEFBuffer );
	misc::nstring___( CEFBuffer.str ).UTF8( Buffer );
qRR
qRT
	if ( Name != NULL )
		cef_string_userfree_free( Name );

	misc::Clear( &CEFBuffer );
qRE
}

str::string_ &misc::GetName(
	cef_domnode_t *Node,
	str::string_ &Name )
{
	GetName_<str::string_>( Node, Name );

	return Name;
}

const char *misc::GetName(
	cef_domnode_t *Node,
	TOL_CBUFFER___ &Buffer )
{
	GetName_<TOL_CBUFFER___>( Node, Buffer );

	return Buffer;
}

str::string_ &misc::GetName(
	const nstring___ &Id,
	cef_frame_t *Frame,
	str::string_ &Name )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Name.Append( GetName( Id, Frame, Buffer ) );
qRR
qRT
qRE
	return Name;
}

const char *misc::GetName(
	const nstring___ &Id,
	cef_frame_t *Frame,
	TOL_CBUFFER___ &Buffer )
{
	return MGetProperty( Id, "tagName", Frame, Buffer );
}

template <typename type> static inline type GetId_(
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

template <typename type> static inline type GetId_(
	const cef_string_t *Pattern,
	const stsfsm::automat_ &Automat,
	type UndefinedValue,
	bso::uint__ Amount )
{
	type Type = UndefinedValue;
qRH
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );
	cef_convert_from( Pattern, &CEFBuffer );
	Type = GetId_( CEFBuffer.str, Automat, UndefinedValue, Amount );
qRR
qRT
qRE
	return Type;
}

#define RM( name )	case rm##name : return #name; break

const char *misc::GetLabel( render_message__ Message )
{
	switch ( Message ) {
	RM( Start );
	RM( HandleSelectedFiles );
//	RM( HandleAction );
	RM( Close );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

render_message__ misc::GetRenderMessage( const cef_string_t *Pattern )
{
	return GetId_( Pattern, RenderAutomat_, rm_Undefined, rm_amount );
}

#define CM( name )	case cm##name : return #name; break

const char *misc::GetLabel( client_message__ Message )
{
	switch ( Message ) {
	CM( OpenFile );
	CM( OpenFiles );
	CM( SaveFile );
	CM( Closed );
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}

const str::string_ &misc::GenerateId( str::string_ &Id )
{
	bso::integer_buffer__ Buffer;

	Id.Append("Computer-generated id #");
	Id.Append( bso::Convert( *Ids_.New(), Buffer ) );

	return Id;
}


void misc::GetString(
	cef_list_value_t *ListValue,
	str::string_ &String )
{
qRH
	cef_string_userfree_t Value = NULL;
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );
	if ( ListValue->get_size( ListValue ) != 1 )
		qRGnr();

	if ( ListValue->get_type( ListValue, 0 ) != VTYPE_STRING )
		qRGnr();

	Value = ListValue->get_string( ListValue, 0 );

	if ( Value != NULL ) {
		cef_convert_from( Value, &CEFBuffer );

		nstring___( CEFBuffer.str ).UTF8( String );
	}
qRR
qRT
	if ( Value != NULL )
		cef_string_userfree_free( Value );

	misc::Clear( &CEFBuffer );
qRE
}


void misc::GetArgs(
	cef_list_value_t *ListValue,
	xdhcbk::args_ &Args )
{
qRH
	str::string String;
qRB
	String.Init();
	GetString( ListValue, String );	

	xdhcbk::Split( String, Args );
qRR
qRT
qRE
}

void misc::PutString(
	const str::string_ &String,
	cef_list_value_t *ListValue )
{
qRH
	cef_string___ CEFString;
qRB
	ListValue->set_size( ListValue, 1 );

	CEFString.Init( String );

	ListValue->set_string( ListValue, 0, CEFString );
qRR
qRT
qRE
}

void misc::PutArgs(
	const xdhcbk::args_ &Args,
	cef_list_value_t *ListValue )
{
qRH
	str::string Merged;
qRB
	Merged.Init();

	xdhcbk::Merge( Args, Merged );

	PutString( Merged, ListValue );
qRR
qRT
qRE
}

client_message__ misc::GetClientMessage( const cef_string_t *Pattern )
{
	return GetId_( Pattern, ClientAutomat_, cm_Undefined, cm_amount );
}

const char *misc::ExecuteJavascript(
	const nstring___ &Script,
	TOL_CBUFFER___ *Buffer,
	cef_frame_t *Frame )
{
qRH
	misc::cef_string___ CEFString;
	cef_v8context_t *V8Context = NULL;
	cef_v8value_t *V8Value = NULL;
	cef_v8exception_t *V8Exception = NULL;
	cef_string_userfree_t Value = NULL;
	bso::integer_buffer__ IBuffer;
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );	// Must be here, or the 'Clear()...' at 'qRT' doesn't work !

	if ( Script.Amount() == 0 )
		qRReturn;

	CEFString.Init( Script );

	if ( Frame != NULL )
		V8Context = Frame->get_v8context( Frame );
	else if ( cef_v8context_in_context() )
		V8Context = cef_v8context_get_current_context();
	else
		qRGnr();

	if ( V8Context == NULL )
		qRGnr();

	if ( !V8Context->is_valid( V8Context ) )
		qRGnr();

	/*
	cio::COut << __LOC__ << txf::nl << txf::commit;
	cio::COut << CEFString()->length << "," << Script.Amount() << " : " << Script << txf::nl << txf::commit;
	cio::COut << __LOC__ << txf::nl << txf::commit;
	*/

	if ( V8Context->eval( V8Context, CEFString, &V8Value, &V8Exception ) != 1 )
		qRGnr();

	if ( Buffer != NULL ) {
		if ( !V8Value->is_undefined( V8Value ) ) {
			if ( V8Value->is_string(V8Value) ) {
				if ( ( Value = V8Value->get_string_value( V8Value ) ) != NULL ) { // NOTA : if the script returns an empty string, 'Value' is NULL !!!

					cef_convert_from( Value, &CEFBuffer );
					nstring___( CEFBuffer.str ).UTF8( *Buffer );
				}  else
					Buffer->reset();
			} else if ( V8Value->is_int( V8Value ) ) {
				str::string( bso::Convert( (bso::int__)V8Value->get_int_value( V8Value ), IBuffer ) ).Convert( *Buffer );
			} else if ( V8Value->is_bool( V8Value ) ) {
				if ( V8Value->get_bool_value( V8Value ) == 0 )
					str::string( "false" ).Convert( *Buffer );
				else
					str::string( "true" ).Convert( *Buffer );
			} else
				Buffer->reset();
		} else
			Buffer->reset();
	}
qRR
qRT
	if ( Value != NULL )
		cef_string_userfree_free( Value );

	if ( V8Context != NULL )
		misc::Release( V8Context );

	misc::Clear( &CEFBuffer );
qRE
	if ( Buffer != NULL )
		return *Buffer;
	else
		return NULL;
}

void misc::AppendTag(
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
	xdhcbk::Escape( str::string( Value.UTF8( Buffer ) ), EscapedValue, '"' );
	Values.Append( EscapedValue );

	NameForRawValue.Init( Name );
	NameForRawValue.Append( '_' );

	Names.Append( NameForRawValue );
	Values.Append( str::string( Value.UTF8( Buffer ) ) );
qRR
qRT
qRE
}

namespace {
	void SetXML_(
		const nstring___ &Message,
		str::string_ &XML )
	{
	qRH
		flx::E_STRING_TOFLOW___ STOFlow;
		xml::writer Writer;
		str::string Buffer;
	qRB
		STOFlow.Init( XML );
		Writer.Init( STOFlow, xml::oCompact, xml::e_Default );

		Buffer.Init();
		Writer.PutValue( Message.UTF8( Buffer ), "Content" );
	qRR
	qRT
	qRE
	}

	inline void SetXSL_( str::string_ &XSL )
	{
		XSL.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<xsl:stylesheet version=\"1.0\"\
			                xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
				<xsl:output method=\"text\"\
					        encoding=\"utf-8\"/>\
				<xsl:template match=\"/\">\
					<xsl:value-of select=\"Content\"/>\
				</xsl:template>\
			</xsl:stylesheet>\
		");
	}
}

void misc::Alert(
	const nstring___ &Message,
	cef_frame_t *Frame )
{
qRH
	str::string Script, XML, XSL, CloseText;
qRB
	XML.Init();
	SetXML_( Message, XML );

	XSL.Init();
	SetXSL_( XSL );

	CloseText.Init();
	sclmisc::GetBaseTranslation( "CloseText", CloseText );

	Script.Init();
	scripter::DialogAlert( XML, XSL, nstring___(), CloseText, Script );

	ExecuteJavascript( Script,Frame );
qRR
qRT
qRE
}

const char *misc::OGetProperty(
	const nstring___ &Id,
	const nstring___ &Name,
	cef_frame_t *Frame,
	TOL_CBUFFER___ &Property )
{
qRH
	str::string Script;
qRB
	Script.Init();
	scripter::GetProperty( Id, Name, Script );

	ExecuteJavascript( Script, &Property, Frame );
qRR
qRT
qRE
	return Property;
}

void misc::SetAttribute(
	const nstring___ &Id,
	const nstring___ &Name,
	const nstring___ &Value,
	cef_frame_t *Frame )
{
qRH
	str::string Script;
qRB
	Script.Init();
	scripter::SetAttribute( Id, Name, Value, Script );

	ExecuteJavascript( Script, NULL, Frame );
qRR
qRT
qRE
}

const char *misc::OGetAttribute(
	const nstring___ &Id,
	const nstring___ &Name,
	cef_frame_t *Frame,
	TOL_CBUFFER___ &Attribute )
{
qRH
	str::string Script;
qRB
	Script.Init();
	scripter::GetAttribute( Id, Name, Script );

	ExecuteJavascript( Script, &Attribute, Frame );
qRR
qRT
qRE
	return Attribute;
}

bso::bool__ misc::OGetAttribute(
	const nstring___ &Id,
	const nstring___ &Name,
	cef_frame_t *Frame,
	str::string_ &Value )
{
	bso::bool__ Exists = true;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	if ( OGetAttribute( Id, Name, Frame, Buffer ) == NULL )
		Exists = false;
	else
		Value.Append( Buffer );
qRR
qRT
qRE
	return Exists;
}

void misc::RemoveAttribute(
	const nstring___ &Id,
	const nstring___ &Name,
	cef_frame_t *Frame )
{
qRH
	str::string Script;
qRB
	Script.Init();
	scripter::RemoveAttribute( Id, Name, Script );

	ExecuteJavascript( Script, NULL, Frame );
qRR
qRT
qRE
}

const str::string_ &misc::GetOrGenerateId(
	cef_domnode_t *Node,
	str::string_ &Id )
{
qRH
	TOL_CBUFFER___ Buffer;
qRB
	Id.Init( misc::OGetAttribute(Node, "id", Buffer ) );

	if ( Id.Amount() == 0 ) {
		misc::GenerateId( Id );
		misc::SetAttribute( Node, "id", Id );
	}
qRR
qRT
qRE
	return Id;
}

const char *misc::GetOrGenerateId(
	cef_domnode_t *Node,
	TOL_CBUFFER___ &Buffer )
{
qRH
	str::string Id;
qRB
	Id.Init();

	GetOrGenerateId( Node, Id );

	Id.Convert( Buffer );
qRR
qRT
qRE
	return Buffer;
}

void misc::SetContent(
	const nstring___ &Id,
	const nstring___ &Value,
	cef_frame_t *Frame )
{
qRH
	str::string Script;
qRB
	Script.Init();
	scripter::SetContent( Id, Value, Script );

	ExecuteJavascript( Script, NULL, Frame );
qRR
qRT
qRE
}

void misc::SetAttribute(
	cef_domnode_t *Node,
	const nstring___ &RawName,
	const nstring___ &RawValue )
{
qRH
	cef_string___ Name, Value;
qRB
	Name.Init( RawName );
	Value.Init( RawValue );

	Node->set_element_attribute( Node, Name, Value );
qRR
qRT
qRE
}

const char *misc::OGetAttribute(
	cef_domnode_t *Node,
	const nstring___ &Name,
	TOL_CBUFFER___ &Buffer )
{
qRH
	cef_string_userfree_t RawValue = NULL;
	str::string Value;
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );

	RawValue = Node->get_element_attribute(Node, misc::cef_string___(  Name ) );

	if ( RawValue != NULL ) {
		cef_convert_from( RawValue, &CEFBuffer );
		nstring___( CEFBuffer.str ).UTF8( Buffer );
	} else
		Buffer.reset();
qRR
qRT
	if ( RawValue != NULL )
		cef_string_userfree_free( RawValue );

	misc::Clear( &CEFBuffer );
qRE
	return Buffer;
}

bso::bool__ misc::OGetAttribute(
	cef_domnode_t *Node,
	const nstring___ &Name,
	str::string_ &Value )
{
	bso::bool__ Exists = true;
qRH
	TOL_CBUFFER___ Buffer;
qRB
	if ( OGetAttribute( Node, Name, Buffer ) != NULL )
		Value.Append( Buffer );
	else
		Exists = false;
qRR
qRT
qRE
	return Exists;
}

void misc::FillList(
	const str::strings_ &Source,
	cef_string_list_t &Target )
{
	ctn::E_CMITEM( str::string_ ) Value;
	sdr::row__ Row = Source.First();

	Value.Init( Source );

	while ( Row != qNIL ) {
		cef_string_list_append( Target, cef_string___( Value( Row ) ) );

		Row = Source.Next( Row );
	}
}

template <typename m, typename p, typename f> static void SendMessage_(
	m RawMessageLabel,
	cef_process_id_t PID,
	cef_browser_t *Browser,
	const p &P,
	f &F)
{
qRH
	cef_string___ MessageLabel;
	cef_process_message_t *Message = NULL;
	cef_list_value_t *ListValues = NULL;
qRB
	MessageLabel.Init( GetLabel( RawMessageLabel ) );

	Message = cef_process_message_create( &MessageLabel );

	ListValues = Message->get_argument_list( Message );

	F( P, ListValues );

	Browser->send_process_message( Browser, PID, Message );
qRR
qRT
	if ( ListValues != NULL )
		misc::Release( ListValues );
qRE
}

void misc::SendMessage(
	client_message__ RawMessage,
	cef_browser_t *Browser,
	const str::string_ &Param )
{
	SendMessage_( RawMessage, PID_BROWSER, Browser, Param, PutString );
}

void misc::SendMessage(
	client_message__ RawMessage,
	cef_browser_t *Browser,
	const xdhcbk::args_ &Args )
{
	SendMessage_( RawMessage, PID_BROWSER, Browser, Args, PutArgs );
}

void misc::SendMessage(
	render_message__ RawMessage,
	cef_browser_t *Browser,
	const str::string_ &Param )
{
	SendMessage_( RawMessage, PID_RENDERER, Browser, Param, PutString );
}

void misc::SendMessage(
	render_message__ RawMessage,
	cef_browser_t *Browser,
	const xdhcbk::args_ &Args )
{
	SendMessage_( RawMessage, PID_RENDERER, Browser, Args, PutArgs );
}

static const str::string_ &GetDefaultEvent_(
	const str::string_ &Id,
	cef_frame_t *Frame,
	str::string_ &Event )
{
qRH
	str::string Name;
qRB
	Name.Init();
	GetName( Id, Frame, Name );

	xdhjst::GetTagDefaultEvent( Name, Event );
qRR
qRT
qRE
	return Event;
}

namespace {
	void GetItems_(
		const str::string_ &Id,
		const rgstry::tentry__ &ItemAttributeName,
		const rgstry::tentry__ &ItemsAttributeName,
		cef_frame_t *Frame,
		xdhcbk::args_ &Items )
	{
	qRH
		str::string Value;
		TOL_CBUFFER___ Buffer;
		xdhcbk::args OtherItems;
	qRB
		Value.Init();
		OGetAttribute( Id, sclmisc::MGetValue( ItemAttributeName, Buffer ), Frame, Value );

		if ( Value.Amount() != 0 ) {
			OtherItems.Init();
			xdhcbk::Split( Value, OtherItems );
			Items.AppendMono( OtherItems );
		}

		Value.Init();
		OGetAttribute( Id, sclmisc::MGetValue( ItemsAttributeName, Buffer ), Frame, Value );

		if ( Value.Amount() != 0 ) {
			OtherItems.Init();
			xdhcbk::Split( Value, OtherItems );
			Items.AppendMulti( OtherItems );
		}
	qRR
	qRT
	qRE
	}
}



void misc::GetEvents(
	const str::string_ &Id,
	cef_frame_t *Frame,
	xdhcbk::args_ &Events )
{
	GetItems_( Id, registry::custom_item::attribute_name::OnEvent, registry::custom_item::attribute_name::OnEvents, Frame, Events );
}

void misc::GetEvents(
	const str::string_ &Id,
	cef_frame_t *Frame,
	str::string_ &Events )
{
qRH
	str::string Value;
	TOL_CBUFFER___ Buffer;
qRB
	Value.Init();
	OGetAttribute( Id, sclmisc::MGetValue( registry::custom_item::attribute_name::OnEvent, Buffer ), Frame, Value );

	if ( Value.Amount() != 0 ) {
		Events.Append( '(' );
		Events.Append( Value );
		Events.Append( ")|");
	}

	Value.Init();
	OGetAttribute( Id, sclmisc::MGetValue( registry::custom_item::attribute_name::OnEvents, Buffer ), Frame, Value );

	if ( Value.Amount() != 0 )
		Events.Append( Value );
qRR
qRT
qRE
}

void misc::GetEventsAbstracts(
	const str::string_ &Id,
	cef_frame_t *Frame,
	event_abstracts_ &Abstracts )
{
qRH
	str::string Events, Name;
	xdhcbk::args Definitions;
	TOL_CBUFFER___ Buffer;
qRB
	Events.Init();
	GetEvents( Id, Frame, Events );

	Name.Init();
	GetName( Id, Frame, Name );

	xdhjst::GetEventsAbstracts( Name, Events, Abstracts );
qRR
qRT
qRE
}

void misc::GetCasts(
	const str::string_ &Id,
	cef_frame_t *Frame,
	xdhcbk::args_ &Casts )
{
	GetItems_( Id, registry::custom_item::attribute_name::Cast, registry::custom_item::attribute_name::Casts, Frame, Casts );
}

static const str::string_ &GetDefaultEvent_(
	cef_frame_t *Frame,
	const nstring___ &Id,
	str::string_ &Event )
{
qRH
	str::string Name;
qRB
	Name.Init();
	GetName( Id, Frame, Name );

	xdhjst::GetTagDefaultEvent( Name, Event );
qRR
qRT
qRE
	return Event;
}

static cef_v8value_t *GetFunction_(
	cef_v8value_t *Object,
	const str::string_ &Name )
{
	cef_v8value_t *Function = NULL;
qRH
	cef_string___ CEFName;
qRB
	CEFName.Init( Name );

	if ( !Object->has_value_bykey( Object, CEFName ) )
		qRReturn;

	Function = Object->get_value_bykey( Object, CEFName );

	if ( Function == NULL  )
		qRReturn;

	if ( !Function->is_function( Function ) )
		Function = NULL;
qRR
qRT
qRE
	return Function;
}

cef_v8value_t *misc::GetFunction(
	cef_v8value_t *Object,
	const str::string_ &Name,
	err::handling__ ErrHandling )
{
	cef_v8value_t *Function = GetFunction_( Object, Name );

	if ( Function == NULL )
		if ( ErrHandling == err::hThrowException )
			qRGnr();

	return Function;
}

static void InitAndFillRenderAutomat_( void )
{
	RenderAutomat_.Init(),
	stsfsm::Fill( RenderAutomat_, rm_amount, GetLabel );
}

static void InitAndFillFillClientAutomat_( void )
{
	ClientAutomat_.Init();
	stsfsm::Fill( ClientAutomat_, cm_amount, GetLabel );
}

static void InitAndFillAutomats_( void )
{
	InitAndFillRenderAutomat_();
	InitAndFillFillClientAutomat_();
}

static mtx::handler___ Mutex_ = mtx::UndefinedHandler;

str::string Response_;

const str::string_ &misc::WaitForResponse( str::string_ &Response )
{
	if ( !mtx::TryToLock( Mutex_ ) )
		qRGnr();

	mtx::Lock( Mutex_ );

	Response.Append( Response_ );

	mtx::Unlock( Mutex_ );

	return Response;
}

void misc::SendResponse( const str::string_ &Response )
{
	Response_.Init( Response );

	mtx::Unlock( Mutex_ );
}

Q37_GCTOR( misc )
{
	Ids_.Init();
	InitAndFillAutomats_();
	Mutex_ = mtx::Create();
	Response_.Init();
}

