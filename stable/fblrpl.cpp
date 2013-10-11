/*
	'fblrpl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblrpl' header file ('fblrpl.h').
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



//	$Id: fblrpl.cpp,v 1.3 2011/10/03 08:22:42 csimon Exp $

#define FBLRPL__COMPILATION

#include "fblrpl.h"

class fblrpltutor
: public ttr_tutor
{
public:
	fblrpltutor( void )
	: ttr_tutor( FBLRPL_NAME )
	{
#ifdef FBLRPL_DBG
		Version = FBLRPL_VERSION "\b\bD $";
#else
		Version = FBLRPL_VERSION;
#endif
		Owner = FBLRPL_OWNER;
		Date = "$Date: 2011/10/03 08:22:42 $";
	}
	virtual ~fblrpltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblrpl;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblrplpersonnalization
: public fblrpltutor
{
public:
	fblrplpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblrplpersonnalization( void )
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

static fblrplpersonnalization Tutor;

ttr_tutor &FBLRPLTutor = Tutor;
