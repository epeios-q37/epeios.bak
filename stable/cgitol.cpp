/*
	'cgitol' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'cgitol' header file ('cgitol.h').
	Copyright (C) 2002-2004 Claude SIMON.

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



//	$Id: cgitol.cpp,v 1.2 2013/07/25 15:59:14 csimon Exp $

#define CGITOL__COMPILATION

#include "cgitol.h"

class cgitoltutor
: public ttr_tutor
{
public:
	cgitoltutor( void )
	: ttr_tutor( CGITOL_NAME )
	{
#ifdef CGITOL_DBG
		Version = CGITOL_VERSION "\b\bD $";
#else
		Version = CGITOL_VERSION;
#endif
		Owner = CGITOL_OWNER;
		Date = "$Date: 2013/07/25 15:59:14 $";
	}
	virtual ~cgitoltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace cgitol;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class cgitolpersonnalization
: public cgitoltutor
{
public:
	cgitolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cgitolpersonnalization( void )
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

static cgitolpersonnalization Tutor;

ttr_tutor &CGITOLTutor = Tutor;
