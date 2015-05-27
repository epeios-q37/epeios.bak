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

