/*
	'xulfrg' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfrg' header file ('xulfrg.h').
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



//	$Id: xulfrg.cpp,v 1.12 2013/01/02 17:32:14 csimon Exp $

#define XULFRG__COMPILATION

#include "xulfrg.h"

class xulfrgtutor
: public ttr_tutor
{
public:
	xulfrgtutor( void )
	: ttr_tutor( XULFRG_NAME )
	{
#ifdef XULFRG_DBG
		Version = XULFRG_VERSION "\b\bD $";
#else
		Version = XULFRG_VERSION;
#endif
		Owner = XULFRG_OWNER;
		Date = "$Date: 2013/01/02 17:32:14 $";
	}
	virtual ~xulfrgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xulfrg;

rgstry::entry___ xulfrg::AnnexTarget( "AnnexTarget", frdrgy::Parameters );
rgstry::entry___ xulfrg::AnnexTargetType( "@Type", AnnexTarget );

static rgstry::entry___ BackendSelection_( "BackendSelection", frdrgy::Parameters );
rgstry::entry___ xulfrg::BackendSelectionMode( "Mode", BackendSelection_ );

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfrgpersonnalization
: public xulfrgtutor
{
public:
	xulfrgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfrgpersonnalization( void )
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

static xulfrgpersonnalization Tutor;

ttr_tutor &XULFRGTutor = Tutor;
