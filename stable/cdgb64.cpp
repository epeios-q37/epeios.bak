/*
	'cdgb64' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'cdgb64' header file ('cdgb64.h').
	Copyright (C) 20112004 Claude SIMON.

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



//	$Id: cdgb64.cpp,v 1.12 2013/04/26 14:43:40 csimon Exp $

#define CDGB64__COMPILATION

#include "cdgb64.h"

class cdgb64tutor
: public ttr_tutor
{
public:
	cdgb64tutor( void )
	: ttr_tutor( CDGB64_NAME )
	{
#ifdef CDGB64_DBG
		Version = CDGB64_VERSION "\b\bD $";
#else
		Version = CDGB64_VERSION;
#endif
		Owner = CDGB64_OWNER;
		Date = "$Date: 2013/04/26 14:43:40 $";
	}
	virtual ~cdgb64tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace cdgb64;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

bso::raw__ *cdgb64::Encode = (bso::raw__ *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

bso::raw__ *cdgb64::Decode = (bso::raw__ *)
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@c"
						"@@@@@@@@@@@@@\x3e@@"
						"\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d@@@@@@"
						"@\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e"
						"\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19@@@@\x3f"
						"@\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28"
						"\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@"
						"@@@@@@@@@@@@@@@@";



class cdgb64personnalization
: public cdgb64tutor
{
public:
	cdgb64personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~cdgb64personnalization( void )
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

static cdgb64personnalization Tutor;

ttr_tutor &CDGB64Tutor = Tutor;
