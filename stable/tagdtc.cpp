/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define TAGDTC__COMPILATION

#include "tagdtc.h"

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
