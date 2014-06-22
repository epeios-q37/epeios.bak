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

csdleo::callback__ *csdles::CSDLESRetrieveCallback( csdleo::data__ *Data )
{
	csdleo::callback__ *Callback = NULL;
ERRProlog
	fnm::name___ Directory;
	TOL_CBUFFER___ Buffer;
	str::string Error;
ERRBegin
	if ( Data == NULL )
		ERRPrm();
		
	if ( !cio::IsInitialized() ) {
		cio::COutF.Init( *Data->COut );
		cio::CErrF.Init( *Data->CErr );
		cio::CInF.Init( flx::VoidIFlowDriver );

		cio::Initialize( cio::tUser );
	}

	if ( !IsInitialized_ && !Data->DryRun )	{

		// Do not work when placed in 'global_cdtor'.
		Directory.Init();
		if ( Data->UP != NULL )
			fnm::GetLocation( (const char *)Data->UP, Directory );
		sclmisc::Initialize( Directory.UTF8( Buffer ) );
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

	Callback = SCLDAEMONNewCallback( Data->Mode );
ERRErr
	if ( cio::IsInitialized() ) {
		Error.Init();
		if ( sclerror::GetPendingError( sclmisc::GetLanguage(), Error, err::hUserDefined  )) {
			cio::CErr << Error << txf::nl;
			ERRRst();
		}
	}
ERREnd
	if ( cio::IsInitialized() ) {
		cio::COut << txf::commit;
		cio::CErr << txf::commit;
	}
ERREpilog
	return Callback;
}

void csdles::CSDLESReleaseCallback( csdleo::callback__ *Callback )
{
	if ( Callback != NULL  )
		delete Callback;
}

void scldaemon::DisplayModuleClosingMessage( void )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	cio::COut << scllocale::GetTranslation( SCLDAEMON_NAME "_ModuleClosing", sclmisc::GetLanguage(), Translation ) << txf::nl;
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

	cio::COut << scllocale::GetTranslation( SCLDAEMON_NAME "_ModuleClosed", sclmisc::GetLanguage(), Translation ) << txf::nl;
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
