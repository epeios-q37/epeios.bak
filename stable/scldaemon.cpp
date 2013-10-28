/*
	'scldaemon.cpp' by Claude SIMON (http://zeusw.org/).

	'scldaemon' is part of the Epeios framework.

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

#define SCLDAEMON__COMPILATION

#include "scldaemon.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace scldaemon;

#include "sclmisc.h"
#include "scllocale.h"
#include "sclerror.h"

#include "lcl.h"
#include "cio.h"

#include "csdleo.h"
#include "csdles.h"

#include "fnm.h"

static bso::bool__ IsInitialized_ = false;

const char *scldaemon::GetLanguage( void )
{
	return "en";	// A modifier.
}


csdleo::callback__ *csdles::CSDLESRetrieveSteering( csdleo::data__ *Data )
{
	csdleo::callback__ *Steering = NULL;
ERRProlog
	FNM_BUFFER___ Buffer;
ERRBegin
	if ( Data == NULL )
		ERRPrm();

	if ( !IsInitialized_ && !Data->DryRun )	{
		cio::COutDriver.Init( *Data->COut, fdr::ts_Default );
		cio::CErrDriver.Init( *Data->CErr, fdr::ts_Default );
		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );

		// Do not work when placed in 'global_cdtor'.
		if ( Data->UP != NULL )
			fnm::GetLocation( (const char *)Data->UP, Buffer );
		sclmisc::Initialize( TargetName, Buffer );
		IsInitialized_ = true;
	}

/*
	switch ( Data->Mode ) {
	case csdleo::mEmbedded:
		Mode = fblbur::mEmbedded;
		break;
	case csdleo::mRemote:
		Mode = fblbur::mRemote;
		break;
	default:
		ERRFwk();
		break;
	}
*/

	Steering = RetrieveSteering( Data->Mode, scllocale::GetLocale() );
ERRErr
	if ( cio::IsInitialized() )
		if ( sclerror::ReportPendingError( GetLanguage(), err::hUserDefined  )) {
			cio::CErr << txf::nl;
			ERRRst();
		}
ERREnd
	if ( cio::IsInitialized() ) {
		cio::COut << txf::commit;
		cio::CErr << txf::commit;
	}
ERREpilog
	return Steering;
}

void csdles::CSDLESReleaseSteering( csdleo::callback__ *Steering )
{
	ReleaseSteering( Steering );
}

void scldaemon::DisplayModuleClosingMessage( void )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	cio::COut << scllocale::GetTranslation( SCLDAEMON_NAME "_ModuleClosing", GetLanguage(), Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}


void scldaemon::DisplayModuleClosedMessage( void )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	cio::COut << scllocale::GetTranslation( SCLDAEMON_NAME "_ModuleClosed", GetLanguage(), Translation ) << txf::nl;
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scldaemonpersonnalization
{
public:
	scldaemonpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scldaemonpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static scldaemonpersonnalization Tutor;
