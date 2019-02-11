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

#define DTFPTB__COMPILATION

#include "dtfptb.h"
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
	bso::byte__ *Pointer = Buffer;

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

				*(Pointer++) = (bso::byte__)( ( Size & ( 255 << 24 ) ) >> 24 );
			}

			*(Pointer++) = (bso::byte__)( ( Size & ( 255 << 16 ) ) >> 16 );
		}

		*(Pointer++) = (bso::byte__)( ( Size & ( 255 << 8 ) ) >> 8 );
	}

	*(Pointer++) = (bso::byte__)( Size & 255 );
}

#if 0	// Version originale.
static inline bso::u32__ Put_(
	bso::u32__ Value,
	flw::oflow__ &Flow )
{
	bso::byte__ Buffer = Value & 0x7F;

	Value >>= 7;

	if ( Value != 0 )
		Buffer |= 0x80;

	Flow.Put( Buffer );

	return Value;
}

void dtfptb::PutSize(
	bso::ulong__ Size,
	flw::oflow__ &Flow )
{
	if ( Size >= M1 )
	{
		Flow.Put( (flw::byte__)M1 );
		Size -= M1;

		if ( Size >= M2 )
		{
			Flow.Put( (flw::byte__)M1 );
			Flow.Put( (flw::byte__)M1 );
			Size -= M2;

			if ( Size >= M3 )
			{
				Flow.Put( (flw::byte__)M1 );
				Flow.Put( (flw::byte__)M1 );
				Flow.Put( (flw::byte__)M1 );

				Size -= M3;

				Flow.Put( (flw::byte__)( ( Size & ( 255 << 24 ) ) >> 24 ) );
			}
			Flow.Put( (flw::byte__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
		}
		Flow.Put( (flw::byte__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
	}
	Flow.Put( (flw::byte__)( Size & 255 ) );
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
		flw::byte__ Data[4];

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

bso::u32__ dtfptb::OldGetSize( fdr::rRDriver &Driver )
{
	bso::sU32 Value = 0;
qRH;
	flw::rDressedRFlow<> Flow;
qRB;
	Flow.Init( Driver );

	Value = OldGetSize( Flow );
qRR;
qRT;
qRE;
	return Value;
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

static bso::byte__ *GetInt_(
	flw::iflow__ &Flow,
	bso::byte__ *DInt )
{
	bso::u8__ Counter = 0;

	while ( ( Counter < BSO_DINT_SIZE_MAX ) && ( ( DInt[Counter] = Flow.Get() ) & 0x80 ) )
		Counter++;

	if ( Counter >= BSO_DINT_SIZE_MAX )
		qRFwk();

	return DInt;
}

static bso::byte__ *GetInt_(
	fdr::rRDriver &Driver,
	bso::byte__ *DInt )
{
qRH;
	flw::rDressedRFlow<> Flow;
qRB;
	Flow.Init( Driver );

	GetInt_( Flow, DInt );
qRR;
qRT;
qRE;
	return DInt;
}

static void PutInt_(
	const bso::xint__ &XInt,
	flw::oflow__ &Flow )
{
	Flow.Write( XInt.DSizeBuffer(), XInt.BufferSize() );
}

static void PutInt_(
	const bso::xint__ &XInt,
	fdr::rWDriver &Driver )
{
qRH;
	flw::rDressedWFlow<> Flow;
qRB;
	Flow.Init( Driver );

	PutInt_( XInt, Flow );
qRR;
qRT;
qRE;
}

#define M( s )	Flow.Put( (flw::byte__)( Int >> ( s * 8 ) ) )

void dtfptb::_FPutInt(
	bso::int__ Int,
	_length__ Length,
	flw::oflow__ &Flow )
{
	switch ( Length ) {
	case 8:
#ifdef CPE_F_64BITS
		M( 7 );
		M( 6 );
		M( 5 );
		M( 4 );
#else
		qRFwk();
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
		qRFwk();
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
#ifdef CPE_F_64BITS
		M( 7 );
		M( 6 );
		M( 5 );
		M( 4 );
#else
		qRFwk();
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
		qRFwk();
		break;
	}

	return Int;
}

#undef M

bso::int__ dtfptb::_FGetInt(
	fdr::rRDriver &Driver,
	_length__ Length )
{
	bso::sSInt Int = 0;
qRH;
	flw::rDressedRFlow<> Flow;
qRB;
	Flow.Init( Driver );

	Int = _FGetInt( Flow, Length );
qRR;
qRT;
qRE;
	return Int;
}

template <typename fd> static bso::sUBig VGetUBig_(
	fd &FD,
	bso::sUBig Max )
{
	bso::byte__ DInt[BSO_DINT_SIZE_MAX];
	bso::sUBig Value = bso::ConvertToUBig( GetInt_( FD, DInt ) );

	if ( Value > Max )
		qRFwk();

	return Value;
}

bso::sUBig dtfptb::_VGetUBig(
	flw::iflow__ &Flow,
	bso::sUBig Max )
{
	return VGetUBig_( Flow, Max );
}

bso::sUBig dtfptb::_VGetUBig(
	fdr::rRDriver &Driver,
	bso::sUBig Max )
{
	return VGetUBig_( Driver, Max );
}

template <typename fd> bso::sSBig VGetSBig_(
	fd &FD,
	bso::sSBig Min,
	bso::sSBig Max )
{
	bso::byte__ DInt[BSO_DINT_SIZE_MAX];
	bso::sSBig Value = bso::ConvertToSBig( GetInt_( FD, DInt ) );

	if ( Value < Min )
		qRFwk();

	if ( Value > Max )
		qRFwk();

	return Value;
}

bso::sSBig dtfptb::_VGetSBig(
	flw::iflow__ &Flow,
	bso::sSBig Min,
	bso::sSBig Max )
{
	return VGetSBig_( Flow, Min, Max );
}

bso::sSBig dtfptb::_VGetSBig(
	fdr::rRDriver &Driver,
	bso::sSBig Min,
	bso::sSBig Max )
{
	return VGetSBig_( Driver, Min, Max );
}

template <typename fd> static void VPutUBig_(
	bso::sUBig UBig,
	fd &FD )
{
	bso::xint__ XInt;

	PutInt_( bso::ConvertToDInt( UBig, XInt ), FD );
}

void dtfptb::_VPutUBig(
	bso::sUBig UBig,
	flw::oflow__ &Flow )
{
	VPutUBig_( UBig, Flow );
}

template <typename fd> void VPutSBig_(
	bso::sSBig SBig,
	fd &FD )
{
	bso::xint__ XInt;

	PutInt_( bso::ConvertToDInt( SBig, XInt ), FD );
}

void dtfptb::_VPutSBig(
	bso::sSBig SBig,
	flw::oflow__ &Flow )
{
	VPutSBig_( SBig, Flow );
}

Q37_GCTOR( dtfptb )
{
	// Pour des raisons de simplifications, on 'cast' parfois autoritairement un 'bso::size__' en 'bso::int__'.
	if ( sizeof( bso::size__ ) != sizeof( bso::int__ ) )
		qRChk();
}
