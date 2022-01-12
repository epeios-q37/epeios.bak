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

#ifndef DTFPTB_INC_
#define DTFPTB_INC_

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

	void FPutInt_(
		bso::int__ Int,
		_length__ Length,
		flw::oflow__ &Flow );

	void FPutInt_(
		bso::int__ Int,
		_length__ Length,
		fdr::rWDriver &Driver );

	template <typename i, typename fd> inline void FPut(
		i Int,
		fd &FD )
	{
		FPutInt_( Int, sizeof( Int ), FD );
	}

	bso::int__ FGetInt_(
		flw::iflow__ &Flow,
		_length__ Length,
		bso::sBool *IsError );

	bso::int__ FGetInt_(
		fdr::rRDriver &Driver,
		_length__ Length,
		bso::sBool *IsError );

	template <typename i, typename fd> inline i FGet(
		fd &FD,
		i &Int,
		bso::sBool *IsError = NULL )
	{
		return Int = (i)FGetInt_( FD, sizeof( Int ), IsError );
	}

# ifdef CPE_S_DARWIN
	template <typename fd> inline void FPut(
		bso::size__ Size,
		flw::rWFlow &Flow )
	{
		FPutInt_( Size, sizeof( Size ), Flow );
	}

	template <typename fd> inline bso::size__ FGet(
		fd &FD,
		bso::size__ &Size )
	{
		return Size = FGetInt_( FD, sizeof( Size ) );
	}
# endif

	bso::sUHuge VGetUHuge_(
		flw::iflow__ &Flow,
		bso::sUHuge Max,
		bso::sBool *IsError );

	bso::sUHuge VGetUHuge_(
		fdr::rRDriver &Driver,
		bso::sUHuge Max,
		bso::sBool *IsError );

	bso::sSHuge VGetSHuge_(
		flw::iflow__ &Flow,
		bso::sSHuge Min,
		bso::sSHuge Max,
		bso::sBool *IsError );

	bso::sSHuge VGetSHuge_(
		fdr::rRDriver &Driver,
		bso::sSHuge Min,
		bso::sSHuge Max,
		bso::sBool *IsError );

	void VPutUHuge_(
		bso::sUHuge UHuge,
		flw::oflow__ &Flow );

	void VPutUHuge_(
		bso::sUHuge UHuge,
		fdr::rWDriver &Driver );

	void VPutSHuge_(
		bso::sSHuge SHuge,
		flw::oflow__ &Flow );

	void VPutSHuge_(
		bso::sSHuge SHuge,
		fdr::rWDriver &Driver );

# ifdef CPE_S_DARWIN
	inline bso::size__ VGet(
		flw::iflow__ &Flow,
		bso::size__ &Size )
	{
		return Size = VGetUHuge_(Flow, BSO_SIZE_MAX, NULL);
	}

	inline void VPut(
		bso::size__ Size,
		flw::oflow__ &Flow )
	{
		VPutUHuge_(Size, Flow);
	}
# endif

# define DTFPTB__M( bitness, umax, smin, smax )\
	inline bso::u##bitness##__ VGet(\
		flw::rRFlow &Flow,\
		bso::u##bitness##__ &Int,\
		bso::sBool *IsError = NULL)\
	{\
		return Int = (bso::u##bitness##__)VGetUHuge_( Flow, umax, IsError );\
	}\
	inline bso::u##bitness##__ VGet(\
		fdr::rRDriver &Driver,\
		bso::u##bitness##__ &Int,\
		bso::sBool *IsError = NULL)\
	{\
		return Int = (bso::u##bitness##__)VGetUHuge_( Driver, umax, IsError );\
	}\
	inline bso::s##bitness##__ VGet(\
		flw::rRFlow &Flow,\
		bso::s##bitness##__ &Int,\
		bso::sBool *IsError = NULL)\
	{\
		return Int = (bso::s##bitness##__)VGetSHuge_( Flow, smin, smax, IsError );\
	}\
	inline bso::s##bitness##__ VGet(\
		fdr::rRDriver &Driver,\
		bso::s##bitness##__ &Int,\
		bso::sBool *IsError = NULL)\
	{\
		return Int = (bso::s##bitness##__)VGetSHuge_( Driver, smin, smax, IsError );\
	}\
	inline void VPut(\
		bso::u##bitness##__ Int,\
		flw::rWFlow &Flow )\
	{\
		VPutUHuge_( Int, Flow );\
	}\
	inline void VPut(\
		bso::u##bitness##__ Int,\
		fdr::rWDriver &Driver )\
	{\
		VPutUHuge_( Int, Driver );\
	}\
	inline void VPut(\
		bso::s##bitness##__ Int,\
		flw::rWFlow &Flow )\
	{\
		VPutSHuge_( Int, Flow );\
	}\
	inline void VPut(\
		bso::s##bitness##__ Int,\
		fdr::rWDriver &Driver )\
	{\
		VPutSHuge_( Int, Driver );\
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
