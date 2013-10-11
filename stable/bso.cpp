/*
	'bso' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bso' header file ('bso.h').
	Copyright (C) 2000-2004 Claude SIMON.

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



//	$Id: bso.cpp,v 1.61 2013/04/11 19:36:47 csimon Exp $

#define BSO__COMPILATION

#include "bso.h"

class bsotutor
: public ttr_tutor
{
public:
	bsotutor( void )
	: ttr_tutor( BSO_NAME )
	{
#ifdef BSO_DBG
		Version = BSO_VERSION "\b\bD $";
#else
		Version = BSO_VERSION;
#endif
		Owner = BSO_OWNER;
		Date = "$Date: 2013/04/11 19:36:47 $";
	}
	virtual ~bsotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace bso;

const xint__ &bso::_ConvertToDInt(
	int__ UInt,
	xint__ &XInt )
{
	length__ Position = BSO_DINT_SIZE_MAX - 1;

	XInt._Int[Position] = UInt & 0x7f;
	UInt >>= 7;

	while ( UInt != 0 ) {
		if ( Position-- == 0 )
			ERRDta();

		XInt._Int[Position] = ( UInt & 0x7f ) | 0x80; 
		UInt >>= 7;
	}

	XInt._Length = BSO_DINT_SIZE_MAX - Position;

	return XInt;
}

#define LIMIT ( BSO_INT_MAX >> 7 )

int__ bso::ConvertToInt(
	const raw__ *DInt,
	size__ &Length )
{
	length__ Position = 0;
	int__ Int = 0;

	do {
		if ( Int > LIMIT )
			ERRDta();

		Int = ( Int << 7 ) + ( DInt[Position] & 0x7f );
	} while ( DInt[Position++] & 0x80 );

	if ( &Length != NULL )
		Length = Position;

	return Int;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class bsopersonnalization
: public bsotutor
{
public:
	bsopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */

		if ( sizeof( size__ ) != sizeof( int__ ) )
			ERRChk();

		if ( sizeof( sint__ ) != sizeof( uint__ ) )
			ERRChk();

		if ( sizeof( int__ ) != sizeof( uint__ ) )
			ERRChk();
	}
	~bsopersonnalization( void )
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

static bsopersonnalization Tutor;

ttr_tutor &BSOTutor = Tutor;
