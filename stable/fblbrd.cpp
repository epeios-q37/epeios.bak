/*
	'fblbrd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblbrd' header file ('fblbrd.h').
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



//	$Id: fblbrd.cpp,v 1.6 2013/04/15 10:50:50 csimon Exp $

#define FBLBRD__COMPILATION

#include "fblbrd.h"

class fblbrdtutor
: public ttr_tutor
{
public:
	fblbrdtutor( void )
	: ttr_tutor( FBLBRD_NAME )
	{
#ifdef FBLBRD_DBG
		Version = FBLBRD_VERSION "\b\bD $";
#else
		Version = FBLBRD_VERSION;
#endif
		Owner = FBLBRD_OWNER;
		Date = "$Date: 2013/04/15 10:50:50 $";
	}
	virtual ~fblbrdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblbrd;

void fblbrd::description_::Init(
	const char *Name,
	const cast *Casts )
{
	int i = 0;
	cast Cast = c_Unknow;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		Cast = Casts[i++];
		this->Casts.Append( Cast );
	} while ( Cast != cEnd );

	while ( ( Cast = Casts[i++] ) != cEnd ) {
		this->Casts.Append( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}


sdr::row__ fblbrd::descriptions_::Position( const description_ &Description ) const
{
	sdr::row__ Position = E_CONTAINER_( description_ )::First();
ERRProlog
	E_CITEM( description_ ) D;
ERRBegin

	D.Init( *this );

	while( ( Position != E_NIL )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = E_CONTAINER_( description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblbrdpersonnalization
: public fblbrdtutor
{
public:
	fblbrdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblbrdpersonnalization( void )
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

static fblbrdpersonnalization Tutor;

ttr_tutor &FBLBRDTutor = Tutor;
