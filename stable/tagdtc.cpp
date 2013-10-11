/*
	'tagdtc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'tagdtc' header file ('tagdtc.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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



//	$Id: tagdtc.cpp,v 1.11 2012/11/14 16:06:37 csimon Exp $

#define TAGDTC__COMPILATION

#include "tagdtc.h"

class tagdtctutor
: public ttr_tutor
{
public:
	tagdtctutor( void )
	: ttr_tutor( TAGDTC_NAME )
	{
#ifdef TAGDTC_DBG
		Version = TAGDTC_VERSION "\b\bD $";
#else
		Version = TAGDTC_VERSION;
#endif
		Owner = TAGDTC_OWNER;
		Date = "$Date: 2012/11/14 16:06:37 $";
	}
	virtual ~tagdtctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace tagdtc;

#define NO	TAGDTC_NO

const tagdtc::char__ tagdtc::Table[] = {
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	0,  1,  2,  3,  4,  5,  6,  7,
	8,  9,  NO, NO, NO, NO, NO, NO,
	NO, 10, 11, 12, 13, 14, 15, 16, 
	17, 18, 19, 20, 21, 22, 23, 24,
	25,	27, 28, 29, 30, 31, 32, 33,
	34,	35, 36, NO, NO, NO, NO, 37,
	NO, 38, 39, 40, 41, 42, 43, 44,
	45, 46, 47, 48, 49, 50, 51, 46,
	53, 54, 55, 56, 57, 58, 59, 60,
	61, 62, 63, NO, NO, NO, NO, NO,
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO, 
	NO, NO, NO, NO, NO, NO, NO, NO
};

void tagdtc::tag_detector_::Add(
	const char *Tag,
	id__ Id )
{
	size_t Limite = strlen( Tag );
	path__ Path = Start_;

	for( size_t P = 0; P < Limite; P++ )
		Path = GetNext_( Path, Tag[P] );

	Ids.Store( Id, Path );
}

void tagdtc::tag_detector_::Add(
	const str::string_ &Tag,
	id__ Id )
{
	epeios::row_t__ Limite = Tag.Amount();
	path__ Path = Start_;

	for( epeios::row_t__ P = 0; P < Limite; P++ )
		Path = GetNext_( Path, Tag( P ) );

	Ids.Store( Id, Path );
}

epeios::row_t__ tagdtc::tag_detector_::Parse(
	xtf::extended_text_iflow__ &IFlow,
	txf::text_oflow__ &OFlow,
	char Delimiter,
	tagdtc::action Action,
	err::handle ErrHandle ) const
{
	char__ C;
	id__ Return = TAGDTC_UNKNOW;

	if ( Action == tagdtc::aPrint )
		while( !IFlow.EOX() && ( ( C = IFlow.Get() ) != Delimiter ) )
			OFlow.Put( C );
	else
		while( !IFlow.EOX() && ( ( C = IFlow.Get() ) != Delimiter ) );

	if ( !IFlow.EOX() ) {
		epeios::row__ P = Start_;

		while( !IFlow.EOX()
			   && ( ( C = IFlow.Get() ) != Delimiter )
			   && ( ( C = Table[C] ) != NO )
			   && ( ( P = Cards.Get( P )[C] ) != TAGDTC_UNKNOW ) );

		if ( C == Delimiter )
			Return = Ids.Get( P );

		if ( ( Return == TAGDTC_UNKNOW )
			  && ( ErrHandle == err::hUsual ) )
			ERRu();
			
	}
	else if ( C != Delimiter )
		Return = TAGDTC_EOF;

	return *Return;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class tagdtcpersonnalization
: public tagdtctutor
{
public:
	tagdtcpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tagdtcpersonnalization( void )
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

static tagdtcpersonnalization Tutor;

ttr_tutor &TAGDTCTutor = Tutor;
