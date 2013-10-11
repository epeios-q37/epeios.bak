/*
	'sdr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sdr' header file ('sdr.h').
	Copyright (C) 2013-2004 Claude SIMON.

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



//	$Id: sdr.cpp,v 1.10 2013/04/15 10:50:54 csimon Exp $

#define SDR__COMPILATION

#include "sdr.h"

class sdrtutor
: public ttr_tutor
{
public:
	sdrtutor( void )
	: ttr_tutor( SDR_NAME )
	{
#ifdef SDR_DBG
		Version = SDR_VERSION "\b\bD $";
#else
		Version = SDR_VERSION;
#endif
		Owner = SDR_OWNER;
		Date = "$Date: 2013/04/15 10:50:54 $";
	}
	virtual ~sdrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace sdr;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sdrpersonnalization
: public sdrtutor
{
public:
	sdrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sdrpersonnalization( void )
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

static sdrpersonnalization Tutor;

ttr_tutor &SDRTutor = Tutor;
