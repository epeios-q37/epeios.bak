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

#define BSO_COMPILATION_

#include "bso.h"

# include "tol.h"

using namespace bso;

const xint__ &bso::_ConvertToDInt(
	sUHuge Huge,
	xint__ &XInt )
{
	length__ Position = BSO_DINT_SIZE_MAX - 1;

	XInt._Int[Position] = Huge & 0x7f;
	Huge >>= 7;

	while ( Huge != 0 ) {
		if ( Position-- == 0 )
			qRFwk();

		XInt._Int[Position] = ( Huge & 0x7f ) | 0x80;
		Huge >>= 7;
	}

	XInt._Length = BSO_DINT_SIZE_MAX - Position;

	return XInt;
}

#define LIMIT ( BSO_HUGE_MAX >> 7 )

sHuge bso::ConvertToHuge(
	const byte__ *DInt,
	size__ *Length )
{
	length__ Position = 0;
	sHuge Huge = 0;

	do {
		if ( Huge > LIMIT )
			qRFwk();

		Huge = ( Huge << 7 ) + ( DInt[Position] & 0x7f );
	} while ( DInt[Position++] & 0x80 );

	if ( Length != NULL )
		*Length = Position;

	return Huge;
}

namespace {
#ifdef CPE_F_64BITS
  qCDEF( bso::sU64, EndianessSealSource_, 0x4142434445464748);
#elif defined( CPE_F_32BITS )
  qCDEF( bso::sU32, EndianessSealSource_, 0x41424344);
#else
# error
#endif

  char EndianessSeal_[sizeof( EndianessSealSource_ )];

  void BuildEndianessSeal_(void)
  {
    strncpy(EndianessSeal_, reinterpret_cast<const char *>(&EndianessSealSource_), sizeof( EndianessSealSource_ ));
  }
}

const char *bso::EndianessSeal(void)
{
  return EndianessSeal_;
}

Q37_GCTOR( bso )
{
	if ( sizeof( size__ ) != sizeof( int__ ) )
		qRChk();

	if ( sizeof( sint__ ) != sizeof( uint__ ) )
		qRChk();

	if ( sizeof( int__ ) != sizeof( uint__ ) )
		qRChk();

  if ( sizeof( EndianessSealSource_) != BSO_ENDIANESS_SEAL_SIZE )
    qRChk();

  BuildEndianessSeal_();
}
