/*
	'scldaemon' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scldaemon' header file ('scldaemon.h').
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: scldaemon.cpp,v 1.4 2013/04/09 17:57:26 csimon Exp $

#define SCLDAEMON__COMPILATION

#include "scldaemon.h"

class scldaemontutor
: public ttr_tutor
{
public:
	scldaemontutor( void )
	: ttr_tutor( SCLDAEMON_NAME )
	{
#ifdef SCLDAEMON_DBG
		Version = SCLDAEMON_VERSION "\b\bD $";
#else
		Version = SCLDAEMON_VERSION;
#endif
		Owner = SCLDAEMON_OWNER;
		Date = "$Date: 2013/04/09 17:57:26 $";
	}
	virtual ~scldaemontutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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


csdleo::user_functions__ *csdles::CSDLESRetrieveSteering( csdleo::data__ *Data )
{
	csdleo::user_functions__ *Steering = NULL;
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

void csdles::CSDLESReleaseSteering( csdleo::user_functions__ *Steering )
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
: public scldaemontutor
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


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scldaemonpersonnalization Tutor;

ttr_tutor &SCLDAEMONTutor = Tutor;
