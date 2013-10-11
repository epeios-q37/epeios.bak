/*
	'ltf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'ltf' header file ('ltf.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: ltf.cpp,v 1.15 2013/04/11 19:36:50 csimon Exp $

#define LTF__COMPILATION

#include "ltf.h"

class ltftutor
: public ttr_tutor
{
public:
	ltftutor( void )
	: ttr_tutor( LTF_NAME )
	{
#ifdef LTF_DBG
		Version = LTF_VERSION "\b\bD $";
#else
		Version = LTF_VERSION;
#endif
		Owner = LTF_OWNER;
		Date = "$Date: 2013/04/11 19:36:50 $";
	}
	virtual ~ltftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ltf;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ltfpersonnalization
: public ltftutor
{
public:
	ltfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ltfpersonnalization( void )
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

static ltfpersonnalization Tutor;

ttr_tutor &LTFTutor = Tutor;
