/*
  'shm' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Requires the 'shm' header file ('shm.h').
  Copyright (C) 2000 Claude L. Simon.

  This file is part of the Epeios project (http://zeusw.org/epeios/).
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id: shm.cpp,v 1.4 2012/11/14 16:06:36 csimon Exp $

#define SHM__COMPILATION

#include "shm.h"

class shmtutor
: public ttr_tutor
{
public:
	shmtutor( void )
	: ttr_tutor( SHM_NAME )
	{
#ifdef SHM_DBG
		Version = SHM_VERSION " (DBG)";
#else
		Version = SHM_VERSION;
#endif
		Owner = SHM_OWNER;
		Date = "$Date: 2012/11/14 16:06:36 $";
	}
	virtual ~shmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class shmpersonnalization
: public shmtutor
{
public:
	shmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~shmpersonnalization( void )
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

static shmpersonnalization Tutor;

ttr_tutor &SHMTutor = Tutor;
