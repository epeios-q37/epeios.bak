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

//	$Id: dtfptb.h,v 1.25 2013/04/13 16:21:16 csimon Exp $

#ifndef DTFPTB__INC
#define DTFPTB__INC

#define DTFPTB_NAME		"DTFPTB"

#if defined( E_DEBUG ) && !defined( DTFPTB_NODBG )
#define DTFPTB_DBG
#endif

//D Data TransFert PorTaBle

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"

namespace dtfptb {
	using namespace dtfbsc;

	typedef bso::byte__ size_buffer__[10];

	#define DTFPTB_L1	255
	#define DTFPTB_L2	65535
	#define DTFPTB_L3	16777215

	inline bso::u8__ OldGetSizeLength( bso::u32__ Size )
	{
		if ( Size >= ( DTFPTB_L3 + DTFPTB_L2 + DTFPTB_L1 ) )
			return 10;
		else if ( Size >= ( DTFPTB_L2 + DTFPTB_L1 ) )
			return 6;
		else if ( Size >= DTFPTB_L1 )
			return 3;
		else
			return 1;
	}

	void OldPutSize(
		bso::u32__ Size,
		size_buffer__ &Buffer );

	void OldPutSize(
		bso::u32__ Size,
		flw::oflow__ &Flow );


	bso::u32__ OldGetSize( flw::iflow__ &Flow );

	bso::u32__ OldGetSize( fdr::rRDriver &Driver );

	bso::u32__ OldGetSize( const size_buffer__ &Buffer );

	typedef bso::u8__ _length__;

	void _FPutInt(
		bso::int__ Int,
		_length__ Length,
		flw::oflow__ &Flow );

	bso::int__ _FGetInt(
		flw::iflow__ &Flow,
		_length__ Length );

	bso::int__ _FGetInt(
		fdr::rRDriver &Driver,
		_length__ Length );

	template <typename i> inline void FPut(
		i Int,
		flw::rWFlow &Flow )
	{
		_FPutInt( Int, sizeof( Int ), Flow );
	}

	template <typename i, typename fd> inline i FGet(
		fd &FD,
		i &Int )
	{
		return Int = (i)_FGetInt( FD, sizeof( Int ) );
	}

# ifdef CPE_S_DARWIN
	template <typename fd> inline void FPut(
		bso::size__ Size,
		flw::rWFlow &Flow )
	{
		_FPutInt( Size, sizeof( Size ), Flow );
	}

	template <typename fd> inline bso::size__ FGet(
		fd &FD,
		bso::size__ &Size )
	{
		return Size = _FGetInt( FD, sizeof( Size ) );
	}
# endif

	bso::sUBig _VGetUBig(
		flw::iflow__ &Flow,
		bso::sUBig Max );

	bso::sUBig _VGetUBig(
		fdr::rRDriver &Driver,
		bso::sUBig Max );

	bso::sSBig _VGetSBig(
		flw::iflow__ &Flow,
		bso::sSBig Min,
		bso::sSBig Max );

	bso::sSBig _VGetSBig(
		fdr::rRDriver &Driver,
		bso::sSBig Min,
		bso::sSBig Max );

	void _VPutUBig(
		bso::sUBig UBig,
		flw::oflow__ &Flow );

	void _VPutSBig(
		bso::sSBig SBig,
		flw::oflow__ &Flow );

# ifdef CPE_S_DARWIN
	inline bso::size__ VGet(
		flw::iflow__ &Flow,
		bso::size__ &Size )
	{
		return Size = _VGetUBig( Flow, BSO_SIZE_MAX );
	}

	inline void VPut(
		bso::size__ Size,
		flw::oflow__ &Flow )
	{
		_VPutUBig( Size, Flow );
	}
# endif

# define DTFPTB__M( bitness, umax, smin, smax )\
	inline bso::u##bitness##__ VGet(\
		flw::rRFlow &Flow,\
		bso::u##bitness##__ &Int )\
	{\
		return Int = (bso::u##bitness##__)_VGetUBig( Flow, umax );\
	}\
	inline bso::u##bitness##__ VGet(\
		fdr::rRDriver &Driver,\
		bso::u##bitness##__ &Int )\
	{\
		return Int = (bso::u##bitness##__)_VGetUBig( Driver, umax );\
	}\
	inline bso::s##bitness##__ VGet(\
		flw::rRFlow &Flow,\
		bso::s##bitness##__ &Int )\
	{\
		return Int = (bso::s##bitness##__)_VGetSBig( Flow, smin, smax );\
	}\
	inline bso::s##bitness##__ VGet(\
		fdr::rRDriver &Driver,\
		bso::s##bitness##__ &Int )\
	{\
		return Int = (bso::s##bitness##__)_VGetSBig( Driver, smin, smax );\
	}\
	inline void VPut(\
		bso::u##bitness##__ Int,\
		flw::rWFlow &Flow )\
	{\
		_VPutUBig( Int, Flow );\
	}\
	inline void VPut(\
		bso::s##bitness##__ Int,\
		flw::rWFlow &Flow )\
	{\
		_VPutSBig( Int, Flow );\
	}\

DTFPTB__M( 32, BSO_U32_MAX, BSO_S32_MIN, BSO_S32_MAX )
DTFPTB__M( 16, BSO_U16_MAX, BSO_S16_MIN, BSO_S16_MAX )
DTFPTB__M( 64, BSO_U64_MAX, BSO_S64_MIN, BSO_S64_MAX )

// One byte is sent/received as is ; no conversion needed.
// DTFPTB__M( 8, BSO_U8_MAX, BSO_S8_MIN, BSO_S8_MAX )
	inline void VPut(
		bso::u8__ Int,
		flw::rWFlow &Flow )
	{
		FPut( Int, Flow );
	}

	inline bso::u8__ VGet(
		flw::rRFlow &Flow,
		bso::u8__ &Int )
	{
		return FGet( Flow, Int );
	}

	inline bso::u8__ VGet(
		fdr::rRDriver &Driver,
		bso::u8__ &Int )
	{
		return FGet( Driver, Int );
	}
}

#endif
