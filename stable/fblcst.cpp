/*
	'fblcst' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblcst' header file ('fblcst.h').
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



//	$Id: fblcst.cpp,v 1.5 2013/04/14 14:37:28 csimon Exp $

#define FBLCST__COMPILATION

#include "fblcst.h"

class fblcsttutor
: public ttr_tutor
{
public:
	fblcsttutor( void )
	: ttr_tutor( FBLCST_NAME )
	{
#ifdef FBLCST_DBG
		Version = FBLCST_VERSION "\b\bD $";
#else
		Version = FBLCST_VERSION;
#endif
		Owner = FBLCST_OWNER;
		Date = "$Date: 2013/04/14 14:37:28 $";
	}
	virtual ~fblcsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblcst;

// MUST reflect 'bkdcst::cast'
const char *fblcst::CastsNames[c_amount] = {
	"_end_",
	"Object",
	"Boolean",
	"Booleans",
	"SInt",
	"SInts",
	"UInt",
	"UInts",
	"Id8",
	"Id8s",
	"XId8s",
	"Id16",
	"Id16s",
	"XId16s",
	"Id32", 
	"Id32s", 
	"XId32s", 
	"Id",
	"Ids",
	"XIds", 
	"Char",
	"String",
	"Strings",
	"XStrings",
	"Byte",
	"Binary",
	"Binaries",
	"Item8s",
	"Item16s",
	"Item32s",
	"Items",
	"XItem8s",
	"XItem16s",
	"XItem32s",
	"XItems",
	"CommandsDetails",
	"ObjectsReferences",
	"Flow",
};

cast__ fblcst::GetID( const char *CastName )
{
	int i = 0;

	while( ( i < c_amount ) && strcmp( CastName, CastsNames[i] ) )
		i++;

	if ( i >= c_amount )
		i = c_Undefined;

	return (cast__)i;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblcstpersonnalization
: public fblcsttutor
{
public:
	fblcstpersonnalization( void )
	{
		if ( c_amount > bso::S8Max )
			ERRChk();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblcstpersonnalization( void )
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

static fblcstpersonnalization Tutor;

ttr_tutor &FBLCSTTutor = Tutor;
