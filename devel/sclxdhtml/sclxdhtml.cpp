/*
	'sclxdhtml.cpp' by Claude SIMON (http://zeusw.org/).

	'sclxdhtml' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define SCLXDHTML__COMPILATION

#include "sclxdhtml.h"

#include "sclfrntnd.h"

using namespace sclxdhtml;

static bso::bool__ IsInitialized_ = false;

static const char *Launcher_ = NULL;

const sclrgstry::registry_ &sclxdhtml::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}

const char *sclxdhtml::GetLauncher( void )
{
	if ( Launcher_ == NULL )
		ERRFwk();

	return Launcher_;
}

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

// Bien que dfinit dans un '.cpp', et propre  ce '.cpp', VC++ se mlange les pinceaux avec le 'callback__' dfinit dans 'scllocale.cpp', d'o le 'namespace'.
namespace {

	typedef xdhcbk::downstream_callback__ _downstream_callback__;
	typedef xdhcbk::upstream_callback__ _upstream_callback__;

	class donwstream_callback_implementation__
	: public _downstream_callback__
	{
	private:
		Q37_MRMDF( xdhcbk::upstream_callback__, _UC, _UpstreamCallback );
	protected:
		virtual void XDHCBKInitialize( const xdhcbk::shared_data__ &Data ) override
		{
			if ( Launcher_ != NULL )
				ERRFwk();

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.ERRError(), Data.SCLError(), Data.CIO(), Data.Localization() );
			}

			_UpstreamCallback = Data.Callback();
		}
		virtual void XDHCBKBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				ERRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xdhcbk::session_callback__ *XDHCBKNew( void ) override
		{
			return SCLXDHTMLNew( _UC() );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			downstream_callback__::reset( P );
		}
		E_CVDTOR( donwstream_callback_implementation__ );
		void Init( void )
		{
			downstream_callback__::Init();
		}
	};
}

static inline void DoNothing_( void )
{}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XDHCBK_RETRIEVE_FUNCTION_NAME, xdhcbk::retrieve );

xdhcbk::downstream_callback__ *XDHCBKRetrieve( void )
{
	donwstream_callback_implementation__ *Callback = NULL;
ERRFProlog
ERRFBegin
	Callback = new donwstream_callback_implementation__;

	if ( Callback == NULL )
		ERRAlc();

	Callback->Init();
ERRFErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
ERRFEnd
ERRFEpilog(DoNothing_())
	return Callback;
}

void sclxdhtml::HandleError(
	proxy__ &Proxy,
	const char *Language )
{
ERRProlog
	str::string Message;
	err::buffer__ ErrBuffer;
ERRBegin
	switch ( ERRType ) {
	case err::t_Abort:
		Message.Init();
		if ( sclerror::GetPendingErrorTranslation( Language, Message, err::hUserDefined ) ) {
			sclerror::ResetPendingError();
			Proxy.Alert( Message );
		} 
		break;
	case err::t_Free:
	case err::t_Return:
		Proxy.Alert( "???" );
		break;
	default:
		Proxy.Alert( err::Message( ErrBuffer ) );
		break;
	}

	ERRRst();
ERRErr
ERREnd
ERREpilog
}


#if 0
bso::bool__ sclxhtml::session___::XDHCBKXDHCBKLaunch(
	const char *Id,
	const char *Action )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Message;
	err::buffer__ ErrBuffer;
ERRBegin
		if ( _OnBeforeAction( Id, Action ) )
			if ( !strcmp( Action, xdhcbk::CloseActionLabel ) )
				Success = _OnClose();	// Dans ce cas, si 'Success' est  'false', la fermeture de l'application est suspendue.
			else
				Success = _Handler.Launch( Id, Action );
ERRErr
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
ERREnd
ERREpilog
	return Success;
}
#endif

void sclxdhtml::LoadProject( proxy__ &Proxy )
{
ERRProlog
	str::string ProjectFeature;
ERRBegin
	ProjectFeature.Init();
	sclfrntnd::LoadProject( xdhdws::prolog::GetProjectFeatures( Proxy, ProjectFeature ), ProjectFeature );
ERRErr
ERREnd
ERREpilog
}

void sclxdhtml::LaunchProject(
	const char *Language,
	frdkrn::kernel___ &Kernel,
	frdssn::session___ &Session,
	proxy__ &Proxy,
	const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
ERRProlog
	str::string BackendFeature;
ERRBegin
	BackendFeature.Init();
	sclfrntnd::Connect( Language, Kernel, xdhdws::login::GetBackendFeatures( Proxy, BackendFeature ), BackendFeature, CompatibilityInformations );

	Session.Open( Language );
ERRErr
ERREnd
ERREpilog
}

