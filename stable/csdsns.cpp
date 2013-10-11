/*
	'csdsns' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdsns' header file ('csdsns.h').
	Copyright (C) 2004 Claude SIMON.

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



//	$Id: csdsns.cpp,v 1.24 2013/04/15 10:50:49 csimon Exp $

#define CSDSNS__COMPILATION

#include "csdsns.h"

class csdsnstutor
: public ttr_tutor
{
public:
	csdsnstutor( void )
	: ttr_tutor( CSDSNS_NAME )
	{
#ifdef CSDSNS_DBG
		Version = CSDSNS_VERSION "\b\bD $";
#else
		Version = CSDSNS_VERSION;
#endif
		Owner = CSDSNS_OWNER;
		Date = "$Date: 2013/04/15 10:50:49 $";
	}
	virtual ~csdsnstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdsns;

#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *csdsns::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( New );
		CASE( Store );
		CASE( TestAndGet );
		CASE( Delete );
	default:
		ERRPrm();
		return NULL;	// Pour éviter un 'warning'.
		break;
	}
}

void csdsns::_functions___::_Clean( void )
{
	sdr::row__ Row = _Core.UPs.First();

	while ( Row != E_NIL ) {
		_Functions->PostProcess( _Core.UPs( Row ) );

		Row = _Core.UPs.Next( Row );
	}

	_Core.reset();
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsnspersonnalization
: public csdsnstutor
{
public:
	csdsnspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsnspersonnalization( void )
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

static csdsnspersonnalization Tutor;

ttr_tutor &CSDSNSTutor = Tutor;
