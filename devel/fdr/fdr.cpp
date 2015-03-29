/*
	'fdr' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fdr' header file ('fdr.h').
	Copyright (C) , 2004 Claude SIMON.

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



//	$Id: fdr.cpp,v 1.18 2013/07/19 14:54:34 csimon Exp $

#define FDR__COMPILATION

#include "fdr.h"

class fdrtutor
: public ttr_tutor
{
public:
	fdrtutor( void )
	: ttr_tutor( FDR_NAME )
	{
#ifdef FDR_DBG
		Version = FDR_VERSION "\b\bD $";
#else
		Version = FDR_VERSION;
#endif
		Owner = FDR_OWNER;
		Date = "$Date: 2013/07/19 14:54:34 $";
	}
	virtual ~fdrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fdr;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fdrpersonnalization
: public fdrtutor
{
public:
	fdrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fdrpersonnalization( void )
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

static fdrpersonnalization Tutor;

ttr_tutor &FDRTutor = Tutor;
