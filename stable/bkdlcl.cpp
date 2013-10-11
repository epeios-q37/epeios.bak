/*
	'bkdlcl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdlcl' header file ('bkdlcl.h').
	Copyright (C) 2002-2004 Claude SIMON.

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



//	$Id: bkdlcl.cpp,v 1.2 2011/06/11 18:16:00 csimon Exp $

#define BKDLCL__COMPILATION

#include "bkdlcl.h"

class bkdlcltutor
: public ttr_tutor
{
public:
	bkdlcltutor( void )
	: ttr_tutor( BKDLCL_NAME )
	{
#ifdef BKDLCL_DBG
		Version = BKDLCL_VERSION "\b\bD $";
#else
		Version = BKDLCL_VERSION;
#endif
		Owner = BKDLCL_OWNER;
		Date = "$Date: 2011/06/11 18:16:00 $";
	}
	virtual ~bkdlcltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bkdlcl;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bkdlclpersonnalization
: public bkdlcltutor
{
public:
	bkdlclpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bkdlclpersonnalization( void )
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

static bkdlclpersonnalization Tutor;

ttr_tutor &BKDLCLTutor = Tutor;
