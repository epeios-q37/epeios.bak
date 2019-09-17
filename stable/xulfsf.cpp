/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define XULFSF_COMPILATION_

#include "xulfsf.h"


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
	bsmBasic,		// Slection dans une liste seulement.
	bsmAdvanced,	// + possibilit de sasir adresse:port pour un 'daemon'.
	bsmExpert,		// + possibilit de slctionn une bibliothque dynamique.
	bsm_amount,
	bsm_Undefined
};

static backend_selection_mode__ GetBackendSelectionMode_( const rgstry::multi_level_registry_ &Registry )
{
	backend_selection_mode__ Mode = bsmBasic;	// Mode par dfaut.
qRH
	str::string RawMode;
qRB
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
qRR
qRT
qRE
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
qRH
	str::string Login, Password;
	bso::bool__ Disable = false;
qRB
	Login.Init();
	Password.Init();

	switch ( frdkrn::GetAuthenticationPromptMode( Registry ) ) {
	case frdkrn::apmNone:
	case frdkrn::apmAuto:
		Disable = true;
		break;
	case frdkrn::apmFull:
		if ( !Registry.GetValue( frdrgy::AuthenticationPassword, Password ) )
			qRFwk();
	case frdkrn::apmPartial:
		if ( !Registry.GetValue( frdrgy::AuthenticationLogin, Login ) )
			qRFwk();
	case frdkrn::apmEmpty:
		break;
	case frdkrn::apm_Undefined:
		qRLmt();
		// Affichage d'un message d'erreur normalement.
		break;
	default:
		qRFwk();
		break;
	}

	Widgets.txbLogin.SetValue( Login );
	Widgets.txbPassword.SetValue( Password );

	Digest.PushTag( "Authentication" );
	Digest.PutAttribute( "Available", Disable ? "no" : "yes" );
	Digest.PopTag();
qRR
qRT
qRE
}

static void GetPredefinedBackends_(
	const frdkrn::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &PredefinedBackends )
{
qRH
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
qRB
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
qRR
qRT
qRE
}

static void FillPredefinedBackendsWidget_( trunk___ &Trunk )
{
qRH
	str::string PredefinedBackends;
	nsIDOMDocumentFragment *Fragment;
	fnm::name___ FileName;
	str::string Buffer;
qRB
	PredefinedBackends.Init();

	GetPredefinedBackends_( Trunk.Kernel().Registry(), Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedBackends );

	Trunk.UI().LogQuietly( PredefinedBackends );

	nsxpcm::RemoveChildren( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );

	FileName.Init();
	Buffer.Init();
	Fragment = nsxpcm::XSLTransformByFileName( PredefinedBackends, fnm::BuildFileName( Trunk.DefaultXSLRootPath(), "PredefinedBackendMenuList", ".xsl", FileName ).UTF8( Buffer ), Trunk.UI().SessionForm().Window(), nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend, Fragment );

	nsxpcm::SetSelectedItem( Trunk.UI().SessionForm().Widgets.mnlPredefinedBackend );
qRR
qRT
qRE
}

void xulfsf::_session_form_core__::_Refresh( xml::writer_ &Digest )
{
qRH
	str::string Translation;
qRB
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
		qRFwk();
		break;
	}

	Widgets.dckBackendType.SetSelectedIndex( _BackendExtendedType );

qRR
qRT
qRE
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
	Widgets.Init();

	Trunk().UI().EventHandlers.SF.Attach( Window );
}
