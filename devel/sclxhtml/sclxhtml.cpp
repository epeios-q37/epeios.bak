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

# include "xhtfmn.h"
# include "xhtfsf.h"


using namespace sclxhtml;

static bso::bool__ IsInitialized_ = false;

const sclrgstry::registry_ &sclxhtml::GetRegistry( void )
{
	return sclrgstry::GetCommonRegistry();
}


#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XHTCLLBK_RETRIEVE_FUNCTION_NAME, xhtcllbk::retrieve );

// Bien que définit dans un '.cpp', et propre à ce '.cpp', VC++ se mélange les pinceaux avec le 'callback__' définit dans 'scllocale.cpp', d'où le 'namespace'.
namespace {
	typedef xhtcllbk::downstream_callback__ _callback__;

	class callback__
	: public _callback__
	{
	private:
		session_core___ *_Session;
	protected:
		session_core___ &_S( void )
		{
			if ( _Session == NULL )
				ERRFwk();

			return *_Session;
		}
		virtual void XHTCLLBKStart( void ) override
		{
			_S().Start();
		}
		virtual bso::bool__ XHTCLLBKOnClose( void ) override
		{
			return _S().OnClose();
		}
		virtual const char *XHTCLLBKLanguage( void ) override
		{
			return sclmisc::GetLanguage();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Session != NULL )
					delete _Session;

			_callback__::reset( P );
			_Session = NULL;
		}
		E_CVDTOR( callback__ );
		void Init( session_core___ &Session )
		{
			_Session = &Session;
			_callback__::Init( Agent().EventManager() );
		}
		xhtagent::agent_core___ &Agent( void )
		{
			return _S().AgentCore();
		}
	};
}

xhtcllbk::downstream_callback__ *XHTCLLBKRetrieve( const xhtcllbk::shared_data__ &Data )
{
	callback__ *Callback = NULL;
ERRProlog
	session_core___ *Session = NULL;
ERRBegin

	SCLXHTMLOnLoading( Data.LauncherIdentification() );

	Callback = new callback__;

	if ( Callback == NULL )
		ERRAlc();

	Session = sclxhtml::SCLXHTMLNewSession( Data.Token(), Data.Callback() );

	if ( Session == NULL )
		ERRFwk();

	Callback->Init( *Session  );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;

	if ( Session !=  NULL )
		delete Session;
ERREnd
ERREpilog
	return Callback;
}

void sclxhtml::MainSubmission( xhtagent::agent_core___ &Agent )
{
ERRProlog
	str::string ProjectFeature;
ERRBegin
	ProjectFeature.Init();
	sclfrntnd::LoadProject( xhtfmn::GetProjectFeatures( Agent, ProjectFeature ), ProjectFeature );
ERRErr
ERREnd
ERREpilog
}

void sclxhtml::SessionFormSubmission(
	frdkrn::kernel___ &Kernel,
	frdssn::session___ &Session,
	xhtagent::agent_core___ &Agent,
	const frdkrn::compatibility_informations__ &CompatibilityInformations )
{
ERRProlog
	str::string BackendFeature;
	TOL_CBUFFER___ Buffer;
ERRBegin
	BackendFeature.Init();
	sclfrntnd::Connect( Kernel, xhtfsf::GetBackendFeatures( Agent, BackendFeature ), BackendFeature, CompatibilityInformations );

	Session.Open( Agent.GetLanguage( Buffer ) );
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
		if ( !::IsInitialized_ )
		{
			cio::Initialize( cio::tVoid );
			sclmisc::Initialize( (const char *)NULL );

			::IsInitialized_ = true;
		}
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
