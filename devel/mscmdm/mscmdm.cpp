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

#define MSCMDM_COMPILATION_

#include "mscmdm.h"

#include "mscmdf.h"

using namespace mscmdm;

static const char *MIDIEventLabels_[mid_amount] = {
	"NoteOff",
	"NoteOn",
	"KeyAftertouch",
	"ControlChange",
	"ProgramChange",
	"ChannelAftertouch",
	"PitchWheelChange",
};

static bso::sU8 MIDIEventDataSize_[mid_amount] = {
	2,
	2,
	2,
	2,
	1,
	1,
	2
};

static const char *SystemEventLabels_[sys_amount] = {
	"Exclusive",
	"TimingClock",
	"Start",
	"Stop",
	"Continue",
	"ActiveSensing",
	"Reset",
	"SongPositionPointer",
	"SongSelect",
	"EndOfExclusive",
};

static const char *MetaEventLabels_[mta_amount] = {
	"SequenceNumber",
	"Text",
	"Copyright",
	"TrackName",
	"InstrumentName",
	"Lyric",
	"Marker",
	"CuePoint",
	"ChannelPrefix",
	"PortPrefix",
	"EndOfTrack",
	"TempoSet",
	"SMPTE",
	"TimeSignature",
	"KeySignature",
	"Sequencer",
	"<unknown>"
};

static const char *EventTypeLabels_[et_amount] = {
	"MIDI",
	"System",
	"Meta",
};

const char *mscmdm::GetMIDIEventLabel( eMidiEvent Event )
{
	if ( Event >= mid_amount )
		qRFwk();

	return MIDIEventLabels_[Event];
}

sSize mscmdm::GetMIDIEventDataSize( eMidiEvent Event )
{
	if ( Event >= mid_amount )
		qRFwk();

	return MIDIEventDataSize_[Event ];
}

const char *mscmdm::GetSystemEventLabel( eSystemEvent Event )
{
	if ( Event >= sys_amount )
		qRFwk();

	return SystemEventLabels_[Event];
}

const char *mscmdm::GetMetaEventLabel( eMetaEvent Event )
{
	if ( Event >= mta_amount )
		qRFwk();

	return MetaEventLabels_[Event];
}

const char *mscmdm::GetEventLabel( const sEventHeader &Event )
{
	switch ( Event.EventType ) {
	case etMIDI:
		return GetMIDIEventLabel( Event.MIDIEvent.Event );
		break;
	case etSystem:
		return GetSystemEventLabel( Event.SystemEvent.Event );
		break;
	case etMeta:
		return GetMetaEventLabel( Event.MetaEvent.Event );
		break;
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}


const char *mscmdm::GetEventTypeLabel( eEventType Type )
{
	if ( Type >= et_amount )
		qRFwk();

	return EventTypeLabels_[Type];
}

/*
0000 1111 1110 0000 0000 0000 0000 0000
   0    f    e    0    0    0    0    0
0000 0000 0001 1111 1100 0000 0000 0000
   0    0    1    f    c    0    0    0
0000 0000 0000 0000 0011 1111 1000 0000
   0    0    0    0    3    f    8    0
0000 0000 0000 0000 0000 0000 0111 1111
   0    0    0    0    0    0    7    f
*/

#define BASE_MASK	0x7f

#define POS1	21
#define POS2	14
#define POS3	7

#define MASK1	( BASE_MASK << POS1 )
#define MASK2	( BASE_MASK << POS2 )
#define MASK3	( BASE_MASK << POS3 )

static inline bso::sU8 GetDeltaTimeTicksSize_( sDeltaTimeTicks Ticks )
{
	bso::sU8 Size = 1;

	if ( Ticks & MASK1 )
		Size++;

	if ( Ticks & MASK2 )
		Size++;

	if ( Ticks & MASK3 )
		Size++;

	return Size;
}

void mscmdm::Encode(
	sDeltaTimeTicks Ticks,
	dData &Data )
{
	if ( Ticks & MASK1 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS1 ) & BASE_MASK ) ) );

	if ( Ticks & MASK2 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS2 ) & BASE_MASK ) ) );

	if ( Ticks & MASK3 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS3 ) & BASE_MASK ) ) );

	Data.Append( (bso::char__)( Ticks & BASE_MASK ) );
}

eMidiEvent mscmdm::DetermineMIDIEvent( flw::sByte Datum )
{
	switch( Datum & 0xf0 ) {
	case 0x80:
		return midNoteOff;
		break;
	case 0x90:
		return midNoteOn;
		break;
	case 0xa0:
		return midKeyAftertouch;
		break;
	case 0xb0:
		return midControlChange;
		break;
	case 0xc0:
		return midProgramChange;
		break;
	case 0xd0:
		return midChannelAftertouch;
		break;
	case 0xe0:
		return midPitchWheelChange;
		break;
	}

	return mid_NotAMIDIEvent;
}

static inline eSystemEvent DetermineSystemEvent_( flw::sByte Datum )
{
	switch ( Datum &0xf ) {
	case 0x0:
		return sysExclusive;
		break;
	case 0x8:
		return sysTimingClock;
		break;
	case 0xa:
		return sysStart;
		break;
	case 0xc:
		return sysStop;
		break;
	case 0xb:
		return sysContinue;
		break;
	case 0xe:
		return sysActiveSensing;
		break;
	case 0x2:
		return sysSongPositionPointer;
		break;
	case 0x3:
		return sysSongSelect;
		break;
	case 0x7:
		return sysEndOfExclusive;
		break;
	}

	return sys_NotASystemEvent;
}

static inline eMetaEvent DetermineMetaEvent_( flw::sByte Datum )
{
	switch ( Datum ) {
	case 0x00:
		return mtaSequenceNumber;
		break;
	case 0x01:
		return mtaText;
		break;
	case 0x2:
		return mtaCopyright;
		break;
	case 0x3:
		return mtaTrackName;
		break;
	case 0x4:
		return mtaInstrumentName;
		break;
	case 0x5:
		return mtaLyric;
		break;
	case 0x6:
		return mtaMarker;
		break;
	case 0x7:
		return mtaCuePoint;
		break;
	case 0x20:
		return mtaChannelPrefix;
		break;
	case 0x21:
		return mtaPortPrefix;
		break;
	case 0x2f:
		return mtaEndOfTrack;
		break;
	case 0x51:
		return mtaTempoSet;
		break;
	case 0x54:
		return mtaSMPTE;
		break;
	case 0x58:
		return mtaTimeSignature;
		break;
	case 0x59:
		return mtaKeySignature;
		break;
	case 0x7f:
		return mtaSequencer;
		break;
	default:
		return mta_Unknown;
		break;
	}

	return mta_NotAMetaEvent;	// Standadisation.
}

eEventType mscmdm::DetermineEvent(
	flw::sByte Datum,
	flw::sByte AdditionalDatum,	// For meta only.
	bso::sU8 &Event )
{
	eEventType EventType = et_Undefined;

	if ( Datum == 0xff  ) {
		EventType = etMeta;
		Event = DetermineMetaEvent_( AdditionalDatum );
	} else if ( ( Datum & 0xf0 ) == 0xf0 ) {
		EventType = etSystem;
		Event = DetermineSystemEvent_( Datum );
	} else {
		EventType = etMIDI;
		Event = DetermineMIDIEvent( Datum );
	}

	return EventType;
}

#if 0
bso::bool__ mscmdm::GetEventHeader(
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	sEventHeader &EventHeader,
	err::handling__ ErrHandling )
{
	flw::sByte Datum;

	switch ( Extraneous ) {
	case xNone:
		break;
	case xTicks:
		EventHeader.DeltaTimeTicks = GetDeltaTimeTicks( IFlow );
		break;
	default:
		ERRu();
		break;
	}

	Datum = IFlow.View();

	if ( EventHeader.EventType == etMIDI )
		if ( Datum & 0x80 ) {
			IFlow.Get();
			EventHeader.MIDIEvent.Tied = false;
		} else {
			EventHeader.MIDIEvent.Tied = true;
			return true;
		}
	else
		IFlow.Get();

	EventHeader.MIDIEvent.ChannelID = Datum & 0xf;

	PartiallyFillEventHeader( Datum, IFlow.View(), EventHeader );

	switch ( EventHeader.EventType ) {
	case etMIDI:
		break;
	case etSystem:
		break;
	case etMeta:
		IFlow.Get();
		EventHeader.MetaEvent.Size = GetDeltaTimeTicks( IFlow );
		break;
	default:
		return false;
		break;
	}

	return true;
}
#else
bso::bool__ mscmdm::GetEventHeader(
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	sEventHeader &EventHeader,
	err::handling__ ErrHandling )
{
	flw::sByte Datum;

	switch ( Extraneous ) {
	case xNone:
		break;
	case xTicks:
		EventHeader.DeltaTimeTicks = GetDeltaTimeTicks( IFlow );
		break;
	default:
		qRFwk();
		break;
	}

	Datum = IFlow.View();

	switch ( EventHeader.EventType ) {
		case etMIDI:
			if ( Datum & 0x80 ) {
				IFlow.Get();
				EventHeader.MIDIEvent.Tied = false;
			} else {
				EventHeader.MIDIEvent.Tied = true;
				return true;
			}
			break;
		case et_Undefined:
			EventHeader.MIDIEvent.Tied = false;
		case etSystem:
		case etMeta:
			IFlow.Get();
			break;
		default :
			qRFwk();
			break;
	}

	EventHeader.MIDIEvent.ChannelID = Datum & 0xf;

	PartiallyFillEventHeader( Datum, IFlow.View(), EventHeader );

	switch ( EventHeader.EventType ) {
	case etMIDI:
		break;
	case etSystem:
		break;
	case etMeta:
		IFlow.Get();
		EventHeader.MetaEvent.Size = GetDeltaTimeTicks( IFlow );
		break;
	default:
		return false;
		break;
	}

	return true;
}
#endif


void mscmdm::PrintMIDIEvent(
	const sExtendedMidiEvent &Event,
	const dData &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << GetMIDIEventLabel( Event.Event )<< ( Event.Tied ? "*" : "" ) << " (" << Event.ChannelID << ") :";


	sdr::row__ Row = Data.First();

	while ( Row != qNIL ) {
		OFlow << ' ' <<  Data( Row );

		Row = Data.Next( Row );
	}
}

void mscmdm::PrintSystemEvent(
	const sExtendedSystemEvent &Event,
	const dData &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << GetSystemEventLabel( Event.Event );

	if ( Event.Event == sysExclusive )
		OFlow << " : " << Data.Amount() <<  " byte long";

	OFlow << '.';
}

void mscmdm::PrintMetaEvent(
	const sExtendedMetaEvent &Event,
	const dData &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << GetMetaEventLabel( Event.Event ) << " : ";

	switch( Event.Event ) {
	case mtaText:
	case mtaCopyright:
	case mtaTrackName:
	case mtaInstrumentName:
	case mtaLyric:
	case mtaMarker:
	case mtaCuePoint:
		OFlow << Data;
		break;
	case mtaEndOfTrack:
		if ( Data.Amount() != 0 )
			qRFwk();
		break;
	case mtaChannelPrefix:
	case mtaPortPrefix:
	case mtaSequenceNumber:
	case mtaTempoSet:
	case mtaSMPTE:
	case mtaTimeSignature:
	case mtaKeySignature:
	{
		sdr::row__ Row = Data.First();

		while ( Row != qNIL ) {
			OFlow << ' ' << Data( Row );

			Row = Data.Next( Row );
		}
		break;
	}
	case mta_Unknown:
		OFlow << Data.Amount() << " byte(s) long";
		break;
	default:
		qRFwk();
		break;
	}
}

void mscmdm::PrintEvent(
	const sEventHeader &EventHeader,
	const dData &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << EventHeader.DeltaTimeTicks << txf::tab;

	switch( EventHeader.EventType ) {
	case etMIDI:
		PrintMIDIEvent( EventHeader.MIDIEvent, Data, OFlow );
		break;
	case etMeta:
		PrintMetaEvent( EventHeader.MetaEvent, Data, OFlow );
		break;
	case etSystem:
		PrintSystemEvent( EventHeader.SystemEvent, Data, OFlow );
		break;
	default:
		qRLmt();
		break;
	}
}

static sSize GetMIDIEventData_(
	const sExtendedMidiEvent &Event,
	flw::iflow__ &IFlow,
	dData &Data )
{
	sSize Counter = GetMIDIEventDataSize( Event.Event );

	while ( Counter-- )
		Data.Append( IFlow.Get() );

	return GetMIDIEventDataSize( Event.Event );
}

static sSize GetSystemEventData_(
	const sExtendedSystemEvent &Event,
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	dData &Data )
{
	sSize Size = 0;

	if ( Event.Event == sysExclusive ) {
		switch( Extraneous ) {
		case xTicks:
		{
			sDeltaTimeTicks Counter  = GetDeltaTimeTicks( IFlow ) - 1;
			Size = Counter;

			while ( Counter-- )
				Data.Append( IFlow.Get() );

			if ( IFlow.Get() != 0xf7 )
				qRFwk();
			break;
		}
		case xNone:
			flw::sByte Datum;
			while( ( Datum = IFlow.Get() ) != 0xf7 ) {
				Size++;
				Data.Append( Datum );
			}
			break;
		default:
			qRFwk();
			break;
		}
	}

	return Size;
}

static sSize GetMetaEventData_(
	const sExtendedMetaEvent &Event,
	flw::iflow__ &IFlow,
	dData &Data )
{
	sSize Counter = Event.Size;

	while ( Counter-- )
		Data.Append( IFlow.Get() );

	return Event.Size;
}

sSize mscmdm::GetEventData(
	const sEventHeader &EventHeader,
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	dData &Data )
{
	switch( EventHeader.EventType ) {
	case etMIDI:
		return GetMIDIEventData_( EventHeader.MIDIEvent, IFlow, Data );
		break;
	case etMeta:
		return GetMetaEventData_( EventHeader.MetaEvent, IFlow, Data );
		break;
	case etSystem:
		return GetSystemEventData_( EventHeader.SystemEvent, IFlow, Extraneous, Data );
		break;
	default:
		qRLmt();
		break;
	}

	return 0;	// Pour viter un 'warning'.
}

void Write_(
	const str::string_ &String,
	flw::oflow__ &OFlow )
{
	sdr::row__ Row = String.First();

	while ( Row != qNIL ) {
		OFlow.Put( String( Row ) );

		Row = String.Next( Row );
	}
}

void mscmdm::PutEventHeader(
	sDeltaTimeTicks Ticks,
	sEventId Id,
	const dData &RawData,
	bso::bool__ Tied,
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
qRH
	str::string EncodedTicks;
qRB
	switch ( Extraneous ) {
	case xNone:
		if ( Ticks != 0 )
			qRFwk();
		break;
	case xTicks:
		EncodedTicks.Init();
		Encode( Ticks, EncodedTicks );
		Write_( EncodedTicks, OFlow );
		break;
	default:
		qRFwk();
		break;
	}

	if ( !Tied )
		OFlow.Put( Id );

	Write_( RawData, OFlow );
qRR
qRT
qRE
}

void mscmdm::PutEvent(
	const dEvent &Event,
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
qRH
	wData RawData;
qRB
	RawData.Init();

	if ( Event.EventHeader().EventType == etMeta ) {
		RawData.Append( Event.EventHeader().MetaEvent.Id );

		Encode( Event.Data.Amount(), RawData );
		RawData.Append( Event.Data );
	} else if ( ( Event.EventHeader().EventType == etSystem ) && ( Event.EventHeader().SystemEvent.Event == sysExclusive ) ) {
		Encode( Event.Data.Amount() + 1, RawData );
		RawData.Append( Event.Data );
		RawData.Append( '\xf7' );
	} else {
		RawData = Event.Data;
	}

	PutEventHeader( Event.EventHeader(), RawData, Extraneous, OFlow );
qRR
qRT
qRE
}


void mscmdm::PutEvents(
	const dEvents &Events,
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
	sRow Row = Events.First();

	while ( Row != qNIL ) {
		PutEvent( Events( Row ), Extraneous, OFlow );

		Row = Events.Next( Row );
	}
}

static mscmdf::track_chunk_size__ GetSize_( const dEvents &Events )
{
	mscmdf::track_chunk_size__ Size = 0;
	sRow Row = Events.First();

	while ( Row != qNIL ) {
		Size+= GetDeltaTimeTicksSize_( Events( Row ).EventHeader().DeltaTimeTicks );

		switch( Events().EventHeader().EventType ) {
		case etMIDI:
			if ( !Events().EventHeader().MIDIEvent.Tied )
				Size += 1;
			break;
		case etSystem:
			Size += 2 + GetDeltaTimeTicksSize_( Events().Data.Amount() );
			break;
		case etMeta:
			Size += 2 + GetDeltaTimeTicksSize_( Events().Data.Amount() );
			break;
		default:
			qRFwk();
			break;
		}

		Size += Events().Data.Amount();

		Row = Events.Next( Row );
	}

	return Size;
}


void mscmdm::PutTrack(
	const dTrack &Track,
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
	sRow Row = qNIL;
	bso::bool__ LastEventIsEndOfTrack = false;
	mscmdf::track_chunk_size__ Size = GetSize_( Track );

	Row = Track.Last();

	if ( Row != qNIL )
		LastEventIsEndOfTrack = ( Track( Row ).EventHeader().EventType == etMeta ) && ( Track( Row ).EventHeader().MetaEvent.Event == mtaEndOfTrack );

	if ( !LastEventIsEndOfTrack )
		Size += 4;

	mscmdf::PutTrackChunkHeader( Size, OFlow );

	PutEvents( Track, Extraneous, OFlow );

	if ( !LastEventIsEndOfTrack )
		OFlow.WriteUpTo( "\x00\xff\x2f\x00", 4 );	// 'EndOfTrack'.

}

void mscmdm::PutTracks(
	const tracks_ &Tracks,
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
	trow__ Row = Tracks.First();

	while ( Row != qNIL ) {
		PutTrack( Tracks( Row ), Extraneous, OFlow );

		Row = Tracks.Next( Row );
	}
}

static inline bso::bool__ EventIsDoublon_(
	const sEventHeader &TargetEvent,
	const dData &TargetData,
	const sEventHeader &SourceEvent,
	const dData &SourceData )
{
	if ( TargetEvent.EventType != etMIDI )
		return false;

	if ( TargetEvent.DeltaTimeTicks != 0 )
		return false;

	return ( TargetEvent.Id == SourceEvent.Id ) && ( TargetData == SourceData );
}


bso::bool__ mscmdm::Parse(
	cEventHandler &Callback,
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	int Flags )
{
	bso::bool__ Success = true;
qRH
	wData Data, PreviousData;
	sEventHeader EventHeader, PreviousEventHeader;
	bso::bool__ Continue = true;
qRB
	PreviousData.Init();

	while ( Continue ) {
		if ( (Success = GetEventHeader( IFlow, Extraneous, EventHeader )) ) {
			Data.Init();
			GetEventData( EventHeader, IFlow, Extraneous, Data );

			if ( Flags & fmSkipDoublons )
				if ( EventIsDoublon_( EventHeader, Data, PreviousEventHeader, PreviousData ) )
					continue;

			if ( Flags & fmZeroVelocityBecomesNoteOff )
				if ( (EventHeader.EventType == etMIDI)
					 && (EventHeader.MIDIEvent.Event == midNoteOn)
					 && (Data( 2 ) == 0) ) { // Velocity
					EventHeader.Id = (EventHeader.Id & 0xf) | 0x90;
					EventHeader.MIDIEvent.Event = midNoteOff;
					EventHeader.MIDIEvent.Tied = false;
				}

			Continue = Callback.HandleEvent( EventHeader, Data, Extraneous );

			PreviousEventHeader = EventHeader;
			PreviousData = Data;
		}
	}
qRR
qRT
qRE
	return Success;
}
