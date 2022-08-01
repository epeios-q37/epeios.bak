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

// MuSiC MiDi Messages

#ifndef MSCMDM_INC_
# define MSCMDM_INC_


# if defined( E_DEBUG ) && !defined( MSCMDM_NODBG )
#  define MSCMDM_DBG
# endif

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "str.h"
#include "ctn.h"

# include "cpe.h"

# ifdef CPE_S_WIN
#  ifdef CPE_F_MT
#   define MSCMDM__DEVICES_AVAILABLE
#  endif
# else
#  define MSCMDM__DEVICES_AVAILABLE
# endif

# ifdef MSCMDM__DEVICES_AVAILABLE
#  include "mscmdd.h"
# endif

namespace mscmdm {

	typedef flw::sByte sEventId;

	using fdr::sSize;

	// Autres donnes associes aux vnements.
	qENUM( Extraneous) {
		// Pas d'autres donnes.
		xNone,
		// 'Delta time ticks'.
		xTicks,
		x_amount,
		x_Undefined,

	};

	//e The MIDI events.
	// 'mid' prefix to avoid confusion with 'eMetaEvent'.
	qENUM( MidiEvent ) {
		//i Note off event.
		midNoteOff,
		//i Note on event.
		midNoteOn,
		//i Key after-touch event.
		midKeyAftertouch,
		//i Control change event.
		midControlChange,
		//i Program change event.
		midProgramChange,
		//i Channel after-touch event.
		midChannelAftertouch,
		//i Pitch wheel change event.
		midPitchWheelChange,
		//i Amount items.
		mid_amount,
		//i Undefined event
		mid_Undefined,
		mid_NotAMIDIEvent,
	};

	const char *GetMIDIEventLabel( eMidiEvent Event );

	//e The system events.
	qENUM( SystemEvent ) {
		//i Exclusive system event,
		sysExclusive,
		//i Timing clock system event.
		sysTimingClock,
		//i Start system event.
		sysStart,
		//i Stop system event.
		sysStop,
		//i Continue system event.
		sysContinue,
		//i Active sensing system event.
		sysActiveSensing,
		//i Reset system envent.
		sysReset,
		//i Song position pointer system event.
		sysSongPositionPointer,
		//i Song select system event.
		sysSongSelect,
		//i End of exclusive system event,
		sysEndOfExclusive,
		//i Amount items.
		sys_amount,
		//i Unknow system event.
		sys_Undefined,
		sys_NotASystemEvent,
	};

	const char *GetSystemEventLabel( eSystemEvent Event );

	//e The meta events.
	// 'mta' prefix to avoid confusion with 'eMidiEvent'.
	qENUM( MetaEvent ) {
		//i Sequence number meta-event.
		mtaSequenceNumber,
		//i Test meta-event,
		mtaText,
		//i Copyright meta-event.
		mtaCopyright,
		//i Track name meta-event.
		mtaTrackName,
		//i Intrument name meta-event.
		mtaInstrumentName,
		//i Lyric meta-event.
		mtaLyric,
		//i Marker meta-event.
		mtaMarker,
		//i Cue point meta-event.
		mtaCuePoint,
		//i Channel prefix meta-event.
		mtaChannelPrefix,
		//i Port prefix meta-event.
		mtaPortPrefix,
		//i End of track meta-event.
		mtaEndOfTrack,
		//i Tempo set meta-event.
		mtaTempoSet,
		//i SMPTE meta-event.
		mtaSMPTE,
		//i Time signature meta-event.
		mtaTimeSignature,
		//i Key signature meta-event.
		mtaKeySignature,
		//i Sequencer meta-event.
		mtaSequencer,
		// Unknown meta-event.
		mta_Unknown,
		//i Amount items.
		mta_amount,
		// Undefined.
		mta_Undefined,
		mta_NotAMetaEvent,
	};

	const char *GetMetaEventLabel( eMetaEvent Event );

	//t Type of the delta time ticks..
	typedef bso::u32__ sDeltaTimeTicks;

	//t Type of a channel identificator.
	typedef bso::u8__ sChannelId;

	qENUM( EventType ) {
		// MIDI Event,
		etMIDI,
		// System event.
		etSystem,
		// Meta event
		etMeta,
		et_amount,
		et_Undefined
	};

	const char *GetEventTypeLabel( eEventType Event );

	eEventType DetermineEvent(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,	// For meta event only.
		bso::u8__ &Event );

	eMidiEvent DetermineMIDIEvent( flw::sByte Datum );

	sSize GetMIDIEventDataSize( eMidiEvent Event );

	inline eEventType DetermineEvent(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,
		eMidiEvent &MIDIEvent,
		eSystemEvent &SystemEvent,
		eMetaEvent &MetaEvent )
	{
		bso::u8__ Event;
		eEventType EventType = DetermineEvent( Datum, AdditionalDatum, Event );

		switch( EventType ) {
		case etMIDI:
			MIDIEvent = (eMidiEvent)Event;
			break;
		case etSystem:
			SystemEvent = (eSystemEvent)Event;
			break;
		case etMeta:
			MetaEvent = (eMetaEvent)Event;
			break;
        default:
            qRFwk();
            break;
		}

		return EventType;
	}

	struct sExtendedMidiEvent {
		// The midi event.
		eMidiEvent Event;
		// The channel ID.
		sChannelId ChannelID;
		// At true for tied event.
		bso::bool__ Tied;
	};

	struct sExtendedSystemEvent {
		// The midi event.
		eSystemEvent Event;
	};

	struct sExtendedMetaEvent {
		// The midi event.
		eMetaEvent Event;
		// Data size.
		sSize Size;
		// Meta-event id.
		sEventId Id;
	};


	//c An event header.
	struct sEventHeader {
		//o The delta time ticks.
		sDeltaTimeTicks DeltaTimeTicks;
		//o The event.
		eEventType EventType;
		// Event id.
		sEventId Id;
		union {
			// The MIDI event, if 'EventType' == 'etMIDI'.
			sExtendedMidiEvent MIDIEvent;
			// The system event, if 'EventType' == 'etSystem'.
			sExtendedSystemEvent SystemEvent;
			// The meta event, if 'EventType' == 'etMeta'.
			sExtendedMetaEvent MetaEvent;
		};
		sEventHeader( void )
		{
			DeltaTimeTicks = 0;
			EventType = et_Undefined;
			Id = 0;
		}
	};

	const char *GetEventLabel( const sEventHeader &Event );

	inline eEventType PartiallyFillEventHeader(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,	// Only for meta event.
		sEventHeader &EventHeader )
	{
		EventHeader.Id = Datum;
		EventHeader.MetaEvent.Id = AdditionalDatum;

		EventHeader.EventType = DetermineEvent( Datum, AdditionalDatum, EventHeader.MIDIEvent.Event, EventHeader.SystemEvent.Event, EventHeader.MetaEvent.Event );

		return EventHeader.EventType;
	}

	typedef str::string_ dData;
	qW(Data);

	struct cEventHandler
	{
	protected:
		virtual bso::bool__ MSCMDMHandleEvent(
			const sEventHeader &Header,
			const dData &Data,
			eExtraneous Extraneous ) = 0;
	public:
		virtual bso::bool__ HandleEvent(
			const sEventHeader &Header,
			const dData &Data,
			eExtraneous Extraneous )
		{
			return MSCMDMHandleEvent( Header, Data, Extraneous );
		}
		qCALLBACK( EventHandler );
	};

	qENUM( FlagPosition ) {
		fpSkipDoublons,	// Identical MIDI events with 0 tick times are elminated.
		fpZeroVelocityBecomesNoteOff,	// 'NoteOn' event with '0' as velocity becomes 'NoteOff'.
		fp_amount,
		fp_Undefined
	};

	qENUM( FlagMask ) {
		fmSkipDoublons = 1 << fpSkipDoublons,
		fmZeroVelocityBecomesNoteOff = 1 << fpZeroVelocityBecomesNoteOff,
		fmDefaultFlag = fmSkipDoublons | fmZeroVelocityBecomesNoteOff
	};


	bso::bool__ Parse(
		cEventHandler &Callback,
		flw::iflow__ &IFlow,
		eExtraneous Extraneous,
		int Flags = fmDefaultFlag );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline bso::bool__ Parse(
		cEventHandler &Callback,
		mscmdd::rRFlow &IFlow,
		int Flags = fmDefaultFlag )
	{
		return Parse( Callback, IFlow, xNone, Flags );
	}
#endif


	//f Put in 'EventHeader' the header of the event red in 'IFlow'. Return the size of the header. If '04, the error.
	bso::bool__ GetEventHeader(
		flw::iflow__ &IFlow,
		eExtraneous Extraneous,
		sEventHeader &EventHeader,
		err::handling__ ErrHandling = err::h_Default );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline bso::bool__ GetEventHeader(
		mscmdd::rRFlow &IFlow,
		sEventHeader &EventHeader,
		err::handling__ ErrHandling = err::h_Default )
	{
		return GetEventHeader( IFlow, xNone, EventHeader, ErrHandling );
	}
#endif

	sSize GetEventData(
		const sEventHeader &EventHeader,
		flw::iflow__ &IFlow,
		eExtraneous Extraneous,
		dData &Data );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline sSize GetEventData(
		const sEventHeader &EventHeader,
		mscmdd::rRFlow &IFlow,
		dData &Data )
	{
		return GetEventData( EventHeader, IFlow, xNone, Data );
	}
# endif

	class dEvent
	{
	public:
		struct s
		{
			sEventHeader EventHeader;
			dData::s Data;
		} &S_;
		dData Data;
		dEvent( s &S )
		: S_( S ),
		  Data( S.Data )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.EventHeader.EventType = et_Undefined;
			S_.EventHeader.DeltaTimeTicks = 0;

			Data.reset( P );
		}
		void plug( bch::sHook &Hook )
		{
			Data.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Data.plug( AS );
		}
		dEvent &operator =( const dEvent &E )
		{
			S_.EventHeader = E.S_.EventHeader;

			Data = E.Data;

			return *this;
		}
		void Init( void )
		{
			reset();

			Data.Init();
		}
		void Init(
			const sEventHeader &Header,
			const dData &Data )
		{
			reset();

			S_.EventHeader = Header;
			this->Data.Init( Data );
		}
		E_RWDISCLOSE_( sEventHeader, EventHeader )
	};

	qW(Event);

	qROW( Row );	// Event row.

	typedef ctn::E_MCONTAINERt_( dEvent, sRow ) dEvents;
	qW(Events)

	void PutEventHeader(
		sDeltaTimeTicks,
		sEventId Id,
		const dData &RawData,
		bso::bool__ Tied,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow );

	inline void PutEventHeader(
		const sEventHeader &Header,
		const dData &RawData,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow )
	{
		PutEventHeader( Header.DeltaTimeTicks, Header.Id, RawData, ( Header.EventType == etMIDI ? Header.MIDIEvent.Tied : false ), Extraneous, OFlow );
	}

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEventHeader(
		const sEventHeader &Header,
		const dData &RawData,
		mscmdd::rRWFlow &OFlow )
	{
		PutEventHeader( Header, RawData, xNone, OFlow );
	}
# endif

	void PutEvent(
		const dEvent &Event,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEvent(
		const dEvent &Event,
		mscmdd::rWFlow &OFlow )
	{
		PutEvent( Event, xNone, OFlow );
	}
# endif

	void PutEvents(
		const dEvents &Events,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEvents(
		const dEvents &Events,
		mscmdd::rWFlow &OFlow )
	{
		PutEvents( Events, xNone, OFlow );
	}
# endif

	typedef dEvents	dTrack;
	qW(Track);

	void PutTrack(
		const dTrack &Track,
		eExtraneous Extraneous,
		flw::oflow__ &OFlow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutTrack(
		const dTrack &Track,
		mscmdd::rWFlow &OFlow )
	{
		PutTrack( Track, xNone, OFlow );
	}
# endif

	E_ROW( trow__ );	// Track row.

	typedef ctn::E_CONTAINERt_( dTrack, trow__ ) tracks_;
	E_AUTO( tracks )

	void PutTracks(
		const tracks_ &Tracks,
		eExtraneous Extraneous,
		flw::oflow__ &Flow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutTracks(
		const tracks_ &Tracks,
		mscmdd::rWFlow &OFlow )
	{
		PutTracks( Tracks, xNone, OFlow );
	}
# endif

	inline bso::bool__ IsMetaDataText( eMetaEvent Event )
	{
		switch( Event ) {
		case mtaText:
		case mtaCopyright:
		case mtaTrackName:
		case mtaInstrumentName:
		case mtaLyric:
		case mtaMarker:
		case mtaCuePoint:
			return true;
			break;
		case mtaChannelPrefix:
		case mtaPortPrefix:
		case mtaEndOfTrack:
		case mtaSequenceNumber:
		case mtaTempoSet:
		case mtaSMPTE:
		case mtaTimeSignature:
		case mtaKeySignature:
		case mta_Unknown:
			return false;
			break;
		default:
			qRFwk();
			break;
		}

		return false;	// To avoid a 'warning'.
	}


	//t Text type.
	typedef str::string_	dText;
	qW(Text);
/*
	//t Type of a sequence number.
	typedef bso::ulong__ sequence_number__;

	//t Type of a tempo.
	typedef bso::ulong__ tempo__;

	//t Type of a time signature (would be detailed in the future).
	typedef bso::ulong__ time_signature__;

	//t Type of a key signature (would be detailed in th future).
	typedef bso::ushort__ key_signature__;
*/
	inline sDeltaTimeTicks GetDeltaTimeTicks( flw::iflow__ &IFlow )
	{
		flw::sByte Datum = IFlow.Get();
		sDeltaTimeTicks DeltaTimeTicks = 0;

		while ( Datum & 0x80 ) {
			DeltaTimeTicks = ( DeltaTimeTicks << 7  ) | ( Datum & 0x7f );

			Datum = IFlow.Get();
		}

		return  ( DeltaTimeTicks << 7 ) | Datum;
	}

	void MIDIEncode(
		bso::sUHuge Value,
		dData &Data );

	inline void GetText(
		flw::iflow__ &IFlow,
		sSize Size,
		dText &Text )
	{
		while( Size-- )
			Text.Append( IFlow.Get() );
	}
/*
	inline sequence_number__ GetSequenceNumber( flw::iflow__ &IFlow )
	{
		return ( IFlow.Get() << 24 ) | ( IFlow.Get() << 16 ) | ( IFlow.Get() << 8 ) | IFlow.Get();
	}

	inline tempo__ GetTempo( flw::iflow__ &IFlow )
	{
		return ( IFlow.Get() << 16 ) | ( IFlow.Get() << 8 ) | IFlow.Get();
	}

	inline time_signature__ GetTimeSignature( flw::iflow__ &IFlow )
	{
		time_signature__ TimeSignature;

		flw::Get( IFlow, TimeSignature );

		return TimeSignature;
	}

	inline key_signature__ GetKeySignature( flw::iflow__ &IFlow )
	{
		key_signature__ KeySignature;

		flw::Get( IFlow, KeySignature );

		return KeySignature;
	}
*/
	void PrintMIDIEvent(
		const sExtendedMidiEvent &Event,
		const dData &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow', which origin is 'Origin', the description of the system event 'Event' in 'IFlow'.
	void PrintSystemEvent(
		const sExtendedSystemEvent &Event,
		const dData &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow' the description of the meta event 'Event' in 'IFlow'.
	void PrintMetaEvent(
		const sExtendedMetaEvent &Event,
		const dData &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow', which origin is 'Origin', the description of the event in 'IFlow'.
	void PrintEvent(
		const sEventHeader &EventHeader,
		const dData &Data,
		txf::text_oflow__ &OFlow );

}

/*$END$*/
#endif
