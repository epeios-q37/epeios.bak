/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#include "render.h"

#include "agent.h"

#include "files.h"

#include "registry.h"

#include "scltool.h"

namespace {
	typedef int (CEF_CALLBACK execute__)(
		struct _cef_v8handler_t* self,
		const cef_string_t* name,
		struct _cef_v8value_t* object,
		size_t argumentsCount,
		struct _cef_v8value_t* const* arguments,
		struct _cef_v8value_t** retval,
		cef_string_t* exception);

	class v8handler__
	: public cef_v8handler_t
	{
	private:
		cef_frame_t *_Frame;
		cef_frame_t *_F( void )
		{
			if ( _Frame == NULL )
				qRGnr();

			return _Frame;
		}
	public:
		void reset( bso::bool__ = true )
		{
			misc::Set( this );
			_Frame = NULL;
		}
		E_CDTOR( v8handler__ );
		void Init(
			cef_frame_t *Frame,
			execute__ &Execute )
		{
			misc::Set( this );
			_Frame = Frame;
			cef_v8handler_t::execute = Execute;
		}
		cef_frame_t *Frame( void )
		{
			return _F();
		}
	};

	struct native_function__
	{
		v8handler__ Handler;
		cef_v8value_t *Function;
		void reset( bso::bool__ P = true )
		{
			Handler.reset( P );
			Function = NULL;
		}
		E_CDTOR( native_function__ );
		void Init(
			cef_string_t *Name,
			cef_frame_t *Frame,
			execute__ &Execute )
		{
			Handler.Init( Frame, Execute );
			Function = cef_v8value_create_function( Name, &Handler );
		}
	};

	class render_process_handler__
	: public cef_render_process_handler_t
	{
	private:
		bso::bool__ _FirstLoadingMade;
	public:
		void reset( bso::bool__ P = true )
		{
			misc::Set<cef_render_process_handler_t>( this );
			_FirstLoadingMade = false;
		}
		E_CDTOR( render_process_handler__ );
		void Init( void )
		{
			misc::Set<cef_render_process_handler_t>( this );
			_FirstLoadingMade = false;
		}
		E_RWDISCLOSE__( bso::bool__, FirstLoadingMade );
	};

	static class rack___ {
		public:
			render_process_handler__ RenderProcessHandler;
			native_function__ LaunchEvent;
			agent::agent___ Agent;
			void reset( bso::bool__ P = true )
			{
				RenderProcessHandler.reset( P );
				Agent.reset( P );
				LaunchEvent.reset( P );
			}
			E_CDTOR( rack___ );
			// The members are intiialized when required.
	} Rack_;
}

static void Merge_(
	const str::strings_ &Strings,
	str::string_ &Result )
{
qRH
	xdhcmn::digest Args;
qRB
	Args.Init();
	Args.AppendMulti( Strings );

	xdhcmn::Merge( Args, Result );
qRR
qRT
qRE
}

static void HandleSelectedFiles_(
	cef_list_value_t *ListValue,
	cef_frame_t *Frame )
{
qRH
	str::string Id, Action, Result;
	str::strings FileNames;
	TOL_CBUFFER___ IdBuffer, ActionBuffer, RegistryBuffer;
qRB
	Id.Init();
	Action.Init();
	FileNames.Init();

	files::HandleResult( ListValue, Id, Action, FileNames );

	Result.Init();
	Merge_( FileNames, Result );

	misc::SetAttribute( Id, sclmisc::MGetValue( registry::custom_item::attribute_name::Result, RegistryBuffer ), Result, Frame );
	Rack_.Agent.Launch( Id.Convert( IdBuffer ), Action.Convert( ActionBuffer ) );
qRR
qRT
qRE
}

static TOL_CBUFFER___ IdentificationBuffer_;
/*
static void HandleAction_( cef_list_value_t *ListValue )
{
qRH
	str::string Action;
	TOL_CBUFFER___ Buffer;
qRB
	Action.Init();

	misc::GetString( ListValue, Action );
	Rack_.Agent.Launch( NULL, Action.Convert( Buffer ) );
qRR
qRT
qRE
}
*/
static void ReportError_(
	cef_frame_t *Frame,
	const char *Message )
{
qRH
	str::string ErrorScript;
//	err::buffer__ Buffer;
qRB
//	cio::COut << err::Message( Buffer ) << txf::nl << txf::commit;

	ErrorScript.Init( "alert('");
	ErrorScript.Append( Message );
	ErrorScript.Append("');");
	misc::ExecuteJavascript( ErrorScript, Frame );
qRR
qRT
qRE
}

static void HandleClosing_(
	cef_list_value_t *ListValue,
	cef_browser_t *Browser )
{
qRH
	xdhcmn::digest Args;	// For an eventual future use, as 'ListValue' should be empty...
	str::string Response;
qRB
	Args.Init();
	misc::Convert( ListValue, Args );

#if 0
	Rack_.Agent.Launch( NULL, xdhcbk::CloseActionLabel );
#else
	if ( Rack_.Agent.Launch( NULL, xdhcmn::CloseActionLabel ) ) {
		Rack_.Agent.reset();
		Response.Init( "true" );
	} else
		Response.Init();
#endif

	misc::SendMessage( misc::cmClosed, Browser, Response );
qRR
qRT
qRE
}

static int CEF_CALLBACK RenderOnProcessMessageReceived_(
    struct _cef_render_process_handler_t* self,
    struct _cef_browser_t* browser,
	cef_process_id_t source_process,
    struct _cef_process_message_t* message)
{
	bso::bool__ Handled = true;
qRH
	cef_string_userfree_t Message = NULL;
	str::string Param;
	str::string Identification;
	cef_frame_t *Frame = NULL;
	cef_list_value_t *ListValue = NULL;
	err::buffer__ ErrBuffer;
qRB
	Frame = misc::GetFrame( browser );
	ListValue = message->get_argument_list( message );
	Message = message->get_name( message );

	if ( Message == NULL )
		qRGnr();

	switch ( misc::GetRenderMessage( Message ) ) {
	case misc::rmStart:
		Param.Init();
		misc::GetString( ListValue, Param );

		Identification.Init( MISC_NAME_LC " V" MISC_VERSION " Build " __DATE__ " " __TIME__ " - " );	
		Identification.Append( cpe::GetDescription() );

		if ( !Rack_.Agent.Init( browser, Param, Identification.Convert( IdentificationBuffer_ ) ) )
			 qRGnr();
		break;
	case misc::rmHandleSelectedFiles:
		HandleSelectedFiles_( ListValue, Frame );
		break;
/*	case misc::rmHandleAction:
		HandleAction_( ListValue );
		break; */
	case misc::rmClose:
		HandleClosing_( ListValue, browser );
		break;
	default:
		qRGnr();
		break;
	}
qRR
	if ( ERRType != err::t_Abort ) {
		ReportError_( Frame, err::Message( ErrBuffer ) );
		ERRRst();
	}
qRT
	if ( ListValue != NULL )
		misc::Release( ListValue );

	if ( Message != NULL )
		cef_string_userfree_free( Message );

	misc::Release( self );
	misc::Release( message );

	if ( Frame != NULL )
		misc::Release( Frame );

	if ( browser != NULL )
		misc::Release( browser );
qRE
	return Handled;
}

static void CEF_CALLBACK OnBrowserDestroyed_(
	struct _cef_render_process_handler_t* self,
	struct _cef_browser_t* browser )
{
	Rack_.Agent.reset();
}

namespace {
	bso::bool__ IsSet_(
		const char *Name,
		_cef_v8value_t *Event )
	{
		bso::bool__ IsSet = false;
	qRH
		misc::cef_string___ Key;
		cef_v8value_t *Value;
	qRB
		Key.Init( Name );
		Value = Event->get_value_bykey( Event, Key );

		if ( !Value->is_bool( Value ) )
			qRGnr();

		IsSet = Value->get_bool_value( Value ) != 0;
	qRR
	qRT
	qRE
		return IsSet;
	}

	inline void HandleKeyModifier_(
		const char *ModifierKeyName,
		char ModifierShortcut,
		_cef_v8value_t *Event,
		str::string_ &Modifiers )
	{
		if ( IsSet_( ModifierKeyName, Event ) )
			Modifiers.Append( ModifierShortcut );
	}

	inline void GetMainKey_(
		_cef_v8value_t *Event,
		str::string_ &Keys )
	{
	qRH
		misc::cef_string___ Key;
		cef_v8value_t *Value;
	qRB
		Key.Init( "keyCode" );
		Value = Event->get_value_bykey( Event, Key );

		if ( Value == NULL )
			qRFwk();

		if ( !Value->is_valid( Value ) )
			qRGnr();

		if ( Value->is_null( Value ) )
			qRGnr();

		if ( Value->is_undefined( Value ) )
			qRGnr();

		if ( !Value->is_int( Value ) )
			qRGnr();

		int32 I = Value->get_int_value( Value );

		if ( isprint( I ) )
			I = tolower( I );
		else
			I += 'a' - 1;

		Keys.Append( I );
	qRR
	qRT
	qRE
	}

	void GetKeys_(
		_cef_v8value_t *Event,
		str::string_ &Shortcut )
	{
		HandleKeyModifier_("metaKey", 'M', Event, Shortcut );
		HandleKeyModifier_("shiftKey", 'S', Event, Shortcut );
		HandleKeyModifier_("ctrlKey", 'C', Event, Shortcut );
		HandleKeyModifier_("altKey", 'A', Event, Shortcut );

		if ( Shortcut.Amount() != 0 )
			Shortcut.Append( '+' );

		GetMainKey_( Event, Shortcut );
	}
}
	
// This is the callback for the native 'launchEvent' JS function.
static int CEF_CALLBACK LaunchEvent_(
	struct _cef_v8handler_t* self,
	const cef_string_t* name,
	struct _cef_v8value_t* object,
	size_t argumentsCount,
	struct _cef_v8value_t* const* arguments,
	struct _cef_v8value_t** retval,
	cef_string_t* exception )
{
	bso::bool__ Success = false;
qRH
	cef_v8value_t *RawDigest = NULL;
//	cef_v8context_t *Context = NULL;
//	cef_v8value_t *Global = NULL;
	misc::nstring___ Digest;
	cef_nstring_t CEFBuffer;
	str::string Buffer;
	err::buffer__ ERRBuffer;
qRB
	misc::Set(&CEFBuffer);

	RawDigest = arguments[0];
	if ( !RawDigest->is_string( RawDigest ) )
		qRGnr();

	cef_convert_from( RawDigest->get_string_value( RawDigest ), &CEFBuffer );
	Digest.Init( CEFBuffer.str );
	misc::Clear( &CEFBuffer );

	*retval = cef_v8value_create_bool( 1 );

	Buffer.Init();
	if ( !Rack_.Agent.HandleEvent( Digest.UTF8( Buffer ) ) )
		qRLmt();	// The browser-based version always call 'stopPropagation()'.

	Success = true;
qRR
	misc::Report( err::Message( ERRBuffer ), NULL );
	ERRRst();
qRT
	misc::Clear( &CEFBuffer );
qRE
	return Success;
}

// This is the callback for the native 'unlock' JS function.
static int CEF_CALLBACK Unlock_(
	struct _cef_v8handler_t* self,
	const cef_string_t* name,
	struct _cef_v8value_t* object,
	size_t argumentsCount,
	struct _cef_v8value_t* const* arguments,
	struct _cef_v8value_t** retval,
	cef_string_t* exception )
{
qRH
	misc::nstring___ Response;
	cef_v8value_t *RawResponse = NULL;
	str::string Buffer;
	cef_nstring_t CEFBuffer;
qRB
	misc::Set( &CEFBuffer );

	Response.Init();
	switch ( argumentsCount ) {
	case 0:
		Response.Init();
		break;
	case 1:
		RawResponse = arguments[0];
		if ( !RawResponse->is_string( RawResponse ) )
			qRGnr();

		cef_convert_from( RawResponse->get_string_value( RawResponse ), &CEFBuffer );
		Response.Init( CEFBuffer.str );
		break;
	default:
		qRGnr();
		break;
	}

	Buffer.Init();
	misc::SendResponse( Response.UTF8( Buffer ) );
qRR
qRT
	misc::Clear( &CEFBuffer );
qRE
	return true;
}

static void CEF_CALLBACK OnContextCreated_(
	struct _cef_render_process_handler_t* self,
	struct _cef_browser_t* browser,
	struct _cef_frame_t* frame,
	struct _cef_v8context_t* context )
{
qRH
	cef_v8value_t *Global = NULL;
	misc::cef_string___ LaunchEventFunctionName;
qRB
	if ( frame->is_main(frame) ) {
		Global = context->get_global( context );

		LaunchEventFunctionName.Init( "launchEvent" );
		Rack_.LaunchEvent.Init( LaunchEventFunctionName, frame, LaunchEvent_ );
		Global->set_value_bykey( Global, LaunchEventFunctionName, Rack_.LaunchEvent.Function, V8_PROPERTY_ATTRIBUTE_NONE );
	}
qRR
qRT
qRE
}

static void LaunchURLOutside_( cef_request_t *Request )
{
qRH
	cef_string_userfree_t URL = NULL;
	cef_nstring_t CEFBuffer;
qRB
	URL = Request->get_url( Request );
	cef_convert_from( URL, &CEFBuffer );

	tol::Launch( CEFBuffer.str );
qRR
qRT
	if ( URL != NULL )
		cef_string_userfree_free( URL );

misc::Clear( &CEFBuffer );
qRE
}

static int CEF_CALLBACK OnBeforeNavigation_(
	struct _cef_render_process_handler_t* self,
	struct _cef_browser_t* browser, struct _cef_frame_t* frame,
	struct _cef_request_t* request, cef_navigation_type_t navigation_type,
		int is_redirect )
{
	// Purpose is to open URL outside of this browser.
	if ( frame->is_main( frame ) ) {
		render_process_handler__ &RenderProcessHandler = *(render_process_handler__ *)self;

		if ( RenderProcessHandler.FirstLoadingMade() ) {
			LaunchURLOutside_( request );

			return 1;	// Stop navigation.
		} else {
			RenderProcessHandler.SetFirstLoadingMade( true );

			return 0;	// Stop navigation.
		}
	} else
		return 0;	// Stop navigation.
}

static void SetRenderProcessHandler_( void )
{
	Rack_.RenderProcessHandler.Init();
	Rack_.RenderProcessHandler.on_before_navigation = OnBeforeNavigation_;
	Rack_.RenderProcessHandler.on_context_created = OnContextCreated_;
	Rack_.RenderProcessHandler.on_process_message_received = RenderOnProcessMessageReceived_;
	Rack_.RenderProcessHandler.on_browser_destroyed = OnBrowserDestroyed_;
}

cef_render_process_handler_t* CEF_CALLBACK render::GetRenderProcessHandler( struct _cef_app_t* self )
{
//	misc::Release( self );

	return &Rack_.RenderProcessHandler;
}

Q37_GCTOR( render )
{
	SetRenderProcessHandler_();
}
