/*
	'xulfeh' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfeh' header file ('xulfeh.h').
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



//	$Id: xxx.cpp,v 1.9 2012/11/14 16:06:23 csimon Exp $

#define XULFEH__COMPILATION

#include "xulfeh.h"

class xulfehtutor
: public ttr_tutor
{
public:
	xulfehtutor( void )
	: ttr_tutor( XULFEH_NAME )
	{
#ifdef XULFEH_DBG
		Version = XULFEH_VERSION "\b\bD $";
#else
		Version = XULFEH_VERSION;
#endif
		Owner = XULFEH_OWNER;
		Date = "$Date: 2012/11/14 16:06:23 $";
	}
	virtual ~xulfehtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

using namespace xulfeh;

using nsxpcm::event__;

#if 0
void xulfeh::window_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}
#endif

void xulfeh::m::new_project__::NSXPCMOnEvent( event__ )
{
	Trunk().DefineSession( str::string() );
}

void xulfeh::m::user_project__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation, FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( XULFEH_NAME "_SelectProjectFile", Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().DefineSession( FileName );
ERRErr
ERREnd
ERREpilog
}

void xulfeh::m::user_project_selection__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation, FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( XULFEH_NAME "_SelectProjectFile", Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().Main().Widgets.txbUserProjectLocation.SetValue( FileName );
ERRErr
ERREnd
ERREpilog
}

void xulfeh::m::apply__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string ProjectTypeLabel;
	str::string PredefinedProjectId;
	str::string ProjectLocation;
	str::string Log;
ERRBegin
	ProjectLocation.Init();

	ProjectTypeLabel.Init();

	switch ( frdkrn::GetProjectType( Trunk().UI().Main().Widgets.mnlProjectType.GetValue( ProjectTypeLabel ) ) ) {
	case frdkrn::ptNew:
		break;
	case frdkrn::ptPredefined:
		PredefinedProjectId.Init();
		Trunk().UI().Main().Widgets.mnlPredefinedProjectList.GetValue( PredefinedProjectId );

		frdkrn::GetPredefinedProjectLocation( PredefinedProjectId, Trunk().Kernel().Registry(), ProjectLocation );
		break;
	case frdkrn::ptUser:
		Trunk().UI().Main().Widgets.txbUserProjectLocation.GetValue( ProjectLocation );
		break;
	default:
		ERRFwk();
		break;
	}

	Log.Init( "ProjectLocation : \"" );
	Log.Append( ProjectLocation );
	Log.Append( '"' );

	Trunk().UI().LogQuietly( Log );

	Trunk().DefineSession( ProjectLocation );
ERRErr
ERREnd
ERREpilog
}

void xulfeh::m::close_project__::NSXPCMOnEvent( event__ )
{
	Trunk().DropSession();
}

void xulfeh::m::project_type_selection__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( EventData().GetTarget(), "value", Value );

	switch ( frdkrn::GetProjectType( Value ) ) {
	case frdkrn::ptNew:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 0 );
		break;
	case frdkrn::ptPredefined:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 1 );
		break;
	case frdkrn::ptUser:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 2 );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
}


void xulfeh::m::exit__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}

void xulfeh::m::about__::NSXPCMOnEvent( event__ )
{
	Trunk().About();
}

void xulfeh::m::web_site__::NSXPCMOnEvent( event__ )
{
	Trunk().BrowseWEBSite();
}

void xulfeh::m::debug__::NSXPCMOnEvent( event__ )
{
	Trunk().UI().CreateDebugDialog();
	Trunk().ExposeSteering();
	nsxpcm::OpenDialog( Trunk().UI().Main().Window(), "debug.xul", "_blank" );
	Trunk().UI().DeleteDebugDialog();
}

void xulfeh::sf::backend_type_selection__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( EventData().GetTarget(), "value", Value );

	switch ( Type = frdkrn::GetBackendExtendedType( Value ) ) {
	case frdkrn::bxtNone:
	case frdkrn::bxtPredefined:
	case frdkrn::bxtDaemon:
	case frdkrn::bxtEmbedded:
		Trunk().UI().SessionForm().SetBackendType( Type );
		break;
	default:
		ERRFwk();
		break;
	}

	Trunk().RefreshUI();
ERRErr
ERREnd
ERREpilog
}

void xulfeh::sf::embedded_backend_selection__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
ERRProlog
	str::string FileName;
	str::string Translation;
ERRBegin
	FileName.Init();
	Translation.Init();

	if ( nsxpcm::DynamicLibraryFileOpenDialogBox( Trunk().UI().SessionForm().Window(), str::string( Trunk().Kernel().GetTranslation( XULFDG_NAME "_EmbeddedBackendFileSelectionDialogBoxTitle", Translation ) ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( FileName );
ERRErr
ERREnd
ERREpilog
}

void xulfeh::sf::apply__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().ApplySession();
}

void xulfeh::sf::cancel__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().CancelSession();
}

#define A( name )\
	nsxpcm::AttachEventHandler( Document, "eh" #name, name );

void xulfeh::event_handlers__::m__::Attach( nsIDOMWindow *Window )
{
	nsIDOMNode *Document = nsxpcm::GetDocument( Window );

	A( About );
	A( Apply );
	A( CloseProject );
	A( Debug );
	A( Exit );
	A( NewProject );
	A( ProjectTypeSelection );
	A( UserProject );
	A( UserProjectSelection );
	A( WebSite );
}

void xulfeh::event_handlers__::sf__::Attach( nsIDOMWindow *Window )
{
	nsIDOMNode *Document = nsxpcm::GetDocument( Window );

	A( Apply );
	A( BackendTypeSelection );
	A( Cancel );
	A( EmbeddedBackendSelection );
}






/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfehpersonnalization
: public xulfehtutor
{
public:
	xulfehpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfehpersonnalization( void )
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

static xulfehpersonnalization Tutor;

ttr_tutor &XULFEHTutor = Tutor;
