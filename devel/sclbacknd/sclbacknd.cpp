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

scldaemon::callback__ *scldaemon::SCLDAEMONNewCallback(
	csdleo::mode__ CSDMode,
	csdleo::context__ Context )
{
	sclbacknd::callback__ *Callback = NULL;
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

	Callback = SCLBACKNDNewCallback( FBLMode, Context );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;
ERREnd
ERREpilog
	return Callback;
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
