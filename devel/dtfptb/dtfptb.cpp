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

#define DTFPTB_COMPILATION_

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

	Flow.reset(false); // To avoid a 'Dismiss(…)'.
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

static bso::sBool GetInt_(
	flw::iflow__ &Flow,
	bso::byte__ *DInt )
{
	bso::u8__ Counter = 0;
	bso::sBool IsError = false;

	while ( ( Counter < BSO_DINT_SIZE_MAX ) && ( ( DInt[Counter] = Flow.Get( &IsError ) ) & 0x80 ) && !IsError ) {
		Counter++;
	}

	if ( Counter >= BSO_DINT_SIZE_MAX )
		qRFwk();

	return !IsError;
}

bso::sBool GetInt_(
	fdr::rRDriver &Driver,
	bso::byte__ *DInt )
{
	bso::sBool Success = false;
qRH;
	flw::rDressedRFlow<> Flow;
qRB;
	Flow.Init( Driver );

	Success = GetInt_( Flow, DInt );

	Flow.reset(false);  // To avoid the 'Dismiss(…)'.
qRR;
qRT;
qRE;
	return Success;
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

	PutInt_(XInt, Flow);

	Flow.DumpCache();

	Flow.reset(false);  // To avoid the 'Commit(…)'.
qRR;
qRT;
qRE;
}

#define M( s )	Flow.Put( (flw::byte__)( Int >> ( s * 8 ) ) )

void dtfptb::FPutInt_(
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

void dtfptb::FPutInt_(
    bso::int__ Int,
	_length__ Length,
	fdr::rWDriver &Driver )
{
qRH;
	flw::rDressedWFlow<> Flow;
qRB;
	Flow.Init( Driver );

	FPutInt_(Int, Length, Flow);

	Flow.DumpCache();

	Flow.reset(false);  // To avoid the 'Commit(…)'.
qRR;
qRT;
qRE;
}

#define M( s )	Int += (bso::int__)( Flow.Get( IsError ) ) << ( s * 8 )

bso::int__ dtfptb::FGetInt_(
	flw::iflow__ &Flow,
	_length__ Length,
	bso::sBool *IsError )
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

bso::int__ dtfptb::FGetInt_(
	fdr::rRDriver &Driver,
	_length__ Length,
	bso::sBool *IsError )
{
	bso::sSInt Int = 0;
qRH;
	flw::rDressedRFlow<> Flow;
qRB;
	Flow.Init( Driver );

	Int = FGetInt_( Flow, Length, IsError );

	Flow.reset(false);  // To avoid the 'Dismiss()'.
qRR;
qRT;
qRE;
	return Int;
}

template <typename fd> static bso::sUHuge TemplatedVGetUHuge_(
	fd &FD,
	bso::sUHuge Max,
	bso::sBool *IsError )
{
	bso::byte__ DInt[BSO_DINT_SIZE_MAX];

	if ( !GetInt_( FD, DInt ) )	{
		if ( IsError == NULL )
			qRFwk();
		else {
			*IsError = true;
			return 0;
		}
	}

	bso::sUHuge Value = bso::ConvertToUHuge( DInt );

	if ( Value > Max )
		qRFwk();

	return Value;
}

bso::sUHuge dtfptb::VGetUHuge_(
	flw::iflow__ &Flow,
	bso::sUHuge Max,
	bso::sBool *IsError )
{
	return TemplatedVGetUHuge_( Flow, Max, IsError );
}

bso::sUHuge dtfptb::VGetUHuge_(
	fdr::rRDriver &Driver,
	bso::sUHuge Max,
	bso::sBool *IsError )
{
	return TemplatedVGetUHuge_( Driver, Max, IsError );
}

template <typename fd> bso::sSHuge TemplatedVGetSHuge_(
	fd &FD,
	bso::sSHuge Min,
	bso::sSHuge Max,
	bso::sBool *IsError )
{
	bso::byte__ DInt[BSO_DINT_SIZE_MAX];

	if ( !GetInt_( FD, DInt ) ) {
		if ( IsError == NULL )
			qRFwk();
		else {
			*IsError = true;
			return 0;
		}
	}

	bso::sSHuge Value = bso::ConvertToSHuge( DInt );

	if ( Value < Min )
		qRFwk();

	if ( Value > Max )
		qRFwk();

	return Value;
}

bso::sSHuge dtfptb::VGetSHuge_(
	flw::iflow__ &Flow,
	bso::sSHuge Min,
	bso::sSHuge Max,
	bso::sBool *IsError )
{
	return TemplatedVGetSHuge_( Flow, Min, Max, IsError );
}

bso::sSHuge dtfptb::VGetSHuge_(
	fdr::rRDriver &Driver,
	bso::sSHuge Min,
	bso::sSHuge Max,
	bso::sBool *IsError )
{
	return TemplatedVGetSHuge_( Driver, Min, Max, IsError );
}

template <typename fd> static void VPutUHuge_(
	bso::sUHuge UHuge,
	fd &FD )
{
	bso::xint__ XInt;

	PutInt_( bso::ConvertToDInt( UHuge, XInt ), FD );
}

void dtfptb::VPutUHuge_(
	bso::sUHuge UHuge,
	fdr::rWDriver &Driver )
{
	::VPutUHuge_( UHuge, Driver );
}

void dtfptb::VPutUHuge_(
	bso::sUHuge UHuge,
	flw::oflow__ &Flow )
{
	::VPutUHuge_( UHuge, Flow );
}

template <typename fd> void VPutSHuge_(
	bso::sSHuge SHuge,
	fd &FD )
{
	bso::xint__ XInt;

	PutInt_( bso::ConvertToDInt( SHuge, XInt ), FD );
}

void dtfptb::VPutSHuge_(
	bso::sSHuge SHuge,
	fdr::rWDriver &Driver)
{
	::VPutSHuge_( SHuge, Driver );
}

void dtfptb::VPutSHuge_(
	bso::sSHuge SHuge,
	flw::oflow__ &Flow )
{
	::VPutSHuge_( SHuge, Flow );
}

Q37_GCTOR( dtfptb )
{
	// Pour des raisons de simplifications, on 'cast' parfois autoritairement un 'bso::size__' en 'bso::int__'.
	if ( sizeof( bso::size__ ) != sizeof( bso::int__ ) )
		qRChk();
}
