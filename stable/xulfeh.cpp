/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define XULFEH__COMPILATION

#include "xulfeh.h"


#include "xulftk.h"

using namespace xulfeh;

using nsxpcm::event__;

#if 0
void xulfeh::window_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement tre ferme, ce sera fait par la fonctions prcdente; inutile de poursuivre la proddure de fermeture.
}
#endif

void xulfeh::m::new_project__::NSXPCMOnEvent( event__ )
{
	Trunk().DefineSession( str::string() );
}

void xulfeh::m::user_project__::NSXPCMOnEvent( event__ )
{
qRH
	str::string Translation, FileName;
qRB
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( XULFEH_NAME "_SelectProjectFile", Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().DefineSession( FileName );
qRR
qRT
qRE
}

void xulfeh::m::user_project_selection__::NSXPCMOnEvent( event__ )
{
qRH
	str::string Translation, FileName;
qRB
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( XULFEH_NAME "_SelectProjectFile", Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().Main().Widgets.txbUserProjectLocation.SetValue( FileName );
qRR
qRT
qRE
}

void xulfeh::m::apply__::NSXPCMOnEvent( event__ )
{
qRH
	str::string ProjectTypeLabel;
	str::string PredefinedProjectId;
	str::string ProjectLocation;
	str::string Log;
qRB
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
		qRFwk();
		break;
	}

	Log.Init( "ProjectLocation : \"" );
	Log.Append( ProjectLocation );
	Log.Append( '"' );

	Trunk().UI().LogQuietly( Log );

	Trunk().DefineSession( ProjectLocation );
qRR
qRT
qRE
}

void xulfeh::m::close_project__::NSXPCMOnEvent( event__ )
{
	Trunk().DropSession();
}

void xulfeh::m::project_type_selection__::NSXPCMOnEvent( event__ )
{
qRH
	str::string Value;
qRB
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
		qRFwk();
		break;
	}
qRR
qRT
qRE
}


void xulfeh::m::exit__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement tre ferme, ce sera fait par la fonctions prcdente; inutile de poursuivre la proddure de fermeture.
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
qRH
	str::string Value;
	frdkrn::backend_extended_type__ Type = frdkrn::bxt_Undefined;
qRB
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
		qRFwk();
		break;
	}

	Trunk().RefreshUI();
qRR
qRT
qRE
}

void xulfeh::sf::embedded_backend_selection__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
qRH
	str::string FileName;
	str::string Translation;
qRB
	FileName.Init();
	Translation.Init();

	if ( nsxpcm::DynamicLibraryFileOpenDialogBox( Trunk().UI().SessionForm().Window(), str::string( Trunk().Kernel().GetTranslation( XULFDG_NAME "_EmbeddedBackendFileSelectionDialogBoxTitle", Translation ) ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().SessionForm().Widgets.txbEmbeddedBackend.SetValue( FileName );
qRR
qRT
qRE
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
	nsxpcm::AttachEventHandler( Element, "eh" #name, name );

void xulfeh::event_handlers__::m__::Attach( nsIDOMWindow *Window )
{
	nsIDOMElement *Element = nsxpcm::GetElement( Window );

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
	nsIDOMElement *Element = nsxpcm::GetElement( Window );

	A( Apply );
	A( BackendTypeSelection );
	A( Cancel );
	A( EmbeddedBackendSelection );
}

