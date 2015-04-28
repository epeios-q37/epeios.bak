/*
	'sclwebkit.cpp' by Claude SIMON (http://zeusw.org/).

	'sclwebkit' is part of the Epeios framework.

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

#define SCLWEBKIT__COMPILATION

#include "sclwebkit.h"

#include "flf.h"
#include "sclmisc.h"
#include "wkagent.h"

using namespace sclwebkit;

#ifdef CPE_WIN
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

void sclwebkit::callback__::Start( void )
{
ERRProlog
	str::string XML, XSL;
ERRBegin
	XML.Init();
	XSL.Init();

	SCLWEBKITStart( A(), XML, XSL );

	A().SetChildren(	"html", XML, XSL );
ERRErr
ERREnd
ERREpilog
}

void sclwebkit::Load(
	const rgstry::entry___ &FileName,
	str::string_ &String )
{
	sclmisc::Load( FileName, sclrgstry::GetRegistry(), String );
}

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( WKCLLBCK_LAUNCH_FUNCTION_NAME, wkcllbck::launch );

typedef wkcllbck::downstream_callback__ _dcallback__;

class dcallback___
: public _dcallback__
{
private:
	wkagent::agent___ _Agent;
	callback__ *_Callback;
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
		wkcllbck::upstream_callback__ &UCallback,
		callback__ &Callback )
	{
		_Agent.Init( UCallback );
		_dcallback__::Init( _Agent.Actions() );
		_Callback = &Callback;
	}
	wkagent::agent___ &Agent( void )
	{
		return _Agent;
	}
};

wkcllbck::downstream_callback__ *WKCLLBCKLaunch( const wkcllbck::shared_data__ &Data )
{
	dcallback___ *DCallback = NULL;
ERRProlog
	callback__ *Callback;
ERRBegin
	DCallback = new dcallback___;

	if ( DCallback == NULL )
		ERRAlc();

	Callback = sclwebkit::SCLWEBKITRetrieveCallback( DCallback->Agent() );

	if ( Callback == NULL )
		ERRFwk();

	DCallback->Init( Data.Callback(), *Callback  );

	Callback->Start();
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

