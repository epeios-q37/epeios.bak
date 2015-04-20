/*
	'mscmdm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mscmdm' header file ('mscmdm.h').
	Copyright (C) 2002, 2004 Claude SIMON.

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



//	$Id: mscmdm.cpp,v 1.8 2012/09/07 16:18:22 csimon Exp $

#define MSCMDM__COMPILATION

#include "mscmdm.h"

class mscmdmtutor
: public ttr_tutor
{
public:
	mscmdmtutor( void )
	: ttr_tutor( MSCMDM_NAME )
	{
#ifdef MSCMDM_DBG
		Version = MSCMDM_VERSION "\b\bD $";
#else
		Version = MSCMDM_VERSION;
#endif
		Owner = MSCMDM_OWNER;
		Date = "$Date: 2012/09/07 16:18:22 $";
	}
	virtual ~mscmdmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

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

static bso::ubyte__ MIDIEventDataSize_[mid_amount] = {
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

const char *mscmdm::GetMIDIEventLabel( midi_event__ Event )
{
	if ( Event >= mid_amount )
		ERRu();

	return MIDIEventLabels_[Event];
}

size__ mscmdm::GetMIDIEventDataSize( midi_event__ Event )
{
	if ( Event >= mid_amount )
		ERRu();

	return MIDIEventDataSize_[Event ];
}

const char *mscmdm::GetSystemEventLabel( system_event__ Event )
{
	if ( Event >= sys_amount )
		ERRu();

	return SystemEventLabels_[Event];
}

const char *mscmdm::GetMetaEventLabel( meta_event__ Event )
{
	if ( Event >= mta_amount )
		ERRu();

	return MetaEventLabels_[Event];
}

const char *mscmdm::GetEventLabel( const event_header__ &Event )
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
		ERRu();
		break;
	}

	return NULL;	// To avoid a 'warning'.
}


const char *mscmdm::GetEventTypeLabel( event_type__ Type )
{
	if ( Type >= et_amount )
		ERRu();

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

static inline bso::ubyte__ GetDeltaTimeTicksSize_( delta_time_ticks__ Ticks )
{
	bso::ubyte__ Size = 1;

	if ( Ticks & MASK1 )
		Size++;

	if ( Ticks & MASK2 )
		Size++;

	if ( Ticks & MASK3 )
		Size++;

	return Size;
}

void mscmdm::Encode(
	delta_time_ticks__ Ticks,
	data_ &Data )
{
	if ( Ticks & MASK1 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS1 ) & BASE_MASK ) ) );

	if ( Ticks & MASK2 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS2 ) & BASE_MASK ) ) );

	if ( Ticks & MASK3 )
		Data.Append( (bso::char__)( 0x80 | ( ( Ticks >> POS3 ) & BASE_MASK ) ) );

	Data.Append( (bso::char__)( Ticks & BASE_MASK ) );
}

midi_event__ mscmdm::DetermineMIDIEvent( flw::datum__ Datum )
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

static inline system_event__ DetermineSystemEvent_( flw::datum__ Datum )
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

static inline meta_event__ DetermineMetaEvent_( flw::datum__ Datum )
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

event_type__ mscmdm::DetermineEvent(
	flw::datum__ Datum,
	flw::datum__ AdditionalDatum,	// For meta only.
	bso::ubyte__ &Event )
{
	event_type__ EventType = et_Undefined;

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
	extraneous__ Extraneous,
	event_header__ &EventHeader,
	err::handling__ ErrHandling )
{
	flw::datum__ Datum;

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
	extraneous__ Extraneous,
	event_header__ &EventHeader,
	err::handling__ ErrHandling )
{
	flw::datum__ Datum;

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
			ERRc();
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
	const extended_midi_event__ &Event,
	const data_ &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << GetMIDIEventLabel( Event.Event )<< ( Event.Tied ? "*" : "" ) << " (" << (bso::ulong__)Event.ChannelID << ") :";

	mdr::row__ Row = Data.First();

	while ( Row != NONE ) {
		OFlow << ' ' << (bso::ulong__)Data( Row );

		Row = Data.Next( Row );
	}
}

void mscmdm::PrintSystemEvent(
	const extended_system_event__ &Event,
	const data_ &Data,
	txf::text_oflow__ &OFlow )
{
	OFlow << GetSystemEventLabel( Event.Event );

	if ( Event.Event == sysExclusive )
		OFlow << " : " << (bso::ulong__)Data.Amount() <<  " byte long";

	OFlow << '.';
}

void mscmdm::PrintMetaEvent(
	const extended_meta_event__ &Event,
	const data_ &Data,
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
			ERRc();
		break;
	case mtaChannelPrefix:
	case mtaPortPrefix:
	case mtaSequenceNumber:
	case mtaTempoSet:
	case mtaSMPTE:
	case mtaTimeSignature:
	case mtaKeySignature:
	{
		mdr::row__ Row = Data.First();

		while ( Row != NONE ) {
			OFlow << ' ' << (bso::ulong__)Data( Row );

			Row = Data.Next( Row );
		}
		break;
	}
	case mta_Unknown:
		OFlow << (bso::ulong__)Data.Amount() << " byte(s) long";
		break;
	default:
		ERRu();
		break;
	}
}

void mscmdm::PrintEvent(
	const event_header__ &EventHeader,
	const data_ &Data,
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
		ERRl();
		break;
	}
}

static size__ GetMIDIEventData_(
	const extended_midi_event__ &Event,
	flw::iflow__ &IFlow,
	data_ &Data )
{
	size__ Counter = GetMIDIEventDataSize( Event.Event );

	while ( Counter-- )
		Data.Append( IFlow.Get() );

	return GetMIDIEventDataSize( Event.Event );
}

static size__ GetSystemEventData_(
	const extended_system_event__ &Event,
	flw::iflow__ &IFlow,
	extraneous__ Extraneous,
	data_ &Data )
{
	size__ Size = 0;

	if ( Event.Event == sysExclusive ) {
		switch( Extraneous ) {
		case xTicks:
		{
			delta_time_ticks__ Counter  = GetDeltaTimeTicks( IFlow ) - 1;
			Size = Counter;

			while ( Counter-- )
				Data.Append( IFlow.Get() );

			if ( IFlow.Get() != 0xf7 )
				ERRf();
			break;
		}
		case xNone:
			flw::datum__ Datum;
			while( ( Datum = IFlow.Get() ) != 0xf7 ) {
				Size++;
				Data.Append( Datum );
			}
			break;
		default:
			ERRu();
			break;
		}
	}

	return Size;
}

static size__ GetMetaEventData_(
	const extended_meta_event__ &Event,
	flw::iflow__ &IFlow,
	data_ &Data )
{
	size__ Counter = Event.Size;

	while ( Counter-- )
		Data.Append( IFlow.Get() );

	return Event.Size;
}

size__ mscmdm::GetEventData(
	const event_header__ &EventHeader,
	flw::iflow__ &IFlow,
	extraneous__ Extraneous,
	data_ &Data )
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
		ERRl();
		break;
	}

	return 0;	// Pour viter un 'warning'.
}

void Write_(
	const str::string_ &String,
	flw::oflow__ &OFlow )
{
	mdr::row__ Row = String.First();

	while ( Row != NONE ) {
		OFlow.Put( String( Row ) );

		Row = String.Next( Row );
	}
}

void mscmdm::PutEventHeader(
	delta_time_ticks__ Ticks,
	event_id__ Id,
	const data_ &RawData,
	bso::bool__ Tied,
	extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
ERRProlog
	str::string EncodedTicks;
ERRBegin
	switch ( Extraneous ) {
	case xNone:
		if ( Ticks != 0 )
			ERRu();
		break;
	case xTicks:
		EncodedTicks.Init();
		Encode( Ticks, EncodedTicks );
		Write_( EncodedTicks, OFlow );
		break;
	default:
		ERRu();
		break;
	}

	if ( !Tied )
		OFlow.Put( Id );

	Write_( RawData, OFlow );
ERRErr
ERREnd
ERREpilog
}

void mscmdm::PutEvent(
	const event_ &Event,
	extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
ERRProlog
	data RawData;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}


void mscmdm::PutEvents(
	const events_ &Events,
	extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
	ctn::E_CMITEMt( event_, erow__ ) Event;
	erow__ Row = Events.First();

	Event.Init( Events );

	while ( Row != NONE ) {
		PutEvent( Event( Row ), Extraneous, OFlow );

		Row = Events.Next( Row );
	}
}

static mscmdf::track_chunk_size__ GetSize_( const events_ &Events )
{
	mscmdf::track_chunk_size__ Size = 0;
	ctn::E_CMITEMt( event_, erow__ ) Event;
	erow__ Row = Events.First();

	Event.Init( Events );

	while ( Row != NONE ) {
		Size+= GetDeltaTimeTicksSize_( Event( Row ).EventHeader().DeltaTimeTicks );

		switch( Event().EventHeader().EventType ) {
		case etMIDI:
			if ( !Event().EventHeader().MIDIEvent.Tied )
				Size += 1;
			break;
		case etSystem:
			Size += 2 + GetDeltaTimeTicksSize_( Event().Data.Amount() );
			break;
		case etMeta:
			Size += 2 + GetDeltaTimeTicksSize_( Event().Data.Amount() );
			break;
		default:
			ERRu();
			break;
		}

		Size += Event().Data.Amount();

		Row = Events.Next( Row );
	}

	return Size;
}


void mscmdm::PutTrack(
	const track_ &Track,
	extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
	ctn::E_CMITEMt( event_, erow__ ) Event;
	erow__ Row = NONE;
	bso::bool__ LastEventIsEndOfTrack = false;
	mscmdf::track_chunk_size__ Size = GetSize_( Track );

	Event.Init( Track );

	Row = Track.Last();

	if ( Row != NONE )
		LastEventIsEndOfTrack = ( Event( Row ).EventHeader().EventType == etMeta ) && ( Event( Row ).EventHeader().MetaEvent.Event == mtaEndOfTrack );

	if ( !LastEventIsEndOfTrack )
		Size += 4;

	mscmdf::PutTrackChunkHeader( Size, OFlow );

	PutEvents( Track, Extraneous, OFlow );

	if ( !LastEventIsEndOfTrack )
		OFlow.WriteUpTo( "\x00\xff\x2f\x00", 4 );	// 'EndOfTrack'.

}

void mscmdm::PutTracks(
	const tracks_ &Tracks,
	extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
	ctn::E_CITEMt( track_, trow__ ) Track;
	trow__ Row = Tracks.First();

	Track.Init( Tracks );

	while ( Row != NONE ) {
		PutTrack( Track( Row ), Extraneous, OFlow );

		Row = Tracks.Next( Row );
	}
}

static inline bso::bool__ EventIsDoublon_(
	const event_header__ &TargetEvent,
	const data_ &TargetData,
	const event_header__ &SourceEvent,
	const data_ &SourceData )
{
	if ( TargetEvent.EventType != etMIDI )
		return false;

	if ( TargetEvent.DeltaTimeTicks != 0 )
		return false;

	return ( TargetEvent.Id == SourceEvent.Id ) && ( TargetData == SourceData );
}


bso::bool__ mscmdm::Parse(
	callback__ &Callback,
	flw::iflow__ &IFlow,
	extraneous__ Extraneous,
	int Flags )
{
	bso::bool__ Success = true;
ERRProlog
	data Data, PreviousData;
	event_header__ EventHeader, PreviousEventHeader;
	bso::bool__ Continue = true;
ERRBegin
	PreviousData.Init();

	while ( Continue ) {
		if ( !( Success = GetEventHeader( IFlow, Extraneous, EventHeader ) ) )
			ERRReturn;

		Data.Init();
		GetEventData( EventHeader, IFlow, Extraneous, Data );

		if ( Flags & fmSkipDoublons )
			if ( EventIsDoublon_( EventHeader, Data, PreviousEventHeader, PreviousData ) )
				continue;

		if ( Flags & fmZeroVelocityBecomesNoteOff )
			if ( ( EventHeader.EventType == etMIDI )
				  && ( EventHeader.MIDIEvent.Event == midNoteOn )
				  && ( Data( 2 ) == 0 ) ) { // Velocity
				EventHeader.Id = ( EventHeader.Id & 0xf ) | 0x90;
				EventHeader.MIDIEvent.Event = midNoteOff;
				EventHeader.MIDIEvent.Tied = false;
			}

		Continue = Callback.HandleEvent( EventHeader, Data, Extraneous );

		PreviousEventHeader = EventHeader;
		PreviousData = Data;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscmdmpersonnalization
: public mscmdmtutor
{
public:
	mscmdmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmdmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mscmdmpersonnalization Tutor;

ttr_tutor &MSCMDMTutor = Tutor;
