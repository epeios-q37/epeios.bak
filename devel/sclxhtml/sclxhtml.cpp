/*
	'sclxhtml.cpp' by Claude SIMON (http://zeusw.org/).

	'sclxhtml' is part of the Epeios framework.

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

	typedef xhtcllbk::callback__ _callback__;

	class callback__
	: public _callback__
	{
	protected:
		virtual void XHTCLLBKInitialize( const xhtcllbk::shared_data__ &Data ) override
		{

			if ( Launcher_ == NULL ) {
				Launcher_ = Data.LauncherIdentification();
				sclmisc::Initialize( Data.ERRError(), Data.SCLError(), (const char *)NULL );
			}
		}
		virtual void XHTCLLBKBaseLanguage( TOL_CBUFFER___ &Buffer ) override
		{
			const char *Language = sclmisc::GetBaseLanguage();

			if ( Language == NULL )
				ERRFwk();

			Buffer.Malloc(strlen( Language) + 1 );

			strcpy( Buffer, Language );
		}
		virtual xhtcllbk::session_callback__ *XHTCLLBKNew( xhtcllbk::proxy_callback__ &Callback ) override
		{
			return SCLXHTMLNew( Callback );
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

DEF( XHTCLLBK_RETRIEVE_FUNCTION_NAME, xhtcllbk::retrieve );

_callback__ *XHTCLLBKRetrieve( void )
{
	callback__ *Callback = NULL;
ERRFProlog
ERRFBegin
	Callback = new callback__;

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

void sclxhtml::HandleError(
	xhtagent::agent___ &Agent,
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
ERRErr
ERREnd
ERREpilog
}


#if 0
bso::bool__ sclxhtml::session___::XHTCLLBKLaunch(
	const char *Id,
	const char *Action )
{
	bso::bool__ Success = false;
ERRProlog
	str::string Message;
	err::buffer__ ErrBuffer;
ERRBegin
		if ( _OnBeforeAction( Id, Action ) )
			if ( !strcmp( Action, xhtcllbk::CloseActionLabel ) )
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

void sclxhtml::LoadProject( xhtagent::agent___ &Agent )
{
ERRProlog
	str::string ProjectFeature;
ERRBegin
	ProjectFeature.Init();
	sclfrntnd::LoadProject( xhtprolog::GetProjectFeatures( Agent, ProjectFeature ), ProjectFeature );
ERRErr
ERREnd
ERREpilog
}

void sclxhtml::LaunchProject(
	const char *Language,
	frdkrn::kernel___ &Kernel,
	frdssn::session___ &Session,
	xhtagent::agent___ &Agent,
	const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
ERRProlog
	str::string BackendFeature;
ERRBegin
	BackendFeature.Init();
	sclfrntnd::Connect( Language, Kernel, xhtlogin::GetBackendFeatures( Agent, BackendFeature ), BackendFeature, CompatibilityInformations );

	Session.Open( Language );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclxhtmlpersonnalization
{
public:
	sclxhtmlpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclxhtmlpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclxhtmlpersonnalization Tutor;
