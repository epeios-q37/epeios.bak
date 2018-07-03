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

//	$Id: mscmdm.h,v 1.7 2012/09/07 16:18:22 csimon Exp $

#ifndef MSCMDM__INC
#define MSCMDM__INC

#define MSCMDM_NAME		"MSCMDM"

#define	MSCMDM_VERSION	"$Revision: 1.7 $"

#define MSCMDM_OWNER		"Claude SIMON"


#if defined( E_DEBUG ) && !defined( MSCMDM_NODBG )
#define MSCMDM_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision: 1.7 $
//C Claude SIMON (csimon at zeusw dot org)
//R $Date: 2012/09/07 16:18:22 $

/* End of automatic documentation generation part. */

/* Addendum to the automatic documentation generation part. */
//D MuSiC MiDi Messages 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

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

	typedef flw::sByte event_id__;

	using flw::size__;

	// Autres donnes associes aux vnements.
	enum extraneous__ {
		// Pas d'autres donnes.
		xNone,
		// 'Delta time ticks'.
		xTicks,
		x_amount,
		x_Undefined,

	};

	//e The MIDI events.
	enum midi_event__ {
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

	const char *GetMIDIEventLabel( midi_event__ Event );

	//e The system events.
	enum system_event__ {
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

	const char *GetSystemEventLabel( system_event__ Event );

	//e The meta events.
	enum meta_event__ {
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

	const char *GetMetaEventLabel( meta_event__ Event );

	//t Type of the delta time ticks..
	typedef bso::u32__ delta_time_ticks__;

	//t Type of a channel identificator.
	typedef bso::u8__ channel_id__;

	enum event_type__ {
		// MIDI Event,
		etMIDI,
		// System event.
		etSystem,
		// Meta event
		etMeta,
		et_amount,
		et_Undefined
	};

	const char *GetEventTypeLabel( event_type__ Event );

	event_type__ DetermineEvent(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,	// For meta event only.
		bso::u8__ &Event );

	midi_event__ DetermineMIDIEvent( flw::sByte Datum );

	size__ GetMIDIEventDataSize( midi_event__ Event );


	inline event_type__ DetermineEvent(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,
		midi_event__ &MIDIEvent,
		system_event__ &SystemEvent,
		meta_event__ &MetaEvent )
	{
		bso::u8__ Event;
		event_type__ EventType = DetermineEvent( Datum, AdditionalDatum, Event );

		switch( EventType ) {
		case etMIDI:
			MIDIEvent = (midi_event__)Event;
			break;
		case etSystem:
			SystemEvent = (system_event__)Event;
			break;
		case etMeta:
			MetaEvent = (meta_event__)Event;
			break;
		}

		return EventType;
	}

	struct extended_midi_event__ {
		// The midi event.
		midi_event__ Event;
		// The channel ID.
		channel_id__ ChannelID;
		// At true for tied event.
		bso::bool__ Tied;
	};

	struct extended_system_event__ {
		// The midi event.
		system_event__ Event;
	};

	struct extended_meta_event__ {
		// The midi event.
		meta_event__ Event;
		// Data size.
		size__ Size;
		// Meta-event id.
		event_id__ Id;
	};


	//c An event header.
	struct event_header__ {
		//o The delta time ticks.
		delta_time_ticks__ DeltaTimeTicks;
		//o The event.
		event_type__ EventType;
		// Event id.
		event_id__ Id;
		union {
			// The MIDI event, if 'EventType' == 'etMIDI'.
			extended_midi_event__ MIDIEvent;
			// The system event, if 'EventType' == 'etSystem'.
			extended_system_event__ SystemEvent;
			// The meta event, if 'EventType' == 'etMeta'.
			extended_meta_event__ MetaEvent;
		};
		event_header__( void )
		{
			DeltaTimeTicks = 0;
			EventType = et_Undefined;
			Id = 0;
		}
	};

	const char *GetEventLabel( const event_header__ &Event );

	inline event_type__ PartiallyFillEventHeader(
		flw::sByte Datum,
		flw::sByte AdditionalDatum,	// Only for meta event.
		event_header__ &EventHeader )
	{
		EventHeader.Id = Datum;
		EventHeader.MetaEvent.Id = AdditionalDatum;

		EventHeader.EventType = DetermineEvent( Datum, AdditionalDatum, EventHeader.MIDIEvent.Event, EventHeader.SystemEvent.Event, EventHeader.MetaEvent.Event );

		return EventHeader.EventType;
	}

	typedef str::string_ data_;
	typedef str::string data;

	struct callback__
	{
	protected:
		virtual bso::bool__ MSCMDMHandleEvent(
			const event_header__ &Header,
			const data_ &Data,
			extraneous__ Extraneous ) = 0;
	public:
		virtual bso::bool__ HandleEvent(
			const event_header__ &Header,
			const data_ &Data,
			extraneous__ Extraneous )
		{
			return MSCMDMHandleEvent( Header, Data, Extraneous );
		}
	};

	enum flag_position__ {
		fpSkipDoublons,	// Identical MIDI events with 0 tick times are elminated.
		fpZeroVelocityBecomesNoteOff,	// 'NoteOn' event with '0' as velocity becomes 'NoteOff'.
		fp_amount,
		fp_Undefined
	};

	enum flag_mask {
		fmSkipDoublons = 1 << fpSkipDoublons,
		fmZeroVelocityBecomesNoteOff = 1 << fpZeroVelocityBecomesNoteOff,
		fmDefaultFlag = fmSkipDoublons | fmZeroVelocityBecomesNoteOff
	};


	bso::bool__ Parse(
		callback__ &Callback,
		flw::iflow__ &IFlow,
		extraneous__ Extraneous,
		int Flags = fmDefaultFlag );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline bso::bool__ Parse(
		callback__ &Callback,
		mscmdd::rRFlow &IFlow,
		int Flags = fmDefaultFlag )
	{
		return Parse( Callback, IFlow, xNone, Flags );
	}
#endif
		

	//f Put in 'EventHeader' the header of the event red in 'IFlow'. Return the size of the header. If '04, the error.
	bso::bool__ GetEventHeader(
		flw::iflow__ &IFlow,
		extraneous__ Extraneous,
		event_header__ &EventHeader,
		err::handling__ ErrHandling = err::h_Default );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline bso::bool__ GetEventHeader(
		mscmdd::rRFlow &IFlow,
		event_header__ &EventHeader,
		err::handling__ ErrHandling = err::h_Default )
	{
		return GetEventHeader( IFlow, xNone, EventHeader, ErrHandling );
	}
#endif

	size__ GetEventData(
		const event_header__ &EventHeader,
		flw::iflow__ &IFlow,
		extraneous__ Extraneous,
		data_ &Data );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline size__ GetEventData(
		const event_header__ &EventHeader,
		mscmdd::rRFlow &IFlow,
		data_ &Data )
	{
		return GetEventData( EventHeader, IFlow, xNone, Data );
	}
# endif

	class event_
	{
	public:
		struct s
		{
			event_header__ EventHeader;
			data_::s Data;
		} &S_;
		data_ Data;
		event_( s &S )
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
		event_ &operator =( const event_ &E )
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
			const event_header__ &Header,
			const data_ &Data )
		{
			reset();

			S_.EventHeader = Header;
			this->Data.Init( Data );
		}
		E_RWDISCLOSE_( event_header__, EventHeader )
	};

	E_AUTO( event )

	E_ROW( erow__ );	// Event row.

	typedef ctn::E_MCONTAINERt_( event_, erow__ ) events_;
	E_AUTO( events )

	void PutEventHeader(
		delta_time_ticks__ Ticks,
		event_id__ Id,
		const data_ &RawData,
		bso::bool__ Tied,
		extraneous__ Extraneous,
		flw::oflow__ &OFlow );

	inline void PutEventHeader(
		const event_header__ &Header,
		const data_ &RawData,
		extraneous__ Extraneous,
		flw::oflow__ &OFlow )
	{
		PutEventHeader( Header.DeltaTimeTicks, Header.Id, RawData, ( Header.EventType == etMIDI ? Header.MIDIEvent.Tied : false ), Extraneous, OFlow );
	}

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEventHeader(
		const event_header__ &Header,
		const data_ &RawData,
		mscmdd::rRWFlow &OFlow )
	{
		PutEventHeader( Header, RawData, xNone, OFlow );
	}
# endif


	void PutEvent(
		const event_ &Event,
		extraneous__ Extraneous,
		flw::oflow__ &OFlow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEvent(
		const event_ &Event,
		mscmdd::rWFlow &OFlow )
	{
		PutEvent( Event, xNone, OFlow );
	}
# endif

	void PutEvents(
		const events_ &Events,
		extraneous__ Extraneous,
		flw::oflow__ &OFlow );
	
# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutEvents(
		const events_ &Events,
		mscmdd::rWFlow &OFlow )
	{
		PutEvents( Events, xNone, OFlow );
	}
# endif

	typedef events_	track_;
	typedef events	track;

	void PutTrack(
		const track_ &Track,
		extraneous__ Extraneous,
		flw::oflow__ &OFlow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutTrack(
		const track_ &Track,
		mscmdd::rWFlow &OFlow )
	{
		PutTrack( Track, xNone, OFlow );
	}
# endif

	E_ROW( trow__ );	// Track row.

	typedef ctn::E_CONTAINERt_( track_, trow__ ) tracks_;
	E_AUTO( tracks )

	void PutTracks(
		const tracks_ &Tracks,
		extraneous__ Extraneous,
		flw::oflow__ &Flow );

# ifdef MSCMDM__DEVICES_AVAILABLE
	inline void PutTracks(
		const tracks_ &Tracks,
		mscmdd::rWFlow &OFlow )
	{
		PutTracks( Tracks, xNone, OFlow );
	}
# endif

	inline bso::bool__ IsMetaDataText( meta_event__ Event )
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
	typedef str::string_	text_;
	typedef str::string		text;
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
	inline delta_time_ticks__ GetDeltaTimeTicks( flw::iflow__ &IFlow )
	{
		flw::sByte Datum = IFlow.Get();
		delta_time_ticks__ DeltaTimeTicks = 0;

		while ( Datum & 0x80 ) {
			DeltaTimeTicks = ( DeltaTimeTicks << 7  ) | ( Datum & 0x7f );

			Datum = IFlow.Get();
		}

		return  ( DeltaTimeTicks << 7 ) | Datum;
	}

	void Encode(
		delta_time_ticks__ Ticks,
		data_ &Data );

	inline void GetText(
		flw::iflow__ &IFlow,
		size__ Size,
		text_ &Text )
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
		const extended_midi_event__ &Event,
		const data_ &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow', which origin is 'Origin', the description of the system event 'Event' in 'IFlow'.
	void PrintSystemEvent(
		const extended_system_event__ &Event,
		const data_ &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow' the description of the meta event 'Event' in 'IFlow'.
	void PrintMetaEvent(
		const extended_meta_event__ &Event,
		const data_ &Data,
		txf::text_oflow__ &OFlow );

	//f Print to 'OFlow', which origin is 'Origin', the description of the event in 'IFlow'.
	void PrintEvent(
		const event_header__ &EventHeader,
		const data_ &Data,
		txf::text_oflow__ &OFlow );

}

/*$END$*/
#endif
