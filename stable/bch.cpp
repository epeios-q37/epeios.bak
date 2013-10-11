/*
	'bch' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'bch' header file ('bch.h').
	Copyright (C) 2001-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: bch.cpp,v 1.95 2013/04/06 14:50:43 csimon Exp $

#define BCH__COMPILATION

#include "bch.h"

class bchtutor
: public ttr_tutor
{
public:
	bchtutor( void )
	: ttr_tutor( BCH_NAME )
	{
#ifdef BCH_DBG
		Version = BCH_VERSION "\b\bD $";
#else
		Version = BCH_VERSION;
#endif
		Owner = BCH_OWNER;
		Date = "$Date: 2013/04/06 14:50:43 $";
	}
	virtual ~bchtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "sdr.h"

using namespace bch;

void bch::_GetRelations(
	const uys::untyped_storage_ &Sorted,
	const uys::untyped_storage_ &Unsorted,
	sdr::size__ Size,
	sdr::row_t__ Limit,
	sdr::datum__ *Buffer,
	E_BUNCH_( sdr::row__ ) &Relations )
{
	sdr::row_t__ Row = 0;

	while ( Row < Limit ) {
		Sorted.Recall( Row, Size, Buffer );
		Relations.Append( Unsorted.Search( Buffer, Size, 0, Limit ) / Size );

		Row += Size;
	}
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class bchpersonnalization
: public bchtutor
{
public:
	bchpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bchpersonnalization( void )
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

static bchpersonnalization Tutor;

ttr_tutor &BCHTutor = Tutor;
