/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define TXMPGH__COMPILATION

#include "txmpgh.h"


void txmpgh::paragraph_::GetSplitParagraph_( xtf::extended_text_iflow__ &Flow )
{
qRH
	line Line;
	bso::bool__ Cont = true;
qRB

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
qRR
qRT
qRE
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

