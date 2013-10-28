/*
	'sclbackend.cpp' by Claude SIMON (http://zeusw.org/).

	'sclbackend' is part of the Epeios framework.

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

#define SCLBACKEND__COMPILATION

#include "sclbackend.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"
#include "scldaemon.h"

#include  "lcl.h"
#include "cio.h"

#include "csdleo.h"
#include "csdles.h"

#include "fblbur.h"

#include "fnm.h"

using namespace sclbacknd;

typedef csdscb::callback__ _callback__;

const char *scldaemon::TargetName = sclbacknd::TargetName;

class callback__
: public _callback__
{
private:
	fblbur::mode__ _Mode;
	const lcl::locale_ *_Locale;
protected:
	virtual void *CSDSCBPreProcess( const char *Origin )
	{
		return New( _Mode, *_Locale, Origin );
	}

	virtual csdscb::action__ CSDSCBProcess(
		flw::ioflow__ &Flow,
		void *UP )
	{
		_data__ &Data = *(_data__ *)UP;

		if ( Data.Backend.Handle( Flow, NULL, Data.RequestLogFunctions ) )
			return csdscb::aContinue;
		else
			return csdscb::aStop;
	}
	virtual void CSDSCBPostProcess( void *UP )
	{
		delete (_data__ *)UP;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_callback__::reset( P );

		_Mode = fblbur::m_Undefined;
		_Locale = NULL;
	}
	E_CVDTOR( callback__ );
	void Init(
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale )
	{
		_callback__::Init();

		_Mode = Mode;
		_Locale = &Locale;
	}
};

csdleo::callback__ *scldaemon::RetrieveSteering(
	csdleo::mode__ CSDMode,
	const lcl::locale_ &Locale )
{
	callback__ *Callback = NULL;
ERRProlog
	fblbur::mode__ FBLMode = fblbur::m_Undefined;
ERRBegin
	switch ( CSDMode ) {
	case csdleo::mEmbedded:
		FBLMode = fblbur::mEmbedded;
		break;
	case csdleo::mRemote:
		FBLMode = fblbur::mRemote;
		break;
	default:
		ERRFwk();
		break;
	}

	if ( ( Callback = new callback__ ) == NULL )
		ERRAlc();

	Callback->Init( FBLMode, Locale );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;

	ERRRst();	// Error catched to avoid that it goes further.
				// Error reported by the fact that the returned value is 'NULL'.
ERREnd
ERREpilog
	return Callback;
}

void scldaemon::ReleaseSteering( csdleo::callback__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclbackendpersonnalization
{
public:
	sclbackendpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclbackendpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static sclbackendpersonnalization Tutor;
