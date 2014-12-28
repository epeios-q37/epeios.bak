/*
	'esketchbkd.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sclbacknd.h"
#include "sclmisc.h"

#include "registry.h"
#include "sktfbc.h"
#include "sktinf.h"

#include "fdr.h"

#include "fblbkd.h"

#include "wrpunbound.h"
#include "wrpexample.h"

#define BACKEND_NAME	SKTINF_LC_AFFIX	"bkd"
#define COPYRIGHT		COPYRIGHT_YEARS " " SKTINF_OWNER_NAME " (" SKTINF_OWNER_CONTACT ")"	
#define API_VERSION		"1"

typedef sclbacknd::callback__ _callback___;

typedef sclbacknd::backend___	_backend___;

class backend___
: public _backend___
{
private:
	FBLBKD_RAM_MODULE( wrpexample::myobject_, common::stuff___ ) MyObject;
	common::stuff___ _Stuff;
public:
	void reset( bso::bool__ P = true )
	{
		_backend___::reset( P );
		_Stuff.reset( P );
		MyObject.reset( P );
	}
	E_CVDTOR( backend___ );
	void Init(
		fblbur::mode__ Mode,
		const char *ClientOrigin )
	{
		_Stuff.Init( *this );

		_backend___::Init(
			Mode,
			API_VERSION,
			ClientOrigin,
			SKTINF_LC_AFFIX,
			BACKEND_NAME " " VERSION,
			COPYRIGHT,
			SKTINF_SOFTWARE_NAME " V" SKTINF_SOFTWARE_VERSION,
			&_Stuff );

		wrpunbound::Inform( *this, _Stuff );

		MyObject.Init( _Stuff );
		Add( MyObject );
	}
};

// Dans cette classe, on place ce qui est commun à tous les 'backend's.
class callback___
: public _callback___
{
protected:
	virtual _backend___ *SCLBACKNDNew( 
		fblbur::mode__ Mode,
		const char *Origin )
	{
		backend___ *Backend = NULL;
	ERRProlog
	ERRBegin
		if ( ( Backend = new backend___ ) == NULL )
			ERRAlc();

		Backend->Init( Mode, Origin );
	ERRErr
		if ( Backend != NULL )
			delete Backend;

		Backend = NULL;
	ERREnd
	ERREpilog
		return Backend;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_callback___::reset( P );

		if ( P == true ) {
			scldaemon::DisplayModuleClosingMessage();
			scldaemon::DisplayModuleClosedMessage();
		}
	}
	E_CVDTOR( callback___ )
	void Init( fblbur::mode__ Mode )
	{
		_callback___::Init( Mode );
	}
};

_callback___ *sclbacknd::SCLBACKNDNewCallback( fblbur::mode__ Mode )
{
	callback___ *Callback = NULL;
ERRProlog
ERRBegin
	if ( ( Callback = new callback___ ) == NULL )
		ERRAlc();

	Callback->Init( Mode );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
ERREnd
ERREpilog
	return Callback;
}

const char *sclmisc::SCLMISCTargetName = BACKEND_NAME;
