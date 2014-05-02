/*
	'stsfsm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'stsfsm' header file ('stsfsm.h').
	Copyright (C) 20132004 Claude SIMON.

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



//	$Id: stsfsm.cpp,v 1.1 2013/07/18 19:46:25 csimon Exp $

#define STSFSM__COMPILATION

#include "stsfsm.h"

class stsfsmtutor
: public ttr_tutor
{
public:
	stsfsmtutor( void )
	: ttr_tutor( STSFSM_NAME )
	{
#ifdef STSFSM_DBG
		Version = STSFSM_VERSION "\b\bD $";
#else
		Version = STSFSM_VERSION;
#endif
		Owner = STSFSM_OWNER;
		Date = "$Date: 2013/07/18 19:46:25 $";
	}
	virtual ~stsfsmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace stsfsm;

void stsfsm::Add(
	const str::string_ &Tag,
	id__ Id,
	automat_ &Automat )
{
	sdr::row__ Row = Tag.First();
	crow__ Current = Automat.First(), Next = E_NIL;

	if ( Current == E_NIL ) {
		Current = Automat.New();
		Automat( Current ).Init();
	}


	while ( Row != E_NIL ) {

		Next = Automat( Current ).Get( Tag( Row ) );

		Automat.Flush();

		if ( Next == E_NIL ) {
			Next = Automat.New();
			Automat( Next ).Init();
			Automat( Current ).Set( Tag( Row ), Next );
			Automat.Flush();
		}

		Current = Next;

		Row = Tag.Next( Row );
	}

	if ( Current == E_NIL )
		ERRFwk();

	if ( Automat( Current ).GetId() != UndefinedId	)
		ERRFwk();

	Automat( Current ).SetId( Id );

	Automat.Flush();
}

status__ stsfsm::parser__::Handle( bso::u8__ C )
{
	ctn::E_CMITEMt( card_, crow__ ) Card;
	crow__ Next = E_NIL;

	Card.Init( _A() );

	if ( _Current == E_NIL )
		_Current = _A().First();

	if ( _Current == E_NIL )
		return sLost;	// L'automate est vide.

	Next = Card( _Current ).Get( C );

	if ( Next == E_NIL )
		return sLost;

	_Current = Next;

	if ( Card( _Current ).GetId() == UndefinedId )
		return sPending;
	else
		return sMatch;
}

id__ stsfsm::GetId_(
	const str::string_ &Pattern,
	const automat_ &Automat )
{
	id__ Id = UndefinedId;
ERRProlog
	parser__ Parser;
	sdr::row__ Row = E_NIL;
	bso::bool__ Match = false;
ERRBegin
	Row = Pattern.First();
	Parser.Init( Automat );

	while ( Row != E_NIL ) {
		switch ( Parser.Handle( Pattern( Row ) ) ) {
		case sLost:
			ERRReturn;
			break;
		case sMatch:
			Match = true;
			break;
		case sPending:
			Match = false;
			break;
		default:
			ERRFwk();
			break;
		}

		Row = Pattern.Next( Row );
	}

	if ( Match )
		Id = Parser.GetId();
ERRErr
ERREnd
ERREpilog
	return Id;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class stsfsmpersonnalization
: public stsfsmtutor
{
public:
	stsfsmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~stsfsmpersonnalization( void )
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

static stsfsmpersonnalization Tutor;

ttr_tutor &STSFSMTutor = Tutor;
