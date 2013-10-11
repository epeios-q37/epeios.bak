/*
	'cio' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'cio' header file ('cio.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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



//	$Id: cio.cpp,v 1.48 2013/04/06 14:50:43 csimon Exp $

#define CIO__COMPILATION

#include "cio.h"

class ciotutor
: public ttr_tutor
{
public:
	ciotutor( void )
	: ttr_tutor( CIO_NAME )
	{
#ifdef CIO_DBG
		Version = CIO_VERSION "\b\bD $";
#else
		Version = CIO_VERSION;
#endif
		Owner = CIO_OWNER;
		Date = "$Date: 2013/04/06 14:50:43 $";
	}
	virtual ~ciotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"

using namespace cio;

static target__ Target_ = t_Undefined;

static flx::void_oflow_driver___ _VOutDriver;
static flx::void_oflow_driver___ _VErrDriver;
static flx::void_iflow_driver___ _VInDriver;

static iof::io_oflow_driver___ _COutDriver;
static iof::io_oflow_driver___ _CErrDriver;
static iof::io_iflow_driver___ _CInDriver;

#if defined( CPE_WIN )
#	include <io.h>
#	include <fcntl.h>
#endif

#ifdef IOP__USE_LOWLEVEL_IO
iop::descriptor__ cio::CInDescriptor = 0, cio::COutDescriptor = 1, cio::CErrDescriptor = 2;
#elif defined( IOP__USE_STANDARD_IO )
iop::descriptor__ cio::cind = stdin, cio::coutd = stdout, cio::cerrd = stderr;
#else
#	error "Unkonw I/O enviroment !"
#endif

flx::relay_oflow_driver___ cio::COutDriver;
flx::relay_oflow_driver___ cio::CErrDriver;
flx::relay_iflow_driver___ cio::CInDriver;

cout___ cio::COut;
cerr___ cio::CErr;
cin___ cio::CIn;

void cio::Initialize( target__ Target )
{
	switch ( Target ) {
	case tConsole:
#if defined( CPE_WIN )
		if ( _setmode( _fileno( stdin ), _O_BINARY ) == -1 )
			ERRLbr();

		if ( _setmode( _fileno( stdout ), _O_BINARY ) == -1 )
			ERRLbr();

		if ( _setmode( _fileno( stderr ), _O_BINARY ) == -1 )
			ERRLbr();
#endif
		_COutDriver.Init( COutDescriptor, fdr::ts_Default );
		_CInDriver.Init( CInDescriptor, fdr::ts_Default );
		_CErrDriver.Init( CErrDescriptor, fdr::ts_Default );

		COutDriver.Init( _COutDriver, fdr::ts_Default );
		CInDriver.Init( _CInDriver, fdr::ts_Default );
		CErrDriver.Init( _CErrDriver, fdr::ts_Default );

		break;
	case tVoid:
		COutDriver.Init( _VOutDriver, fdr::ts_Default );
		CInDriver.Init( _VInDriver, fdr::ts_Default );
		CErrDriver.Init( _VErrDriver, fdr::ts_Default );
		break;
	case tUser:
		if ( !COutDriver.IsInitialized() )
			ERRFwk();

		if ( !CInDriver.IsInitialized() )
			ERRFwk();

		if ( !CErrDriver.IsInitialized() )
			ERRFwk();

		break;
	default:
		ERRPrm();
		break;
	}

	cio::COut.Init();
	cio::CErr.Init();
	cio::CIn.Init();

	::Target_ = Target;
}

target__ cio::Target( void )
{
	return ::Target_;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ciopersonnalization
: public ciotutor
{
public:
	ciopersonnalization( void )
	{
#ifdef CIO__AUTOMATIC_INITIALIZATION
		cio::Initialize( t_Default );
#endif
	/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ciopersonnalization( void )
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

static ciopersonnalization Tutor;

ttr_tutor &CIOTutor = Tutor;
