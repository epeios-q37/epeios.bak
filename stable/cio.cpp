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

iof::io_oflow_driver___ cio::SOutDriver;
iof::io_oflow_driver___ cio::SErrDriver;
iof::io_iflow_driver___ cio::SInDriver;

cof___ cio::COutF, cio::CErrF;
cif__ cio::CInF;

txf::text_oflow__ cio::COut, cio::CErr;
txf::text_iflow__ cio::CIn;

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
		SOutDriver.Init( COutDescriptor, fdr::ts_Default );
		SInDriver.Init( CInDescriptor, fdr::ts_Default );
		SErrDriver.Init( CErrDescriptor, fdr::ts_Default );

		COutF.Init( SOutDriver );
		CInF.Init( SInDriver );
		CErrF.Init( SErrDriver );

		break;
	case tVoid:
		COutF.Init( _VOutDriver );
		CInF.Init( _VInDriver );
		CErrF.Init( _VErrDriver );
		break;
	case tUser:
		if ( !COutF.IsInitialized() )
			ERRFwk();

		if ( !CInF.IsInitialized() )
			ERRFwk();

		if ( !CErrF.IsInitialized() )
			ERRFwk();

		break;
	default:
		ERRPrm();
		break;
	}

	cio::COut.Init( COutF );
	cio::CErr.Init( CErrF );
	cio::CIn.Init( CInF );

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
