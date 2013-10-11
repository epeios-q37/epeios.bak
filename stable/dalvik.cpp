/*
	'dalvik' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dalvik' header file ('dalvik.h').
	Copyright (C) 20122004 Claude SIMON.

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



//	$Id: dalvik.cpp,v 1.1 2012/12/04 16:04:27 csimon Exp $

#define DALVIK__COMPILATION

#include "dalvik.h"

class dalviktutor
: public ttr_tutor
{
public:
	dalviktutor( void )
	: ttr_tutor( DALVIK_NAME )
	{
#ifdef DALVIK_DBG
		Version = DALVIK_VERSION "\b\bD $";
#else
		Version = DALVIK_VERSION;
#endif
		Owner = DALVIK_OWNER;
		Date = "$Date: 2012/12/04 16:04:27 $";
	}
	virtual ~dalviktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dalvik;

using namespace jvabse;
using namespace dvkbse;

class dalvikpersonnalization
: public dalviktutor
{
public:
	dalvikpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dalvikpersonnalization( void )
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

static dalvikpersonnalization Tutor;

ttr_tutor &DALVIKTutor = Tutor;
