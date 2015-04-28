/*
	'fblber' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblber' header file ('fblber.h').
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

