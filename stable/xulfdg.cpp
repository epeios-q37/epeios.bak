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

#define XULFDG__COMPILATION

#include "xulfdg.h"

#include "xulftk.h"

using namespace xulfdg;
using nsxpcm::event__;

using namespace xulftk;

#define DIGEST_TARGET	XULFDG_AFFIX

const char *xulfdg::debug_dialog__::XULFBSRefresh( xml::writer_ &Digest )
{
	xulfbs::_PushDigestTag( Digest );

	Digest.PushTag( "IsConnected" );
	Digest.PutValue( Trunk().Kernel().IsConnected() ? "true" : "false" );
	Digest.PopTag();

	Digest.PopTag();

	return DIGEST_TARGET;
}

void xulfdg::jsconsole_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().UI().GetJSConsole();
	Trunk().UI().DebugDialog().Close();
}

void xulfdg::dominspector_eh__::NSXPCMOnEvent( event__ )
{
	nsxpcm::GetDOMInspector( Trunk().UI().Main().Window() );
	Trunk().UI().DebugDialog().Close();
}

void xulfdg::frontend_error_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
ERRBegin
	Trunk().UI().DebugDialog().Close();
	ERRFree();
ERRErr
ERREnd
ERREpilog
}

void xulfdg::backend_error_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	xulftk::trunk___ *Trunk = NULL;
ERRBegin
	Trunk = &this->Trunk();	// Trunk() est perdu lors du 'Close()' qui suit.

	Trunk->UI().DebugDialog().Close();
	Trunk->Kernel().ThrowERRFwk();
ERRErr
ERREnd
ERREpilog
}
/* UI Registrations */

static void Attach_(
	trunk___ &Trunk,
	xulwdg::event_handler__<xulftk::trunk___> &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	
}

#define A( name )\
	nsxpcm::AttachEventHandler( Element, "eh" #name, name );

void xulfdg::event_handlers__::Attach( nsIDOMWindow *Window )
{
	nsIDOMElement *Element = nsxpcm::GetElement( Window );

	A( JSConsole );
	A( DOMInspector );
	A( FrontendError );
	A( BackendError );
}

void xulfdg::debug_dialog__::Attach( nsIDOMWindow *Window )
{
ERRProlog
	str::string Id;
	str::string Translation;
ERRBegin
	xulfdg::window__::Attach( Window );	// 'xulfdg::' ne devrait pas tre ncessaire, mais le mauvais 'Attach()' est appel par VC++12 si absent...

	Id.Init();

	if ( nsxpcm::GetId( nsxpcm::GetElement( Window ), Id ) != XULFDG_WINDOW_ID ) {
		Translation.Init();
		Trunk().UI().Alert( Trunk().Kernel().GetTranslation( XULFDG_NAME "_IncompatibleDebugDialog", Translation ) );
		nsxpcm::Close( Window );
		ERRReturn;
	}

	EventHandlers.Attach( Window );

	Refresh();
ERRErr
ERREnd
ERREpilog
}
