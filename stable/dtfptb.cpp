/*
	'dtfptb' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dtfptb' header file ('dtfptb.h').
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

#define DTFPTB__COMPILATION

#include "dtfptb.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"

using namespace dtfptb;

#define M1	DTFPTB_L1
#define M2	DTFPTB_L2
#define M3	DTFPTB_L3


void dtfptb::OldPutSize(
	bso::u32__ Size,
	size_buffer__ &Buffer )
{
	bso::raw__ *Pointer = Buffer;

	if ( Size >= M1 )
	{
		*(Pointer++) = M1;
		Size -= M1;

		if ( Size >= M2 )
		{
			*(Pointer++) = M1;
			*(Pointer++) = M1;
			Size -= M2;

			if ( Size >= M3 )
			{
				*(Pointer++) = M1;
				*(Pointer++) = M1;
				*(Pointer++) = M1;

				Size -= M3;

				*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 24 ) ) >> 24 );
			}

			*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 16 ) ) >> 16 );
		}

		*(Pointer++) = (bso::raw__)( ( Size & ( 255 << 8 ) ) >> 8 );
	}

	*(Pointer++) = (bso::raw__)( Size & 255 );
}


static inline bso::u32__ Put_(
	bso::u32__ Value,
	flw::oflow__ &Flow )
{
	bso::raw__ Buffer = Value & 0x7F;

	Value >>= 7;

	if ( Value != 0 )
		Buffer |= 0x80;

	Flow.Put( Buffer );

	return Value;
}

#if 0	// Version originale.
void dtfptb::PutSize(
	bso::ulong__ Size,
	flw::oflow__ &Flow )
{
	if ( Size >= M1 )
	{
		Flow.Put( (flw::datum__)M1 );
		Size -= M1;

		if ( Size >= M2 )
		{
			Flow.Put( (flw::datum__)M1 );
			Flow.Put( (flw::datum__)M1 );
			Size -= M2;

			if ( Size >= M3 )
			{
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );

				Size -= M3;

				Flow.Put( (flw::datum__)( ( Size & ( 255 << 24 ) ) >> 24 ) );
			}
			Flow.Put( (flw::datum__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
		}
		Flow.Put( (flw::datum__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
	}
	Flow.Put( (flw::datum__)( Size & 255 ) );
}
#else
void dtfptb::OldPutSize(
	bso::u32__ Size,
	flw::oflow__ &Flow )
{
	size_buffer__ Buffer;

	OldPutSize( Size, Buffer );

	Flow.Write( Buffer, OldGetSizeLength( Size ) );
}

#endif

bso::u32__ dtfptb::OldGetSize( flw::iflow__ &IFlow )
{
	bso::u32__ Size = (bso::u8__)IFlow.Get();

	if ( Size == M1 )
	{
		flw::datum__ Data[4];

		IFlow.Read( 2, Data );

		Size += ( Data[0] << 8 ) | Data[1];

		if ( Size == ( M1 + M2 ) )
		{
			IFlow.Read( 3, Data );

			Size += ( Data[0] << 16 ) | ( Data[1] << 8 ) | Data[2];

			if ( Size == ( M1 + M2 + M3 ) ) {

				IFlow.Read( 4, Data );

				Size += ( Data[0] << 24 ) | ( Data[1] << 16 ) | ( Data[2] << 8 ) | Data[3];
			}
		}
	}

	return Size;
}

bso::u32__ dtfptb::OldGetSize( const size_buffer__ &Buffer )
{
	bso::u32__ Size = Buffer[0];

	if ( Size == M1 )
	{
		Size += ( Buffer[1] << 8 ) | Buffer[2];

		if ( Size == ( M1 + M2 ) )
		{
			Size += ( Buffer[3] << 16 ) | ( Buffer[4] << 8 ) | Buffer[5];

			if ( Size == ( M1 + M2 + M3 ) ) {

				Size += ( Buffer[6] << 24 ) | ( Buffer[7] << 16 ) | ( Buffer[8] << 8 ) | Buffer[9];
			}
		}
	}

	return Size;
}

static bso::raw__ *_GetInt(
	flw::iflow__ &Flow,
	bso::raw__ *DInt )
{
	bso::u8__ Counter = 0;

	while ( ( Counter < BSO_DINT_SIZE_MAX ) && ( ( DInt[Counter] = Flow.Get() ) & 0x80 ) )
		Counter++;

	if ( Counter >= BSO_DINT_SIZE_MAX )
		ERRDta();

	return DInt;
}

static void _PutInt(
	const bso::xint__ &XInt,
	flw::oflow__ &Flow )
{
	Flow.Write( XInt.DSizeBuffer(), XInt.BufferSize() );
}

#define M( s )	Flow.Put( (flw::datum__)( Int >> ( s * 8 ) ) )

void dtfptb::_FPutInt(
	bso::int__ Int,
	_length__ Length,
	flw::oflow__ &Flow )
{
	switch ( Length ) {
	case 8:
#ifdef CPE_INT64
		M( 7 );
		M( 6 );
		M( 5 );
		M( 4 );
#else
		ERRVct();
		break;
#endif
	case 4:
		M( 3 );
		M( 2 );
	case 2:
		M( 1 );
	case 1:
		M( 0 );
		break;
	default:
		ERRPrm();
		break;
	}
}
#undef M

#define M( s )	Int += (bso::int__)( Flow.Get() ) << ( s * 8 )

bso::int__ dtfptb::_FGetInt(
	flw::iflow__ &Flow,
	_length__ Length )
{
	bso::int__ Int = 0;

	switch( Length ) {
	case 8:
#ifdef CPE_INT64
		M( 7 );
		M( 6 );
		M( 5 );
		M( 4 );
#else
		ERRVct();
		break;
#endif
	case 4:
		M( 3 );
		M( 2 );
	case 2:
		M( 1 );
	case 1:
		M( 0 );
		break;
	default:
		ERRPrm();
		break;
	}

	return Int;
}

#undef M

bso::uint__ dtfptb::_VGetUInt(
	flw::iflow__ &Flow,
	bso::uint__ Max )
{
	bso::raw__ DInt[BSO_DINT_SIZE_MAX];
	bso::uint__ Value = bso::ConvertToUInt( _GetInt( Flow, DInt ) );

	if ( Value > Max )
		ERRDta();

	return Value;
}

bso::sint__ dtfptb::_VGetSInt(
	flw::iflow__ &Flow,
	bso::sint__ Min,
	bso::sint__ Max )
{
	bso::raw__ DInt[BSO_DINT_SIZE_MAX];
	bso::sint__ Value = bso::ConvertToSInt( _GetInt( Flow, DInt ) );

	if ( Value < Min )
		ERRDta();

	if ( Value > Max )
		ERRDta();

	return Value;
}

void dtfptb::_VPutUInt(
	bso::uint__ UInt,
	flw::oflow__ &Flow )
{
	bso::xint__ XInt;

	_PutInt( bso::ConvertToDInt( UInt, XInt ), Flow );
}

void dtfptb::_VPutSInt(
	bso::sint__ SInt,
	flw::oflow__ &Flow )
{
	bso::xint__ XInt;

	_PutInt( bso::ConvertToDInt( SInt, XInt ), Flow );
}

Q37_GCTOR( dtfptb )
{
	// Pour des raisons de simplifications, on 'cast' parfois autoritairement un 'bso::size__' en 'bso::int__'.
	if ( sizeof( bso::size__ ) != sizeof( bso::int__ ) )
		ERRChk();
}
