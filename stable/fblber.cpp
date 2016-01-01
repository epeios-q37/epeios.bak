/*
	Copyright (C) 1999-2016 Claude SIMON (http://q37.info/contact/).

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
	sdr::row__ Row = qNIL;
	cast__ Cast = c_Undefined;
	bso::bool__ FlowParameterDetected = false;

	Row = Casts.First();

	while ( ( Row != qNIL )
		    && ( ( Cast = (cast__)Casts( Row ) ) != cEnd ) ) {

		if ( FlowParameterDetected )
			qRFwk();

		if ( Flow.Get() != Cast )
			qRFwk();

		if ( Cast == cFlow ) {
			FlowParameterDetected = true;
		} else {
			if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
				qRFwk();
		}

		Row = Casts.Next( Row );
	}

	if ( Row == qNIL )
		qRFwk();

	if ( Flow.Get() != fblcst::cEnd )
		qRFwk();

	if ( FlowParameterDetected ) {
		if ( Repository_.Append( GetPointer_( Flow ) ) != Casts.Previous( Row ) )
			qRFwk();

		FlowParameterDetected= false;
	}

	if ( Repository_.Append( (void *)NULL ) != Row )
		qRFwk();

	Row = Casts.Next( Row );

	while ( Row != qNIL ) {
		if ( ( Cast = (cast__)Flow.Get() ) != Casts( Row ) )
			qRFwk();

		if ( FlowParameterDetected )
			qRFwk();

		if ( Cast == cFlow )
			FlowParameterDetected = true;

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			qRFwk();

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
		qRFwk();
}

void fblber::embedded_callbacks___::FBLBRQPopInEnd(
	sdr::row__ CRow,
	flw::iflow__ &Flow )
{
	if ( Repository_.Append( (void *)NULL ) != CRow )
		qRFwk();
}

void fblber::embedded_callbacks___::FBLBRQPopOut(
	sdr::row__ CRow,
	flw::iflow__ &Flow,
	cast__ Cast )
{
	if ( Repository_.Append( GetPointer_( Flow ) ) != CRow )
		qRFwk();
}

