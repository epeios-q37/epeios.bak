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

#define TXMPGH__COMPILATION

#include "txmpgh.h"


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

