/*
	'mdr' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mdr' header file ('mdr.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: mdr.cpp,v 1.26 2013/03/14 11:09:19 csimon Exp $

#define MDR__COMPILATION

#include "mdr.h"

class mdrtutor
: public ttr_tutor
{
public:
	mdrtutor( void )
	: ttr_tutor( MDR_NAME )
	{
#ifdef MDR_DBG
		Version = MDR_VERSION "\b\bD $";
#else
		Version = MDR_VERSION;
#endif
		Owner = MDR_OWNER;
		Date = "$Date: 2013/03/14 11:09:19 $";
	}
	virtual ~mdrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mdr;

xsize__ mdr::Convert( size__ Size )
{
	xsize__ XSize;
	length__ Position = MDR__DSIZE_SIZE_MAX - 1;

	XSize._Size[Position] = Size & 0x7f;
	Size >>= 7;

	while ( Size != 0 ) {
		if ( Position-- == 0 )
			ERRc();

		XSize._Size[Position] = ( Size & 0x7f ) | 0x80; 
		Size >>= 7;
	}

	XSize._Length = MDR__DSIZE_SIZE_MAX - Position;

	return XSize;
}

#define LIMIT ( MDR_SIZE_MAX >> 7 )

size__ mdr::Convert( const mdr::datum__ *DSize )
{
	length__ Position = 0;
	size__ Size = 0;

	do {
		if ( Size > LIMIT )
			ERRc();

		Size = ( Size << 7 ) + ( DSize[Position] & 0x7f );
	} while ( DSize[Position++] & 0x80 );

	return Size;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mdrpersonnalization
: public mdrtutor
{
public:
	mdrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mdrpersonnalization( void )
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

static mdrpersonnalization Tutor;

ttr_tutor &MDRTutor = Tutor;
