/*
	Header for the 'dtfptb' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

//	$Id: dtfptb.h,v 1.25 2013/04/13 16:21:16 csimon Exp $

#ifndef DTFPTB__INC
#define DTFPTB__INC

#define DTFPTB_NAME		"DTFPTB"

#define	DTFPTB_VERSION	"$Revision: 1.25 $"

#define DTFPTB_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &DTFPTBTutor;

#if defined( E_DEBUG ) && !defined( DTFPTB_NODBG )
#define DTFPTB_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.25 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2013/04/13 16:21:16 $

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Data TransFert PorTaBle 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "dtfbsc.h"

namespace dtfptb {
	using namespace dtfbsc;

	typedef bso::raw__ size_buffer__[10];

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

	bso::u32__ OldGetSize( const size_buffer__ &Buffer );

	typedef bso::u8__ _length__;

	void _FPutInt(
		bso::int__ Int,
		_length__ Length,
		flw::oflow__ &Flow );

	bso::int__ _FGetInt(
		flw::iflow__ &Flow,
		_length__ Length );

	template <typename i> inline void FPut(
		i Int,
		flw::oflow__ &Flow )
	{
		_FPutInt( Int, sizeof( Int ), Flow );
	}

	template <typename i> inline i FGet(
		flw::iflow__ &Flow,
		i &Int )
	{
		return Int = (i)_FGetInt( Flow, sizeof( Int ) );
	}

# ifdef CPE_XCODE
	inline void FPut(
		bso::size__ Size,
		flw::oflow__ &Flow )
	{
		_FPutInt( Size, sizeof( Size ), Flow );
	}

	inline bso::size__ FGet(
		flw::iflow__ &Flow,
		bso::size__ &Size )
	{
		return Size = _FGetInt( Flow, sizeof( Size ) );
	}
# endif

	bso::uint__ _VGetUInt(
		flw::iflow__ &Flow,
		bso::uint__ Max );

	bso::sint__ _VGetSInt(
		flw::iflow__ &Flow,
		bso::sint__ Min,
		bso::sint__ Max );

	void _VPutUInt(
		bso::uint__ UInt,
		flw::oflow__ &Flow );

	void _VPutSInt(
		bso::sint__ SInt,
		flw::oflow__ &Flow );

# ifdef CPE_XCODE
	inline bso::size__ VGet(
		flw::iflow__ &Flow,
		bso::size__ &Size )
	{
		return Size = _VGetUInt( Flow, BSO_SIZE_MAX );
	}

	inline void VPut(
		bso::size__ Size,
		flw::oflow__ &Flow )
	{
		_VPutUInt( Size, Flow );
	}
# endif

# define DTFPTB__M( bitness, umax, smin, smax )\
	inline bso::u##bitness##__ VGet(\
		flw::iflow__ &Flow,\
		bso::u##bitness##__ &Int )\
	{\
		return Int = (bso::u##bitness##__)_VGetUInt( Flow, umax );\
	}\
	inline bso::s##bitness##__ VGet(\
		flw::iflow__ &Flow,\
		bso::s##bitness##__ &Int )\
	{\
		return Int = (bso::s##bitness##__)_VGetSInt( Flow, smin, smax );\
	}\
	inline void VPut(\
		bso::u##bitness##__ Int,\
		flw::oflow__ &Flow )\
	{\
		_VPutUInt( Int, Flow );\
	}\
	inline void VPut(\
		bso::s##bitness##__ Int,\
		flw::oflow__ &Flow )\
	{\
		_VPutSInt( Int, Flow );\
	}


# ifdef CPE_X64
DTFPTB__M( 64, BSO_U64_MAX, BSO_S64_MIN, BSO_S64_MAX )
# endif
DTFPTB__M( 32, BSO_U32_MAX, BSO_S32_MIN, BSO_S32_MAX )
DTFPTB__M( 16, BSO_U16_MAX, BSO_S16_MIN, BSO_S16_MAX )

// Pour un octet, on envoit/reçoit l'octet tel quel ; pas besoin de conversion.
// DTFPTB__M( 8, BSO_U8_MAX, BSO_S8_MIN, BSO_S8_MAX )
	inline void VPut(
		bso::u8__ Int,
		flw::oflow__ &Flow )
	{
		FPut( Int, Flow );
	}

	inline bso::u8__ VGet(
		flw::iflow__ &Flow,
		bso::u8__ &Int )
	{
		return FGet( Flow, Int );
	}


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
