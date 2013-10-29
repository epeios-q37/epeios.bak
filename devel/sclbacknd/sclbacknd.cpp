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

#include "sclbacknd.h"

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

const char *scldaemon::TargetName = sclbacknd::TargetName;

csdleo::callback__ *scldaemon::RetrieveSteering(
	csdleo::mode__ CSDMode,
	const lcl::locale_ &Locale )
{
	sclbacknd::callback__ *Steering = NULL;
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

	Steering = SCLBACKNDNew(  FBLMode, Locale );
ERRErr
	if ( Steering != NULL )
		delete Steering;

	Steering = NULL;

	ERRRst();	// Erreur interceptée pour éviter se propagation.
				// L'erreur est signalée par la valeur retournéee 'NULL'.
ERREnd
ERREpilog
	return Steering;
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
