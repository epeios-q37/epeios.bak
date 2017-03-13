/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define MSCMDF__COMPILATION

#include "mscmdf.h"

using namespace mscmdf;

#define HEADER_CHUNK_HEAD "MThd\x00\x00\x00\x06"
#define HEADER_CHUNK_HEAD_SIZE	( sizeof( HEADER_CHUNK_HEAD ) - 1 )

#define TRACK_CHUNK_HEAD "MTrk"
#define TRACK_CHUNK_HEAD_SIZE	( sizeof( TRACK_CHUNK_HEAD ) - 1 )

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

bso::bool__ mscmdf::GetHeaderChunk(
	flw::iflow__ &Flow,
	header_chunk__ &HeaderChunk,
	err::handling__ ErrHandling )
{
	flw::sByte Buffer[HEADER_CHUNK_HEAD_SIZE];

	Flow.Read( HEADER_CHUNK_HEAD_SIZE, Buffer );

	if ( memcmp( Buffer, HEADER_CHUNK_HEAD, HEADER_CHUNK_HEAD_SIZE ) )
		switch ( ErrHandling ) {
		case err::hThrowException:
			qRFwk();
			break;
		case err::hUserDefined:
			return false;
			break;
		default:
			qRFwk();
			break;
		}

	HeaderChunk.SMFType = Flow.Get() << 8 | Flow.Get();

	HeaderChunk.TrackChunkAmount = Flow.Get() << 8 | Flow.Get();

	HeaderChunk.DeltaTimeTicks = Flow.Get() << 8 | Flow.Get();

	return true;
}

static void Put_(
	bso::u16__ Value,
	flw::oflow__ &Flow )
{
	Flow.Put( Value >> 8 );
	Flow.Put( Value & 0xff );
}

void mscmdf::PutHeaderChunk(
	smf_type__ SMFType,
	track_chunk_amount__ TrackChunkAmount,
	delta_time_ticks__ DeltaTimeTicks,
	flw::oflow__ &OFlow )
{
	OFlow.Write( HEADER_CHUNK_HEAD, HEADER_CHUNK_HEAD_SIZE );

	Put_( SMFType, OFlow );
	Put_( TrackChunkAmount, OFlow );
	Put_( DeltaTimeTicks, OFlow );
}

static inline bso::bool__ HandleDeltaTimeTicks_(
	flw::iflow__ &IFlow,
	delta_time_ticks__ &DeltaTimeTicks )
{
	flw::sByte Datum = IFlow.Get();

	DeltaTimeTicks = ( DeltaTimeTicks << 8 ) | ( Datum & 0x7f );

	return ( Datum & 0x80 ) != 0;
}

track_chunk_size__  mscmdf::GetTrackChunkSize(
	flw::iflow__ &IFlow,
	err::handling__ ErrHandling )
{
	flw::sByte Buffer[TRACK_CHUNK_HEAD_SIZE];

	IFlow.Read( TRACK_CHUNK_HEAD_SIZE, Buffer );

	if ( memcmp( Buffer, TRACK_CHUNK_HEAD, TRACK_CHUNK_HEAD_SIZE ) )
		switch ( ErrHandling ) {
		case err::hThrowException:
			qRFwk();
			break;
		case err::hUserDefined:
			return 0;	// I suppose that no track chunk can have a size of 0.
			break;
		default:
			qRFwk();
			break;
		}

	return ( IFlow.Get() << 24 ) | ( IFlow.Get() << 16 )
		                         | ( IFlow.Get() << 8 ) | IFlow.Get();

}

void mscmdf::PutTrackChunkHeader(
	track_chunk_size__ Size,
	flw::oflow__ &OFlow )
{
	OFlow.Write( TRACK_CHUNK_HEAD, TRACK_CHUNK_HEAD_SIZE );

	OFlow.Put( (flw::sByte)( Size >> 24 ) );
	OFlow.Put( (flw::sByte)( ( Size >> 16 ) & 0xff ) );
	OFlow.Put( (flw::sByte)( ( Size >> 8 ) & 0xff ) );
	OFlow.Put( (flw::sByte)( Size & 0xff ) );
}

