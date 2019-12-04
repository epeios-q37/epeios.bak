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

#define FBLBRD_COMPILATION_

#include "fblbrd.h"


#include "cpe.h"

using namespace fblbrd;

void fblbrd::description_::Init(
	const char *Name,
	const cast__ *Casts )
{
	int i = 0;
	cast__ Cast = c_Undefined;
	bso::bool__ FlowCastDetected = false;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		Cast = Casts[i++];

		if ( FlowCastDetected ) {
			if ( Cast != cEnd )
				qRFwk();
		} else if ( Cast == cFlow )
			FlowCastDetected = true;

		this->Casts.Append( Cast );
	} while ( Cast != cEnd );

	FlowCastDetected = false;

	while ( ( Cast = Casts[i++] ) != cEnd ) {

		if ( FlowCastDetected )
				qRFwk();
		else if ( Cast == cFlow )
			FlowCastDetected = true;

		this->Casts.Append( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}

#if defined( CPE_C_GCC ) || defined( CPE_C_CLANG )
typedef int va_t__;
#else
typedef cast__ va_t__;
#endif

# if 0
void fblbrd::description_::Init(
	const char *Name,
	cast__ Cast,
	va_list VL )
{
	bso::bool__ FlowCastDetected = false;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	while ( Cast != cEnd )
	{
		if ( FlowCastDetected )
				qRFwk();
		else if ( Cast == cFlow )
			FlowCastDetected = true;

		this->Casts.Append( Cast );
		Cast = (cast__)va_arg( VL, va_t__ );
	}

	this->Casts.Append( cEnd );

	FlowCastDetected = false;

	while ( ( Cast = (cast__)va_arg( VL, va_t__ ) ) != cEnd ) {

		if ( FlowCastDetected )
				qRFwk();
		else if ( Cast == cFlow )
			FlowCastDetected = true;

		this->Casts.Append( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}
#endif


sdr::row__ fblbrd::descriptions_::Position( const description_ &Description ) const
{
	sdr::row__ Position = E_CONTAINER_( description_ )::First();

	while( ( Position != qNIL )
		    && ( ( Get( Position ).Name != Description.Name )
		         || ( Get( Position ).Casts != Description.Casts ) ) )
		Position = E_CONTAINER_( description_ )::Next( Position );

	return Position;
}
