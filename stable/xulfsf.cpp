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
	bsmBasic,		// Sélection dans une liste seulement.
	bsmAdvanced,	// + possibilité de sasir adresse:port pour un 'daemon'.
	bsmExpert,		// + possibilité de sélctionné une bibliothèque dynamique.
	bsm_amount,
	bsm_Undefined
};

static backend_selection_mode__ GetBackendSelectionMode_( const rgstry::multi_level_registry_ &Registry )
{
	backend_selection_mode__ Mode = bsmBasic;	// Mode par défaut.
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
	xml::writer_ &Digest )
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

	Digest.PushTag( "EmbeddedBackend" );
	Digest.PutAttribute( "Available", Embedded ? "yes" : "no" );
	Digest.PopTag();

	Digest.PushTag( "DaemonBackend" );
	Digest.PutAttribute( "Available", Daemon ? "yes" : "no" );
	Digest.PopTag();

	Digest.PushTag( "PredefinedBackend" );
	Digest.PutAttribute( "Available", "yes"  );		// A minima, toujours disponible.
	Digest.PopTag();

	Digest.PushTag( "NoBackend" );
	Digest.PutAttribute( "Available", "yes" );		// A minima, toujours disponible.
	Digest.PopTag();

	return Success;

}

void HandleAuthenticationSubForm_(
	const frdkrn::registry_ &Registry,
	xml::writer_ &Digest,
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

	Digest.PushTag( "Authentication" );
	Digest.PutAttribute( "Available", Disable ? "no" : "yes" );
	Digest.PopTag();
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
	fnm::name___ FileName;
	str::string Buffer;
ERRBegin
	PredefinedBackends.Init();

	GetPredefinedBackends_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedBackends );

	Trunk.UI().LogQuietly( PredefinedBackends );

	nsxpcm::RemoveChildren( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );

	FileName.Init();
	Buffer.Init();
	Fragment = nsxpcm::XSLTransformByFileName( PredefinedBackends, fnm::BuildFileName( Trunk.DefaultXSLRootPath(), "PredefinedBackendMenuList", ".xsl", FileName ).UTF8( Buffer ), Trunk.UI().SessionForm().Document(), nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend, Fragment );

	nsxpcm::SetSelectedItem( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );
ERRErr
ERREnd
ERREpilog
}

void xulfsf::_session_form_core__::_Refresh( xml::writer_ &Digest )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	if ( !HideUnusedBackendSelectionMode_( Trunk().Kernel().Registry(), Digest ) )
		Trunk().UI().LogAndPrompt( Trunk().Kernel().GetTranslation( XULFSF_NAME "_BadValueForBackendSelectionMode", Translation ) );

	HandleAuthenticationSubForm_( Trunk().Kernel().Registry(), Digest, Widgets );

	FillPredefinedBackendsWidget_( Trunk() );

	switch ( _BackendExtendedType ) {
	case frdkrn::bxtNone:
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniNoBackend );
		break;
	case frdkrn::bxtPredefined:
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniPredefinedBackend );
		break;
	case frdkrn::bxtDaemon:
		Widgets.mnlBackendType.SetSelectedItem( Widgets.mniDaemonBackend );
		break;
	case frdkrn::bxtEmbedded:
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

#define A( name ) name.Attach( nsxpcm::supports__( Window, #name ) );

void xulfsf::widgets__::Attach( nsIDOMWindow *Window )
{
	A( mnlBackendType );
	A( mniNoBackend );
	A( mniPredefinedBackend );
	A( mniDaemonBackend );
	A( mniEmbeddedBackend );
	A( dckBackendType );
	A( mnlPredefinedBackend );
	A( txbDaemonBackend );
	A( txbEmbeddedBackend );
	A( txbLogin );
	A( txbPassword );
}

void xulfsf::_session_form_core__::Attach( nsIDOMWindow *Window )
{
	Widgets.Attach( Window );
	Trunk().UI().EventHandlers.SF.Attach( Window );
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
