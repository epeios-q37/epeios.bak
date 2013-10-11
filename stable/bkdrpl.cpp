/*
	'bkdrpl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdrpl' header file ('bkdrpl.h').
	Copyright (C) 2001-2004 Claude SIMON.

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



//	$Id: bkdrpl.cpp,v 1.4 2011/06/11 18:16:00 csimon Exp $

#define BKDRPL__COMPILATION

#include "bkdrpl.h"

class bkdrpltutor
: public ttr_tutor
{
public:
	bkdrpltutor( void )
	: ttr_tutor( BKDRPL_NAME )
	{
#ifdef BKDRPL_DBG
		Version = BKDRPL_VERSION "\b\bD $";
#else
		Version = BKDRPL_VERSION;
#endif
		Owner = BKDRPL_OWNER;
		Date = "$Date: 2011/06/11 18:16:00 $";
	}
	virtual ~bkdrpltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bkdrpl;

#define CASE( name )	LCL_CASE( name, r )

const char *bkdrpl::Label( reply__ Reply )
{
	switch ( Reply ) {
		CASE( BackendError );
		CASE( UserError );
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bkdrplpersonnalization
: public bkdrpltutor
{
public:
	bkdrplpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bkdrplpersonnalization( void )
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

static bkdrplpersonnalization Tutor;

ttr_tutor &BKDRPLTutor = Tutor;
