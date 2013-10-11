/*
	'dtflcf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'dtflcf' header file ('dtflcf.h').
	Copyright (C) 2000, 2003 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: dtflcf.cpp,v 1.6 2012/11/14 16:06:26 csimon Exp $

#define DTFLCF__COMPILATION

#include "dtflcf.h"

class dtflcftutor
: public ttr_tutor
{
public:
	dtflcftutor( void )
	: ttr_tutor( DTFLCF_NAME )
	{
#ifdef DTFLCF_DBG
		Version = DTFLCF_VERSION "\b\bD $";
#else
		Version = DTFLCF_VERSION;
#endif
		Owner = DTFLCF_OWNER;
		Date = "$Date: 2012/11/14 16:06:26 $";
	}
	virtual ~dtflcftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dtflcf;



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtflcfpersonnalization
: public dtflcftutor
{
public:
	dtflcfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtflcfpersonnalization( void )
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

static dtflcfpersonnalization Tutor;

ttr_tutor &DTFLCFTutor = Tutor;
