/*
	'csdsnb' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdsnb' header file ('csdsnb.h').
	Copyright (C) 2004 Claude SIMON.

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



//	$Id: csdsnb.cpp,v 1.4 2013/03/29 11:02:50 csimon Exp $

#define CSDSNB__COMPILATION

#include "csdsnb.h"

class csdsnbtutor
: public ttr_tutor
{
public:
	csdsnbtutor( void )
	: ttr_tutor( CSDSNB_NAME )
	{
#ifdef CSDSNB_DBG
		Version = CSDSNB_VERSION "\b\bD $";
#else
		Version = CSDSNB_VERSION;
#endif
		Owner = CSDSNB_OWNER;
		Date = "$Date: 2013/03/29 11:02:50 $";
	}
	virtual ~csdsnbtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdsnb;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsnbpersonnalization
: public csdsnbtutor
{
public:
	csdsnbpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsnbpersonnalization( void )
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

static csdsnbpersonnalization Tutor;

ttr_tutor &CSDSNBTutor = Tutor;
