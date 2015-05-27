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

#define FBLBER__COMPILATION

#include "fblber.h"

using namespace fblber;

static inline void *GetPointer_( flw::iflow__ &Flow )
{
	void *P = NULL;

	flw::Get( Flow, P );

	return P;
}

# if 0
void fblber::embedded_callbacks___::_GetAll(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	sdr::row__ Row = E_NIL;
	cast__ Cast = c_Undefined;
	bso::bool__ FlowParameterDetected = false;

	Row = Casts.First();

	while ( ( Row != E_NIL )
		    && ( ( Cast = (cast__)Casts( Row ) ) != cEnd ) ) {

		if ( FlowParameterDetected )
			ERRFwk();

		if ( Flow.Get() != Cast )
			ERRFwk();

		if ( Cast == cFlow ) {
			FlowParameterDetected = true;
		} else {
			if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
				ERRFwk();
		}

		Row = Casts.Next( Row );
	}

	if ( Row == E_NIL )
		ERRFwk();

	if ( Flow.Get() != fblcst::cEnd )
		ERRFwk();

	if ( FlowParameterDetected ) {
		if ( Repository_.Append( GetPointer_( Flow ) ) != Casts.Previous( Row ) )
			ERRFwk();

		FlowParameterDetected= false;
	}

	if ( Repository_.Append( (void *)NULL ) != Row )
		ERRFwk();

	Row = Casts.Next( Row );

	while ( Row != E_NIL ) {
		if ( ( Cast = (cast__)Flow.Get() ) != Casts( Row ) )
			ERRFwk();

		if ( FlowParameterDetected )
			ERRFwk();

		if ( Cast == cFlow )
			FlowParameterDetected = true;

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			ERRFwk();

		Row = Casts.Next( Row );
	}
}
#endif

void fblber::embedded_callbacks___::FBLBRQPopIn(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( Repository_.Append( GetPointer_( Flow ) ) != CRow )
		ERRFwk();
}

void fblber::embedded_callbacks___::FBLBRQPopInEnd(
	sdr::row__ CRow,
	flw::iflow__ &Flow )
{
	if ( Repository_.Append( (void *)NULL ) != CRow )
		ERRFwk();
}

void fblber::embedded_callbacks___::FBLBRQPopOut(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( Repository_.Append( GetPointer_( Flow ) ) != CRow )
		ERRFwk();
}

