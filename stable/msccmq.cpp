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

#define MSCCMQ__COMPILATION

#include "msccmq.h"

#include "bch.h"

using namespace msccmq;
using namespace mscmld;
using namespace mscmdm;

static pitch_octave__ GetOctave_( bso::ubyte__ Absolute )
{
	return Absolute / 12;
}

static pitch_name__ GetName_(
	bso::ubyte__ Absolute,
	signature_key__ Key)
{
	switch ( Absolute % 12 ) {
	case 0:
		if ( Key >= 7 )
			return pnB;
		else
			return pnC;
		break;
	case 1:
		if ( Key <= -4 )
			return pnD;
		else
			return pnC;
		break;
	case 2:
		return pnD;
		break;
	case 3:
		if ( Key <= -2 )
			return pnE;
		else
			return pnD;
		break;
	case 4:
		if ( Key <= -7 )
			return pnF;
		else
			return pnE;
		break;
	case 5:
		if ( Key >= 6 )
			return pnE;
		else
			return pnF;
		break;
	case 6:
		if ( Key <= -5 )
			return pnG;
		else
			return pnF;
		break;
	case 7:
		return pnG;
		break;
	case 8:
		if ( Key <= -3 )
			return pnA;
		else
			return pnG;
		break;
	case 9:
		return pnA;
		break;
	case 10:
		if ( Key <= -1 )
			return pnB;
		else
			return pnA;
		break;
	case 11:
		if ( Key <= -6 )
			return pnC;
		else
			return pnB;
		break;
	default:
		ERRc();
		break;
	}

	ERRc();

	return pn_Undefined;	// To avoid a warning.
}

pitch_accidental__ GetAccidental_(
	bso::ubyte__ Absolute,
	signature_key__ Key )
{
	switch ( Absolute %12 ) {
	case 0:
		if ( Key >= 7 )
			return paSharp;
		else
			return paNatural;
		break;
	case 1:
		if ( Key <= -4 )
			return paFlat;
		else
			return paSharp;
		break;
	case 2:
		return paNatural;
		break;
	case 3:
		if ( Key <= -2 )
			return paFlat;
		else
			return paSharp;
		break;
	case 4:
		if ( Key <= -7 )
			return paFlat;
		else
			return paNatural;
		break;
	case 5:
		if ( Key >= 6 )
			return paSharp;
		else
			return paNatural;
		break;
	case 6:
		if ( Key <= -5 )
			return paFlat;
		else
			return paSharp;
		break;
	case 7:
		return paNatural;
		break;
	case 8:
		if ( Key <= -3 )
			return paFlat;
		else
			return paSharp;
		break;
	case 9:
		return paNatural;
		break;
	case 10:
		if ( Key <= -1 )
			return paFlat;
		else
			return paSharp;
		break;
	case 11:
		if ( Key <= -6 )
			return paFlat;
		else
			return paNatural;
		break;
	default:
		ERRc();
		break;
	}

	ERRc();

	return pa_Undefined;	// To avoid a wanrning.


}

struct skip_track_callback__
: public callback__
{
protected:
	virtual bso::bool__ MSCMDMHandleEvent(
		const event_header__ &Header,
		const data_ &Data,
		origin__ Origin )
	{
		if ( ( Header.EventType == mscmdm::etMeta ) && ( Header.MetaEvent.Event == mscmdm::mtaEndOfTrack ) )
			return false;
		else
			return true;
	}
public:
	void Init( void )
	{
	}
};


struct callback
: public callback__
{
private:
	signature__ _Signature;
	data _DurationData;
	void _HandlePitch( const data_ &Data )
	{
		bso::ubyte__ Absolute = Data( Data.First() );
		Pitches.Append( pitch__( GetOctave_( Absolute ), GetName_( Absolute, _Signature.Key ), GetAccidental_( Absolute, _Signature.Key ) ) );
	}
#if 0
	void _HandleDuration( const data_ &Data )
	{
		switch ( Data( Data.First() ) % 12 ) {
		case 0:	// C
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 1;
			break;
		case 1:		//C#
			if ( _Pending == 0 )
				ERRu();
			Durations.Append( duration__(_Pending, 1 ) );
			_Pending = 0;
			break;
		case 2:		// D
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 2;
			break;
		case 3:		// D#
			if ( _Pending == 0 )
				ERRu();
			Durations.Append( duration__(_Pending, 2 ) );
			_Pending = 0;
			break;
		case 4:		// E
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 3;
			break;
		case 5:		// F
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 4;
			break;
		case 6:		// F#
			ERRl();
			break;
		case 7:		// G
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 5;
			break;
		case 8:		// G#
			ERRl();
			break;
		case 9:		// A
			if ( _Pending != 0 )
				Durations.Append( duration__(_Pending ) );
			_Pending = 6;
			break;
		case 10:	// A#
			ERRl();
			break;
		case 11:	// B
			if ( _Pending != 0 )
				Durations.Append( _Pending );
			_Pending = 7;
			break;
		default:
			ERRc();
			break;
		}
	}
#else
	void _HandleDuration( const data_ &Data )
	{
		duration__ PendingDuration;
		epeios::row__ Row = Data.First();

		while ( Row != NONE ) {
			if ( isspace( Data( Row ) ) ) {
				if ( PendingDuration.IsValid() ) {
					Durations.Append( PendingDuration );
					PendingDuration = duration__();
				}
			} else if ( isdigit( Data( Row ) ) ) {
				if ( PendingDuration.IsValid() ) {
					Durations.Append( PendingDuration );
					PendingDuration = duration__();
				}
				if ( Data( Row ) == 0 )
					ERRu();

				PendingDuration = duration__( Data( Row ) - '0' );
			} else if ( Data( Row ) == '.' ) {
				if ( !PendingDuration.IsValid() )
					ERRu();

				PendingDuration.Modifier++;
			} else if ( Data( Row ) == '~' ) {
				if ( !PendingDuration.IsValid() )
					ERRu();

				if ( PendingDuration.TiedToNext )
					ERRu();

				PendingDuration.TiedToNext = true;
			} else if ( Data( Row ) == 'r' ) {
				if ( PendingDuration.IsValid() ) {
					Durations.Append( PendingDuration );
					PendingDuration = duration__();
				}
				prow__ PRow = *Durations.Last();

				if ( PRow == NONE )
					PRow = Pitches.First();
				else
					PRow = Pitches.Next( PRow );

				if ( PRow == NONE ) {
					ERRu();
				} else 
					Pitches.Store( pitch__( pnRest, 0 ), PRow );
			} else if ( Data( Row ) == 'R' ) {
				if ( PendingDuration.IsValid() ) {
					Durations.Append( PendingDuration );
					PendingDuration = duration__();
				}
				Pitches.Append( pitch__( pnRest, 0 ) );
			} else
				ERRu();

			Row = Data.Next( Row );
		}

		if ( PendingDuration.IsValid() )
			Durations.Append( PendingDuration );
	}
#endif
protected:
	virtual bso::bool__ MSCMDMHandleEvent(
		const event_header__ &Header,
		const data_ &Data,
		origin__ Origin )
	{
		if ( ( Header.EventType == etMIDI ) && ( Header.MIDIEvent.Event == midNoteOn ) && ( Data( Data.Last() ) != 0 ) ) {
			_HandlePitch( Data );
			Signatures.Append( _Signature );
		} else if ( Header.EventType == mscmdm::etMeta ) {
			switch ( Header.MetaEvent.Event ) {
			case mscmdm::mtaEndOfTrack:
				_HandleDuration( _DurationData );
				return false;
				break;
			case mscmdm::mtaKeySignature:
				_Signature = signature__( Data( Data.First() ), _Signature.Time );
				break;
			case mscmdm::mtaTimeSignature:
				_Signature = signature__( _Signature.Key, signature_time__( Data( Data.First() ), 1 << Data( Data.First( 1 ) ) ) );
				break;
			case mscmdm::mtaLyric:
				_DurationData.Append( Data );
				break;
			}
		}

		return true;
	}
public:
	pitches Pitches;
	durations Durations;
	signatures Signatures;
	void Init( void )
	{
		Pitches.Init();
		Durations.Init();
		Signatures.Init();

		_DurationData.Init();
		_Signature = signature__( signature_time__( 4, 4 ), 0 );
	}
};

void msccmq::Parse(
	flw::iflow__ &IFlow,
	melody_ &Melody )
{
qRH
	mscmdf::header_chunk__ HeaderChunk;
	mscmdf::track_chunk_size__ TrackChunkSize;
//	skip_track_callback__ SkipTrackCallback;
	callback Callback;
qRB
	mscmdf::GetHeaderChunk( IFlow, HeaderChunk );

	if ( HeaderChunk.SMFType != 0 )
		ERRu();

	if ( HeaderChunk.TrackChunkAmount != 1 )
		ERRf();

	TrackChunkSize = mscmdf::GetTrackChunkSize( IFlow );

	Callback.Init();
	mscmdm::Parse( Callback, IFlow, oFile );

	Merge( Callback.Pitches, Callback.Durations, Callback.Signatures, Melody );
qRR
qRT
qRE
}
