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

#define SCLXHTML__COMPILATION

#include "sclxhtml.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "sclmisc.h"

#include "frdkrn.h"

# include "xhtprolog.h"
# include "xhtlogin.h"

using namespace sclxhtml;

static bso::bool__ IsInitialized_ = false;

static const char *Launcher_ = NULL;

const sclrgstry::registry_ &sclxhtml::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}

const char *sclxhtml::GetLauncher( void )
{
	if ( Launcher_ == NULL )
		qRFwk();

	return Launcher_;
}

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scllocale.cpp', d'o le 'namespace'.
namespace {

	typedef xdhcbk::callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		xdhcbk::proxy_callback__ *_ProxyCallback;
		Q37_RMDF( xdhcbk::proxy_callback__, _PC, _ProxyCallback );
	protected:
		virtual void XDHCBKInitialize( const xdhcbk::shared_data__ &Data ) override
		{
			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.qRRor(), Data.SCLError(), Data.CIO(), Data.Localization() );
			}

			_ProxyCallback = Data.Callback();
		}
		virtual void XDHCBKBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				qRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcbk::session_callback__ *XDHCBKNew( void ) override
		{
			return SCLXHTMLNew( _PC() );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_callback__::reset( P );
		}
		E_CVDTOR( callback__ );
		void Init( void )
		{
			_callback__::Init();
		}
	};
}

static inline void DoNothing_( void )
{}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XDHCBK_RETRIEVE_FUNCTION_NAME, xdhcbk::retrieve );

_callback__ *XDHCBKRetrieve( void )
{
	callback__ *Callback = NULL;
qRFH
qRFB
	Callback = new callback__;

	if ( Callback == NULL )
		qRAlc();

	Callback->Init();
qRFR
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
qRFT
qRFE(DoNothing_())
	return Callback;
}

void sclxhtml::HandleError(
	xhtagent::agent___ &Agent,
	const char *Language )
{
qRH
	str::string Message;
	err::buffer__ ErrBuffer;
qRB
	switch ( ERRType ) {
	case err::t_Abort:
		Message.Init();
		if ( sclerror::GetPendingErrorTranslation( Language, Message, err::hUserDefined ) ) {
			sclerror::ResetPendingError();
			Agent.RawAlert( Message );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		Agent.RawAlert( "???" );
		break;
	default:
		Agent.RawAlert( err::Message( ErrBuffer ) );
		break;
	}

	ERRRst();
qRR
qRT
qRE
}


#if 0
bso::bool__ sclxhtml::session___::XDHCBKXDHCBKLaunch(
	const char *Id,
	const char *Action )
{
	bso::bool__ Success = false;
qRH
	str::string Message;
	err::buffer__ ErrBuffer;
qRB
		if ( _OnBeforeAction( Id, Action ) )
			if ( !strcmp( Action, xdhcbk::CloseActionLabel ) )
				Success = _OnClose();	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
			else
				Success = _Handler.Launch( Id, Action );
qRR
#if 0
	switch ( ERRType ) {
	case err::t_Abort:
		Message.Init();
		if ( sclerror::GetPendingErrorTranslation( _L(), Message, err::hUserDefined ) ) {
			sclerror::ResetPendingError();
			_A().RawAlert( Message );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		_A().RawAlert( "???" );
		break;
	default:
		_A().RawAlert( err::Message( ErrBuffer ) );
		break;
	}

	ERRRst();
# endif
qRT
qRE
	return Success;
}
#endif

void sclxhtml::LoadProject( xhtagent::agent___ &Agent )
{
qRH
	str::string ProjectFeature;
qRB
	ProjectFeature.Init();
	sclfrntnd::LoadProject( xhtprolog::GetProjectFeatures( Agent, ProjectFeature ), ProjectFeature );
qRR
qRT
qRE
}

void sclxhtml::LaunchProject(
	const char *Language,
	frdkrn::kernel___ &Kernel,
	frdssn::session___ &Session,
	xhtagent::agent___ &Agent,
	const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
qRH
	str::string BackendFeature;
qRB
	BackendFeature.Init();
	sclfrntnd::Connect( Language, Kernel, xhtlogin::GetBackendFeatures( Agent, BackendFeature ), BackendFeature, CompatibilityInformations );

	Session.Open( Language );
qRR
qRT
qRE
}

