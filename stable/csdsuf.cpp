/*
	'csdsuf' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdsuf' header file ('csdsuf.h').
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



//	$Id: csdsuf.cpp,v 1.1 2011/09/23 16:50:53 csimon Exp $

#define CSDSUF__COMPILATION

#include "csdsuf.h"

class csdsuftutor
: public ttr_tutor
{
public:
	csdsuftutor( void )
	: ttr_tutor( CSDSUF_NAME )
	{
#ifdef CSDSUF_DBG
		Version = CSDSUF_VERSION "\b\bD $";
#else
		Version = CSDSUF_VERSION;
#endif
		Owner = CSDSUF_OWNER;
		Date = "$Date: 2011/09/23 16:50:53 $";
	}
	virtual ~csdsuftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdsuf;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsufpersonnalization
: public csdsuftutor
{
public:
	csdsufpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsufpersonnalization( void )
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

static csdsufpersonnalization Tutor;

ttr_tutor &CSDSUFTutor = Tutor;
