/*
	'xulfsf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfsf' header file ('xulfsf.h').
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



//	$Id: xulfsf.cpp,v 1.26 2013/06/11 10:32:27 csimon Exp $

#define XULFSF__COMPILATION

#include "xulfsf.h"

class xulfsftutor
: public ttr_tutor
{
public:
	xulfsftutor( void )
	: ttr_tutor( XULFSF_NAME )
	{
#ifdef XULFSF_DBG
		Version = XULFSF_VERSION "\b\bD $";
#else
		Version = XULFSF_VERSION;
#endif
		Owner = XULFSF_OWNER;
		Date = "$Date: 2013/06/11 10:32:27 $";
	}
	virtual ~xulfsftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"
#include "xulfrg.h"

#include "cpe.h"
#include "fnm.h"

#include "nsIDOMEventTarget.h"

using namespace xulfsf;

using nsxpcm::event__;

using xulftk::trunk___;

enum backend_selection_mode__
{
	bsmBasic,		// S�lection dans une liste seulement.
	bsmAdvanced,	// + possibilit� de sasir adresse:port pour un 'daemon'.
	bsmExpert,		// + possibilit� de s�lctionn� une biblioth�que dynamique.
	bsm_amount,
	bsm_Undefined
};

static backend_selection_mode__ GetBackendSelectionMode_( const rgstry::multi_level_registry_ &Registry )
{
	backend_selection_mode__ Mode = bsmBasic;	// Mode par d�faut.
ERRProlog
	str::string RawMode;
ERRBegin
	RawMode.Init();
	
	if ( Registry.GetValue( xulfrg::BackendSelectionMode, RawMode ) ) {
		if ( RawMode == "Expert" )
			Mode = bsmExpert;
		else if ( RawMode == "Advanced" )
			Mode = bsmAdvanced;
		else if ( RawMode == "Basic" ) 
			Mode = bsmBasic;
		else
			Mode = bsm_Undefined;
	}
ERRErr
ERREnd
ERREpilog
	return Mode;
}

bso::bool__ HideUnusedBackendSelectionMode_( 
	const rgstry::multi_level_registry_ &Registry,
	broadcasters__ &Broadcasters )
{
	bso::bool__ Success = true;
	bso::bool__ Embedded = false, Daemon = false;

	switch ( GetBackendSelectionMode_( Registry ) ) {
	case bsmExpert:
		Embedded = true;
	case bsmAdvanced:
		Daemon = true;
		break;
	default:
		Success = false;
	case bsmBasic:
		break;
	}

	Broadcasters.bdcEmbeddedBackend.Show( Embedded );
	Broadcasters.bdcDaemonBackend.Show( Daemon );
	Broadcasters.bdcPredefinedBackend.Show();	// A minima, celui-ci est toujours affich�.
	Broadcasters.bdcNoBackend.Show();			// A minima, celui-ci est toujours affich�.

	return Success;

}

void HandleAuthenticationSubForm_(
	const frdkrn::registry_ &Registry,
	broadcasters__ &Broadcasters,
	widgets__ &Widgets )
{
ERRProlog
	str::string Login, Password;
	bso::bool__ Disable = false;
ERRBegin
	Login.Init();
	Password.Init();

	switch ( frdkrn::GetAuthenticationPromptMode( Registry ) ) {
	case frdkrn::apmNone:
	case frdkrn::apmAuto:
		Disable = true;
		break;
	case frdkrn::apmFull:
		if ( !Registry.GetValue( frdrgy::AuthenticationPassword, Password ) )
			ERRDta();
	case frdkrn::apmPartial:
		if ( !Registry.GetValue( frdrgy::AuthenticationLogin, Login ) )
			ERRDta();
	case frdkrn::apmEmpty:
		break;
	case frdkrn::apm_Undefined:
		ERRLmt();
		// Affichage d'un message d'erreur normalement.
		break;
	default:
		ERRFwk();
		break;
	}

	Widgets.txbLogin.SetValue( Login );
	Widgets.txbPassword.SetValue( Password );
	Broadcasters.bdcAuthentication.Disable( Disable );
ERRErr
ERREnd
ERREpilog
}

static void GetPredefinedBackends_(
	const frdkrn::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &PredefinedBackends )
{
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
ERRBegin
	OFlow.Init( PredefinedBackends );
	TFlow.Init( OFlow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	Writer.PushTag( "PredefinedBackends" );

	Default.Init();
	Registry.GetValue( frdrgy::DefaultPredefinedBackend, Default );

	if ( Default.Amount() != 0 )
		Writer.PutAttribute( "Default", Default );

	frdkrn::GetPredefinedBackends( Registry, Locale, Language, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static void FillPredefinedBackendsWidget_( trunk___ &Trunk )
{
ERRProlog
	str::string PredefinedBackends;
	nsIDOMDocumentFragment *Fragment;
	FNM_BUFFER___ FileNameBuffer;
ERRBegin
	PredefinedBackends.Init();

	GetPredefinedBackends_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedBackends );

	Trunk.UI().LogQuietly( PredefinedBackends );

	nsxpcm::RemoveChildren( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );

	Fragment = nsxpcm::XSLTransformByFileName( PredefinedBackends, str::string( fnm::BuildFileName( Trunk.DefaultXSLRootPath(), "PredefinedBackendMenuList", ".xsl", FileNameBuffer ) ), Trunk.UI().SessionForm().Document(), nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend, Fragment );

	nsxpcm::SetSelectedItem( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );
ERRErr
ERREnd
ERREpilog
}

void xulfsf::session_form__::GetDigest( xml::writer_ &Digest )
{
ERRProlog
	str::string Translation;
ERRBegin
	xulftk::trunk___ &Trunk = XULFSFTrunk();

	Translation.Init();

	if ( !HideUnusedBackendSelectionMode_( Trunk.Kernel().Registry(), Broadcasters ) )
		Trunk.UI().LogAndPrompt( Trunk.Kernel().GetTranslation( XULFSF_NAME "_BadValueForBackendSelectionMode", Translation ) );

	HandleAuthenticationSubForm_( Trunk.Kernel().Registry(), Broadcasters, Widgets );

	FillPredefinedBackendsWidget_( Trunk );
#if 0
	if ( _BackendExtendedType == frdkrn::bxt_Undefined )
		ERRReturn;
#endif
	switch ( _BackendExtendedType ) {
	case frdkrn::bxtNone:
		Broadcasters.bdcNoBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniNoBackend );
		break;
	case frdkrn::bxtPredefined:
		Broadcasters.bdcPredefinedBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniPredefinedBackend );
		break;
	case frdkrn::bxtDaemon:
		Broadcasters.bdcDaemonBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniDaemonBackend );
		break;
	case frdkrn::bxtEmbedded:
		Broadcasters.bdcEmbeddedBackend.Show();
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniEmbeddedBackend );
		break;
	default:
		ERRFwk();
		break;
	}

	Widgets.dckBackendType.SetSelectedIndex( _BackendExtendedType );

ERRErr
ERREnd
ERREpilog
}

#if 0
void xulfsf::select_project_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation;
	str::string FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Trunk().Kernel().LocaleRack(), FileName ) ) {
		Trunk().UI().SessionForm().Widgets.ProjectFileNameTextbox.SetValue( FileName );
	}
ERRErr
ERREnd
ERREpilog
}
#endif

static void Register_(
	trunk___ &Trunk,
	broadcaster__ &Broadcaster,
	const char *Id )
{
	Broadcaster.Attach( nsxpcm::supports__( Trunk.UI().SessionForm().Window(), Id ) );
}

#define R( name ) Register_( Trunk, Broadcasters.name, #name );
static void Register_(
	trunk___ &Trunk,
	broadcasters__ &Broadcasters )
{
	R( bdcNoBackend );
	R( bdcPredefinedBackend );
	R( bdcDaemonBackend );
	R( bdcEmbeddedBackend );
	R( bdcEmbeddedBackendSelection );
	R( bdcAuthentication );
}

static void Register_(
	trunk___ &Trunk,
	xulfbs::event_handler__<xulftk::trunk___> &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::AttachEventHandler( Trunk.UI().SessionForm().Document(), Id, EventHandler );
}

#undef R

#define R( name ) Widgets.name.Attach( nsxpcm::supports__( Trunk.UI().SessionForm().Window(), #name ) );

static void Register_(
	trunk___ &Trunk,
	widgets__ &Widgets )
{
	R( mnlBackendType );
	R( mniNoBackend );
	R( mniPredefinedBackend );
	R( mniDaemonBackend );
	R( mniEmbeddedBackend );
	R( dckBackendType );
	R( mnlPredefinedBackend );
	R( txbDaemonBackend );
	R( txbEmbeddedBackend );
	R( txbLogin );
	R( txbPassword );
}

#define A( name )\
	nsxpcm::AttachEventHandler( Document, "eh" #name, EventHandlers.name );

static void Attach_(
	xulfeh::event_handlers__::sf__ &EventHandlers,
	nsIDOMNode *Document )
{
	A( Apply );
	A( BackendTypeSelection );
	A( Cancel );
	A( EmbeddedBackendSelection );
}

void xulfsf::session_form__::Register( nsIDOMWindow *Window )
{
	xulftk::trunk___ &Trunk = XULFSFTrunk();

	_ui_core__::Attach( Window );

	Register_( Trunk, Trunk.UI().SessionForm().Broadcasters );
	Register_( Trunk, Trunk.UI().SessionForm().Widgets );

	Attach_( Trunk.UI().EventHandlers.SF, Trunk.UI().SessionForm().Document() );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfsfpersonnalization
: public xulfsftutor
{
public:
	xulfsfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfsfpersonnalization( void )
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

static xulfsfpersonnalization Tutor;

ttr_tutor &XULFSFTutor = Tutor;
