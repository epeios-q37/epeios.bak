/*
	'sclbacknd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclbacknd' header file ('sclbacknd.h').
	Copyright (C) 20112004 Claude SIMON.

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



//	$Id: sclbacknd.cpp,v 1.9 2013/01/02 17:32:10 csimon Exp $

#define SCLBACKND__COMPILATION

#include "sclbacknd.h"

class sclbackndtutor
: public ttr_tutor
{
public:
	sclbackndtutor( void )
	: ttr_tutor( SCLBACKND_NAME )
	{
#ifdef SCLBACKND_DBG
		Version = SCLBACKND_VERSION "\b\bD $";
#else
		Version = SCLBACKND_VERSION;
#endif
		Owner = SCLBACKND_OWNER;
		Date = "$Date: 2013/01/02 17:32:10 $";
	}
	virtual ~sclbackndtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

typedef csdsuf::user_functions__ _user_functions__;

const char *scldaemon::TargetName = sclbacknd::TargetName;

class user_functions__
: public _user_functions__
{
private:
	fblbur::mode__ _Mode;
	const lcl::locale_ *_Locale;
protected:
	virtual void *CSDSUFPreProcess( const char *Origin )
	{
		return New( _Mode, *_Locale, Origin );
	}

	virtual csdsuf::action__ CSDSUFProcess(
		flw::ioflow__ &Flow,
		void *UP )
	{
		_data__ &Data = *(_data__ *)UP;

		if ( Data.Backend.Handle( Flow, NULL, Data.RequestLogFunctions ) )
			return csdsuf::aContinue;
		else
			return csdsuf::aStop;
	}
	virtual void CSDSUFPostProcess( void *UP )
	{
		delete (_data__ *)UP;
	}
	virtual void CSDSUFExit( void )
	{
		scldaemon::DisplayModuleClosingMessage();
		cio::COut << txf::commit;

		// Do here what neede to exit properly the module.

		scldaemon::DisplayModuleClosedMessage();
		cio::COut << txf::commit;
	}
public:
	void reset( bso::bool__ P = true )
	{
		_user_functions__::reset( P );

		_Mode = fblbur::m_Undefined;
		_Locale = NULL;
	}
	E_CVDTOR( user_functions__ );
	void Init(
		fblbur::mode__ Mode,
		const lcl::locale_ &Locale )
	{
		_user_functions__::Init();

		_Mode = Mode;
		_Locale = &Locale;
	}
};

csdleo::user_functions__ *scldaemon::RetrieveSteering(
	csdleo::mode__ CSDMode,
	const lcl::locale_ &Locale )
{
	user_functions__ *Functions = NULL;
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

	if ( ( Functions = new user_functions__ ) == NULL )
		ERRAlc();

	Functions->Init( FBLMode, Locale );
ERRErr
	if ( Functions != NULL )
		delete Functions;

	Functions = NULL;

	ERRRst();	// Error catched to avoid that it goes further.
				// Error reported by the fact that the returned value is 'NULL'.
ERREnd
ERREpilog
	return Functions;
}

void scldaemon::ReleaseSteering( csdleo::user_functions__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclbackndpersonnalization
: public sclbackndtutor
{
public:
	sclbackndpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclbackndpersonnalization( void )
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

static sclbackndpersonnalization Tutor;

ttr_tutor &SCLBACKNDTutor = Tutor;
