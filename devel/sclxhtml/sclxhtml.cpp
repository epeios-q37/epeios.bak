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
#include "sclfrntnd.h"

#include "frdkrn.h"

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
		virtual void XHTCLLBKStart( void )
		{
			if ( _Session == NULL )
				ERRFwk();

			_Session->Start();
		}
		virtual const char *XHTCLLBKLanguage( void )
		{
			return sclmisc::GetLanguage();
		}
		session_core___ &_S( void )
		{
			if ( _Session == NULL )
				ERRFwk();

			return *_Session;
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

	SCLXHTMLOnLoading();

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

static void LoadProject_(
	xhtagent::agent_core___ &Agent,
	const str::string_ &FileName )
{
ERRProlog
	str::string Id;
ERRBegin
	Agent.Alert( FileName );

	Id.Init();
	sclmisc::LoadProject( FileName, Id );
ERRErr
ERREnd
ERREpilog
}

static void LoadPredefinedProject_( 
	const sclrgstry::registry_ &Registry,
	xhtagent::agent_core___ &Agent,
	const str::string_ &Id )
{
ERRProlog
	str::string ProjectFileName;
ERRBegin
	if ( Id.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLXHTML_NAME "_EmptyPredefinedProjectId" );

	ProjectFileName.Init();

	sclfrntnd::GetProjectFileName( Registry, Id, ProjectFileName );

	if ( ProjectFileName.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLXHTML_NAME "_NoOrBadProjectFileNameInPredefinedProject", Id );

	LoadProject_( Agent, ProjectFileName );
ERRErr
ERREnd
ERREpilog
}

void sclxhtml::MainSubmission(
	const sclrgstry::registry_ &Registry,
	xhtagent::agent_core___ &Agent )
{
ERRProlog
	str::string ProjectFeature;
ERRBegin
	ProjectFeature.Init();
	switch ( xhtfmn::GetProjectFeatures(Agent, ProjectFeature ) ) {
	case xhtfbs::ptNew:
		sclrgstry::EraseProjectRegistry();
		break;
	case xhtfbs::ptPredefined:
		LoadPredefinedProject_( Registry, Agent, ProjectFeature );
		break;
	case xhtfbs::ptUser:
		if ( ProjectFeature.Amount() == 0  )
			sclmisc::ReportAndAbort( SCLXHTML_NAME "_NoProjectFileSelected" );
		break;
	case xhtfbs::pt_Undefined:
		ERRFwk();
		break;
	default:
		ERRFwk();
		break;
	}
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
