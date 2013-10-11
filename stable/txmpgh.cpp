/*
	'txmpgh' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'txmpgh' header file ('txmpgh.h').
	Copyright (C) 2000-2001,2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: txmpgh.cpp,v 1.9 2012/11/14 16:06:38 csimon Exp $

#define TXMPGH__COMPILATION

#include "txmpgh.h"

class txmpghtutor
: public ttr_tutor
{
public:
	txmpghtutor( void )
	: ttr_tutor( TXMPGH_NAME )
	{
#ifdef TXMPGH_DBG
		Version = TXMPGH_VERSION "\b\bD $";
#else
		Version = TXMPGH_VERSION;
#endif
		Owner = TXMPGH_OWNER;
		Date = "$Date: 2012/11/14 16:06:38 $";
	}
	virtual ~txmpghtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

void txmpgh::paragraph_::GetSplitParagraph_( xtf::extended_text_iflow__ &Flow )
{
ERRProlog
	line Line;
	bso::bool__ Cont = true;
ERRBegin

	do {
		Line.Init();

		Flow.GetLine( Line );

		if ( Line.Amount() )
			Append( Line );
		else
			Cont = false;

		if ( Flow.EOX() )
			Cont = false;
	} while (Cont );
ERRErr
ERREnd
ERREpilog
}

void txmpgh::paragraph_::Merge( line_ &Line ) const
{
	epeios::row__ P = lines_::First();
	ctn::E_CMITEM( line_ ) Part;

	Part.Init( *this );

	if ( P != NONE ) {
		Line.Append( Part( P ) );
		P = lines_::Next( P );
	}

	while( P != NONE ) {
		Line.Append( ' ' );
		Line.Append( Part( P ) );
		P = lines_::Next( P );
	}
}



txf::text_oflow__ &operator <<(
	txf::text_oflow__ &F,
	const txmpgh::paragraph_ &P )
{
	epeios::row__ Position = P.First();
	ctn::E_CMITEM( str::string_ ) Element;

	Element.Init( P );

	while ( Position != NONE ) {
		F << Element( Position ) << txf::nl;
		Position = P.Next( Position );
	}

	return F;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class txmpghpersonnalization
: public txmpghtutor
{
public:
	txmpghpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~txmpghpersonnalization( void )
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

static txmpghpersonnalization Tutor;

ttr_tutor &TXMPGHTutor = Tutor;
