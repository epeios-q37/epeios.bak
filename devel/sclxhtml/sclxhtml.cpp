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

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

void sclxhtml::callback__::Start( void )
{
	SCLXHTMLStart( A() );
}

void sclxhtml::Load(
	const rgstry::tentry__ &FileName,
	str::string_ &String )
{
	sclmisc::Load( FileName, sclrgstry::GetRegistry(), String );
}

/*
void sclxhtml::LoadAndTranslateTags(
	const rgstry::entry___ &FileName,
	const char *Language,
	str::string_ &String )
{
	sclmisc::LoadAndTranslateTags( FileName, sclrgstry::GetRegistry(), Language, String, '$' );
}
*/

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( XHTCLLBK_RETRIEVE_FUNCTION_NAME, xhtcllbk::retrieve );

typedef xhtcllbk::downstream_callback__ _dcallback__;

class dcallback___
: public _dcallback__
{
private:
	xhtagent::agent___ _Agent;
	callback__ *_Callback;
protected:
	virtual void XHTCLLBKStart( void )
	{
		if ( _Callback == NULL )
			ERRFwk();

		_Callback->Start();
	}
	virtual const char *XHTCLLBKLanguage( void )
	{
		return sclmisc::GetLanguage();
	}
public:
	void reset( bso::bool__ P = true )
	{
		if ( P )
			if ( _Callback != NULL )
				delete _Callback;

		_dcallback__::reset( P );
		_Agent.reset( P );
		_Callback = NULL;
	}
	E_CVDTOR( dcallback___ );
	void Init(
		xhtcllbk::upstream_callback__ &UCallback,
		callback__ &Callback )
	{
		_Agent.Init( UCallback );
		_dcallback__::Init( _Agent.Handlers() );
		_Callback = &Callback;
	}
	xhtagent::agent___ &Agent( void )
	{
		return _Agent;
	}
};

xhtcllbk::downstream_callback__ *XHTCLLBKRetrieve( const xhtcllbk::shared_data__ &Data )
{
	dcallback___ *DCallback = NULL;
ERRProlog
	callback__ *Callback;
ERRBegin
	DCallback = new dcallback___;

	if ( DCallback == NULL )
		ERRAlc();

	Callback = sclxhtml::SCLXHTMLRetrieveCallback( DCallback->Agent() );

	if ( Callback == NULL )
		ERRFwk();

	DCallback->Init( Data.Callback(), *Callback  );
ERRErr
	if ( DCallback != NULL )
		delete DCallback;

	DCallback = NULL;

	if ( Callback !=  NULL )
		delete Callback;
ERREnd
ERREpilog
	return DCallback;
}

static void LoadProject_(
	xhtagent::agent___ &Agent,
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
	xhtagent::agent___ &Agent,
	const str::string_ &Id )
{
ERRProlog
	str::string ProjectFileName;
ERRBegin
	if ( Id.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLXHTML_NAME "_EmptyPredefinedProjectId" );

	ProjectFileName.Init();

	sclfrntnd::GetProjectFileName( Id, ProjectFileName );

	if ( ProjectFileName.Amount() == 0 )
		sclmisc::ReportAndAbort( SCLXHTML_NAME "_NoOrBadProjectFileNameInPredefinedProject", Id );

	LoadProject_( Agent, ProjectFileName );
ERRErr
ERREnd
ERREpilog
}

void sclxhtml::MainSubmission(
	xhtagent::agent___ &Agent,
	xml::writer_ &Writer )
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
		LoadPredefinedProject_( Agent, ProjectFeature );
		break;
	case xhtfbs::ptUser:
		break;
	case xhtfbs::pt_Undefined:
		sclmisc::ReportAndAbort( SCLXHTML_NAME "_NoProjectFileSelected" );
		break;
	default:
		ERRFwk();
		break;
	}

	Writer.PushTag( "PredefinedBackends" );

	sclfrntnd::GetPredefinedBackends( Writer );

	Writer.PopTag();
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
