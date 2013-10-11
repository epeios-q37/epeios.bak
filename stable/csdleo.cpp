/*
	'csdleo' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdleo' header file ('csdleo.h').
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



//	$Id: csdleo.cpp,v 1.12 2013/04/09 08:03:07 csimon Exp $

#define CSDLEO__COMPILATION

#include "csdleo.h"

class csdleotutor
: public ttr_tutor
{
public:
	csdleotutor( void )
	: ttr_tutor( CSDLEO_NAME )
	{
#ifdef CSDLEO_DBG
		Version = CSDLEO_VERSION "\b\bD $";
#else
		Version = CSDLEO_VERSION;
#endif
		Owner = CSDLEO_OWNER;
		Date = "$Date: 2013/04/09 08:03:07 $";
	}
	virtual ~csdleotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdleo;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdleopersonnalization
: public csdleotutor
{
public:
	csdleopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdleopersonnalization( void )
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

static csdleopersonnalization Tutor;

ttr_tutor &CSDLEOTutor = Tutor;
