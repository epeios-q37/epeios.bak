/*
  'fhd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Requires the 'fhd' header file ('fhd.h').
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: fhd.cpp,v 1.4 2012/11/14 16:06:27 csimon Exp $

#define FHD__COMPILATION

#include "fhd.h"

class fhdtutor
: public ttr_tutor
{
public:
	fhdtutor( void )
	: ttr_tutor( FHD_NAME )
	{
#ifdef FHD_DBG
		Version = FHD_VERSION " (DBG)";
#else
		Version = FHD_VERSION;
#endif
		Owner = FHD_OWNER;
		Date = "$Date: 2012/11/14 16:06:27 $";
	}
	virtual ~fhdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fhd;

void fhd::oflow_driver___::ForceWriting_(
	const data__ *Buffer,
	amount__ Amount )
{
	amount__ AmountWritten = PutUpTo( Buffer, Amount );

	while( AmountWritten < Amount )
		AmountWritten += PutUpTo( Buffer + AmountWritten, Amount - AmountWritten );
}

void fhd::oflow_driver___::ForceDumpingOfCache_( bool Synchronisation )
{
	while( !DumpCache_( Synchronisation ) );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fhdpersonnalization
: public fhdtutor
{
public:
	fhdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fhdpersonnalization( void )
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

static fhdpersonnalization Tutor;

ttr_tutor &FHDTutor = Tutor;
