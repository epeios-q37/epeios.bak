/*
	'xulfdg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfdg' header file ('xulfdg.h').
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: xulfdg.cpp,v 1.22 2013/06/11 10:32:26 csimon Exp $

#define XULFDG__COMPILATION

#include "xulfdg.h"

class xulfdgtutor
: public ttr_tutor
{
public:
	xulfdgtutor( void )
	: ttr_tutor( XULFDG_NAME )
	{
#ifdef XULFDG_DBG
		Version = XULFDG_VERSION "\b\bD $";
#else
		Version = XULFDG_VERSION;
#endif
		Owner = XULFDG_OWNER;
		Date = "$Date: 2013/06/11 10:32:26 $";
	}
	virtual ~xulfdgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

static void Register_(
	trunk___ &Trunk,
	xulwdg::event_handler__<xulftk::trunk___> &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::AttachEventHandler( Trunk.UI().DebugDialog().Document(), Id, EventHandler );
}

#define I( name ) Register_( Trunk, EventHandlers.name, #name );	

static void Register_(
	trunk___ &Trunk,
	debug_dialog__::event_handlers__ &EventHandlers )
{
	I( ehJSConsole );
	I( ehDOMInspector );
	I( ehFrontendError );
	I( ehBackendError );
}

void xulfdg::debug_dialog__::Register( nsIDOMWindow *Window )
{
ERRProlog
	str::string Id;
	str::string Translation;
ERRBegin
	xulfdg::window__::Register( Window );	// 'xulfdg::' ne devrait pas être nécessaire, mais sans, 'VC++ 12' n'est pas content...

	Id.Init();

	if ( nsxpcm::GetId( nsxpcm::GetElement( Window ), Id ) != XULFDG_WINDOW_ID ) {
		Translation.Init();
		Trunk().UI().Alert( Trunk().Kernel().GetTranslation( XULFDG_NAME "_IncompatibleDebugDialog", Translation ) );
		nsxpcm::Close( Window );
		ERRReturn;
	}

	Register_( Trunk(), Trunk().UI().DebugDialog().EventHandlers );

	Trunk().UI().DebugDialog().Refresh();
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfdgpersonnalization
: public xulfdgtutor
{
public:
	xulfdgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfdgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static xulfdgpersonnalization Tutor;

ttr_tutor &XULFDGTutor = Tutor;
