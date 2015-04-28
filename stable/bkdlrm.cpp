/*
	'bkdlrm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bkdlrm' header file ('bkdlrm.h').
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

#define BKDLRM__COMPILATION

#include "bkdlrm.h"

using namespace bkdlrm;

static inline void *GetPointer_( flw::iflow__ &Flow )
{
	void *P = NULL;

	flw::Get( Flow, P );

	return P;
}

void bkdlrm::local_request_manager::_GetAll(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	epeios::row__ Row = NONE;

	Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {

		if ( Flow.Get() != *Casts( Row ) )
			ERRb();

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			ERRc();

		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRc();

	if ( Flow.Get() != bkdcst::cEnd )
		ERRb();

	if ( Repository_.Append( (void *)NULL ) != Row )
		ERRc();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		if ( Flow.Get() != *Casts( Row ) )
			ERRb();

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			ERRc();

		Row = Casts.Next( Row );
	}
}

