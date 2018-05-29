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

#ifndef DTFBSC__INC
#define DTFBSC__INC

#define DTFBSC_NAME		"DTFBSC"

#define	DTFBSC_VERSION	"$Revision: 1.12 $"

#define DTFBSC_OWNER		"Claude SIMON"

#if defined( E_DEBUG ) && !defined( DTFBSC_NODBG )
#define DTFBSC_DBG
#endif

// Data transFer BaSiC.


#include "err.h"
#include "flw.h"
#include "bso.h"

namespace dtfbsc {
	using bso::u8__;
	using bso::s8__;
	using bso::u16__;
	using bso::s16__;
	using bso::u32__;
	using bso::s32__;

# ifdef BSO_64BITS_ENABLED
	using bso::u64__;
	using bso::s64__;
#endif

	//f Put 'UByte' in 'Flow'.
	inline void PutU8(
		u8__ U8,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::byte__ *)&U8 );
	}

	//f Return unsigned byte in 'Flow'.
	inline u8__ GetU8( flw::iflow__ &Flow )
	{
		return (u8__)Flow.Get();
	}

	//f Put 'SByte' in 'Flow'.
	inline void PutS8(
		s8__ S8,
		flw::oflow__ &Flow )
	{
		Flow.Put( *(flw::byte__ *)&S8 );
	}

	//f Return unsigned byte in 'Flow'.
	inline s8__ GetS8( flw::iflow__ &Flow )
	{
		return (s8__)Flow.Get();
	}

}

#endif
