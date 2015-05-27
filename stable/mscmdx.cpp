/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define MSCMDX__COMPILATION

#include "mscmdx.h"

using namespace mscmdx;

using xml::dump_;

#define MIDI_FILE_TAG	"MidiFile"
#define TRACK_CHUNK_TAG	"TrackChunk"

#define TIME_STAMP_ATTRIBUTE			"TimeStamp"
#define SMF_TYPE_ATTRIBUTE				"SMFType"
#define TRACK_CHUNK_AMOUNT_ATTRIBUTE	"TrackChunkAmount"
#define DELTA_TIME_TICKS_ATTRIBUTE		"DeltaTimeTicks"
#define SIZE_ATTRIBUTE					"Size"
#define EVENT_TAG						"Event"
#define TYPE_ATTRIBUTE					"Type"
#define ID_ATTRIBUTE					"Id"
#define LABEL_ATTRIBUTE					"Label"
#define TIED_ATTRIBUTE					"Tied"
#define DATA_ATTRIBUTE					"Data"
#define META_ID_ATTRIBUTE				"MetaId"

static const char *GetHex_( flw::datum__ Datum )
{
	static char Buffer[3];

	sprintf( Buffer, "%02x", Datum );

	return Buffer;
}

static void GetHex_(
	const mscmdm::data_ &Data,
	str::string_ &Result )
{
	mdr::row__ Row = Data.First();

	if ( Row != NONE ) {
		Result.Append( GetHex_( Data( Row ) ) );

		Row = Data.Next( Row );
	}

	while ( Row != NONE  ) {
		Result.Append( ' ' );

		Result.Append( GetHex_( Data( Row ) ) );

		Row = Data.Next( Row );
	}
}

static bso::bool__ ParseTrackChunk_(
	flw::iflow__ &IFlow,
	mscmdm::extraneous__ Extraneous,
	xml::writer_ &Writer,
	err::handling__ ErrHandling )
{
	bso::bool__ Success = false;
ERRProlog
	bso::integer_buffer__ Buffer;
	mscmdf::track_chunk_size__ TrackChunkSize = 0;
	mscmdm::event_header__ EventHeader;
	mscmdm::data Data, EncodedSize;
	str::string HexBuffer;
ERRBegin
	TrackChunkSize = mscmdf::GetTrackChunkSize( IFlow, ErrHandling );

	if ( TrackChunkSize == 0 )
		ERRReturn;

	Writer.PushTag( TRACK_CHUNK_TAG );

	Writer.PutAttribute( SIZE_ATTRIBUTE, bso::Convert( TrackChunkSize, Buffer ) );

	do {
		mscmdm::GetEventHeader( IFlow, Extraneous, EventHeader );
		Writer.PushTag( EVENT_TAG );

		if ( Extraneous == mscmdm::xTicks )
			Writer.PutAttribute( DELTA_TIME_TICKS_ATTRIBUTE, bso::Convert( EventHeader.DeltaTimeTicks, Buffer ) );

		Writer.PutAttribute( TYPE_ATTRIBUTE, mscmdm::GetEventTypeLabel( EventHeader.EventType ) );

		Writer.PutAttribute( ID_ATTRIBUTE, GetHex_( EventHeader.Id ) );

		Writer.PutAttribute( LABEL_ATTRIBUTE, mscmdm::GetEventLabel( EventHeader ) );

		if ( EventHeader.EventType == mscmdm::etMIDI )
			if ( EventHeader.MIDIEvent.Tied )
				Writer.PutAttribute( TIED_ATTRIBUTE, "true" );


		HexBuffer.Init();

		if ( EventHeader.EventType == mscmdm::etMeta ) {
		}

		Data.Init();
		mscmdm::GetEventData( EventHeader, IFlow, Extraneous, Data );

		if ( EventHeader.EventType == mscmdm::etMeta ) {
			HexBuffer.Append( GetHex_ (EventHeader.MetaEvent.Id ) );

			HexBuffer.Append( ' ' );

			EncodedSize.Init();

			mscmdm::Encode( EventHeader.MetaEvent.Size, EncodedSize );

			GetHex_( EncodedSize, HexBuffer );

			if ( Data.Amount() != 0 )
				HexBuffer.Append( ' ' );
		}

		GetHex_( Data, HexBuffer );
		Writer.PutAttribute( DATA_ATTRIBUTE, HexBuffer );

		if ( EventHeader.EventType == mscmdm::etMeta ) {
			Writer.PutAttribute( META_ID_ATTRIBUTE, GetHex_( EventHeader.MetaEvent.Id ) );

			if ( mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) )
				Writer.PutValue( Data );
		}

		Writer.PopTag();
	} while ( ( EventHeader.EventType != mscmdm::etMeta ) || ( EventHeader.MetaEvent.Event != mscmdm::mtaEndOfTrack ) );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
	return true;
}

bso::bool__ mscmdx::MIDIToXMID(
	flw::iflow__ &IFlow,
	extraneous__ Extraneous,
	xml::writer_ &Writer,
	err::handling__ ErrHandling )
{
	bso::integer_buffer__ IBuffer;
	mscmdf::header_chunk__ HeaderChunk;
	tol::buffer__ DTBuffer;

	if ( !mscmdf::GetHeaderChunk( IFlow, HeaderChunk, ErrHandling ) )
		return false;

	Writer.PushTag( MIDI_FILE_TAG );

	Writer.PutAttribute( SMF_TYPE_ATTRIBUTE, bso::Convert( HeaderChunk.SMFType, IBuffer ) );
	Writer.PutAttribute( TRACK_CHUNK_AMOUNT_ATTRIBUTE, bso::Convert( HeaderChunk.TrackChunkAmount, IBuffer ) );
	Writer.PutAttribute( DELTA_TIME_TICKS_ATTRIBUTE, bso::Convert( HeaderChunk.DeltaTimeTicks, IBuffer ) );

	Writer.PutAttribute( TIME_STAMP_ATTRIBUTE, tol::DateAndTime( DTBuffer ) );

	while ( HeaderChunk.TrackChunkAmount-- )
		if ( !ParseTrackChunk_( IFlow, Extraneous, Writer, ErrHandling ) )
			return false;

	Writer.PopTag();

	return true;
}

struct callback
: public xml::callback__
{
private:
	mscmdf::smf_type__ _SMFType;
	mscmdf::delta_time_ticks__ _HeaderChunkDeltaTimeTicks;
	mscmdm::event_id__ _Id, _MetaId;
	bso::bool__ _TiedEvent;
	mscmdm::delta_time_ticks__ _EventDeltaTimeTicks;
	flw::oflow__ *_OFlow;
	mscmdm::data _RawData, _MetaData;
	mscmdm::events _Events;
	mscmdm::tracks _Tracks;
	mscmdm::extraneous__ _Extraneous;
	bso::bool__ _GetData(
		const str::string_ &Value,
		mscmdm::data_ &Data )
	{
		mdr::row__ Error = NONE;

		mdr::row__ Row = Value.First();

		while ( Row != NONE ) {
			Error = NONE;

			Data.Append( Value.ToUB( Row, &Error, str::b16 ) );

			if ( Error != NONE ) {
				if ( Value( Error ) != ' ' )
					return false;

				Row = Value.Next( Error );

			} else
				Row = NONE;
		}
		
		return true;
	}
protected:
		virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump )
		{
			return true;
		}
		virtual bso::bool__ XMLStartTag(
			const str::string_ &Name,
			const dump_ &Dump )
		{
			if ( Name == EVENT_TAG ) {
				_RawData.Init();
				_MetaData.Init();
				_TiedEvent = false;
			}

			return true;
		}
		virtual bso::bool__ XMLStartTagClosed(
			const str::string_ &Name,
			const dump_ &Dump )
		{
			return true;
		}
		virtual bso::bool__ XMLAttribute(
			const str::string_ &TagName,
			const str::string_ &Name,
			const str::string_ &Value,
			const dump_ &Dump )
		{
			mdr::row__ Error = NONE;

			if ( TagName == MIDI_FILE_TAG ) {
				if ( Name == SMF_TYPE_ATTRIBUTE )
					_SMFType = Value.ToUB( &Error );
				else if ( Name == DELTA_TIME_TICKS_ATTRIBUTE )
					_HeaderChunkDeltaTimeTicks = Value.ToUS( &Error );
			} else if ( TagName == TRACK_CHUNK_TAG ) {
			} else if ( TagName == EVENT_TAG ) {
				if ( Name == DELTA_TIME_TICKS_ATTRIBUTE )
					_EventDeltaTimeTicks = Value.ToUL( &Error );
				else if ( Name == ID_ATTRIBUTE )
					_Id = Value.ToUB( &Error, str::b16 );
				else if ( Name == TIED_ATTRIBUTE )
					_TiedEvent = true;
				else if ( Name == META_ID_ATTRIBUTE ) {
					_MetaId = Value.ToUB( &Error, str::b16 );
				} else if ( Name == DATA_ATTRIBUTE )
					if ( !_GetData( Value, _RawData ) )
						return false;
			}

			return Error == NONE;

		}
		virtual bso::bool__ XMLValue(
			const str::string_ &TagName,
			const str::string_ &Value,
			const dump_ &Dump )
		{
			if ( _MetaData.Amount() != 0 )
				return false;
			else {
				_MetaData = Value;
				return true;
			}
		}
		virtual bso::bool__ XMLEndTag(
			const str::string_ &Name,
			const dump_ &Dump )
		{
		ERRProlog
			mscmdm::event Event;
			mscmdm::event_header__ EventHeader;
		ERRBegin
			if ( Name == EVENT_TAG ) {
				mscmdm::PartiallyFillEventHeader( _Id, _MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = _EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = _TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						_MetaData = _RawData;

						_MetaData.Remove( _MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( _MetaData.First() != NONE ) && ( _MetaData( _MetaData.First() ) & 0x80 ) )
							_MetaData.Remove( _MetaData.First() );

						if ( _MetaData.First() != NONE )
							_MetaData.Remove( _MetaData.First() );
					}

					Event.Init( EventHeader, _MetaData );
				} else
					Event.Init( EventHeader, _RawData );

				_RawData.Init();
				_MetaData.Init();

				_Events.Append( Event );
			} else if ( Name == TRACK_CHUNK_TAG ) {
				_Tracks.Append( _Events );
				_Events.Init();
			} else if ( Name == MIDI_FILE_TAG ) {
				if ( _Tracks.Amount() > MSCMDF_TRACK_CHUNK_AMOUNT_MAX )
					ERRu();

				mscmdf::PutHeaderChunk( _SMFType, (mscmdf::track_chunk_amount__)_Tracks.Amount(), _HeaderChunkDeltaTimeTicks, *_OFlow );
				mscmdm::PutTracks( _Tracks, _Extraneous, *_OFlow );

				_Tracks.Init();
			}
		ERRErr
		ERREnd
		ERREpilog
			return true;
		}
public:
	void reset( bso::bool__ P = true )
	{
		_OFlow = NULL;
		_RawData.reset( P );
		_MetaData.reset( P );
		_Events.reset( P );
		_Tracks.reset( P );
		_Extraneous = mscmdm::x_Undefined;
	}
	callback( void )
	{
		reset( false );
	}
	~callback( void )
	{
		reset();
	}
	void Init(
		flw::oflow__ &OFlow,
		mscmdm::extraneous__ Extraneous )
	{
		reset();

		_RawData.Init();
		_MetaData.Init();
		_Events.Init();
		_Tracks.Init();
		_Extraneous = Extraneous;

		_OFlow = &OFlow;
	}
};

xml::status__ mscmdx::XMIDToMIDI(
	xtf::extended_text_iflow__ &IFlow,
	mscmdm::extraneous__ Extraneous,
	flw::oflow__ &OFlow )
{
	xml::status__ Status = xml::s_Undefined;
ERRProlog
	callback Callback;
ERRBegin
	Callback.Init( OFlow, Extraneous );

	Status = xml::Parse( IFlow, xml::eh_Default, Callback );
ERRErr
ERREnd
ERREpilog
	return Status;
}

static inline bso::bool__ GetData_(
	const str::string_ &Value,
	mscmdm::data_ &Data )
{
	mdr::row__ Error = NONE;

	mdr::row__ Row = Value.First();

	while ( Row != NONE ) {
		Error = NONE;

		Data.Append( Value.ToUB( Row, &Error, str::b16 ) );

		if ( Error != NONE ) {
			if ( Value( Error ) != ' ' )
				return false;

			Row = Value.Next( Error );

		} else
			Row = NONE;
	}
		
	return true;
}

status__ mscmdx::ParseEvent(
	xml::parser___ &Parser,
	mscmdm::event_ &Event )
{
	status__ Status = s_Undefined;
ERRProlog
	bso::bool__ TiedEvent = false;
	mscmdm::data RawData, MetaData;
	mscmdm::delta_time_ticks__ EventDeltaTimeTicks;
	mscmdm::event_id__ Id, MetaId;
	bso::bool__ InProgress = false;
ERRBegin
	RawData.Init();
	MetaData.Init();

	while ( Status == s_Undefined ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == EVENT_TAG ) {

				if ( InProgress )
					Status = sUnexpectedTag;

				RawData.Init();
				MetaData.Init();
				TiedEvent = false;
				InProgress = true;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.TagName() == EVENT_TAG ) {
				mdr::row__ Error = NONE;

				if ( Parser.AttributeName() == DELTA_TIME_TICKS_ATTRIBUTE )
					EventDeltaTimeTicks = Parser.Value().ToUL( &Error );
				else if ( Parser.AttributeName() == ID_ATTRIBUTE )
					Id = Parser.Value().ToUB( &Error, str::b16 );
				else if ( Parser.AttributeName() == TIED_ATTRIBUTE )
					TiedEvent = true;
				else if ( Parser.AttributeName() == META_ID_ATTRIBUTE ) {
					MetaId = Parser.Value().ToUB( &Error, str::b16 );
				} else if ( Parser.AttributeName() == DATA_ATTRIBUTE )
					if ( !GetData_( Parser.Value(), RawData ) )
						return Status = sBadDataAttributeValue;

				if ( Error != NONE )
					Status = sBadIdValue;

				InProgress = true;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tValue:
			if ( Parser.TagName() == EVENT_TAG ) {

				if ( !InProgress )
					Status = sUnexpectedContext;

				if ( MetaData.Amount() == 0 )
					MetaData = Parser.Value();
				else
					Status = sUnexpectedValue;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() == EVENT_TAG ) {
				mscmdm::event_header__ EventHeader;

				mscmdm::PartiallyFillEventHeader( Id, MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						MetaData = RawData;

						MetaData.Remove( MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( MetaData.First() != NONE ) && ( MetaData( MetaData.First() ) & 0x80 ) )
							MetaData.Remove( MetaData.First() );

						if ( MetaData.First() != NONE )
							MetaData.Remove( MetaData.First() );
					}

					Event.Init( EventHeader, MetaData );
				} else
					Event.Init( EventHeader, RawData );

				RawData.Init();
				MetaData.Init();
			}

			if ( !InProgress )
				Status = sOK;


			break;
		case xml::t_Processed:
			Status = sUnexpectedContext;
			break;
		case xml::t_Error:
			Status = sXML;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

status__ mscmdx::ParseEvents(
	xml::parser___ &Parser,
	mscmdm::events_ &Events )
{
	status__ Status = s_Undefined;
ERRProlog
	bso::bool__ TiedEvent = false;
	mscmdm::data RawData, MetaData;
	mscmdm::delta_time_ticks__ EventDeltaTimeTicks;
	mscmdm::event_id__ Id, MetaId;
	bso::bool__ InProgress = false;
ERRBegin
	RawData.Init();
	MetaData.Init();

	while ( Status == s_Undefined ) {
		switch ( Parser.Parse( xml::tfObvious ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == EVENT_TAG ) {

				if ( InProgress )
					Status = sUnexpectedTag;

				RawData.Init();
				MetaData.Init();
				TiedEvent = false;
				InProgress = true;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tAttribute:
			if ( Parser.TagName() == EVENT_TAG ) {
				mdr::row__ Error = NONE;

				if ( Parser.AttributeName() == DELTA_TIME_TICKS_ATTRIBUTE )
					EventDeltaTimeTicks = Parser.Value().ToUL( &Error );
				else if ( Parser.AttributeName() == ID_ATTRIBUTE )
					Id = Parser.Value().ToUB( &Error, str::b16 );
				else if ( Parser.AttributeName() == TIED_ATTRIBUTE )
					TiedEvent = true;
				else if ( Parser.AttributeName() == META_ID_ATTRIBUTE ) {
					MetaId = Parser.Value().ToUB( &Error, str::b16 );
				} else if ( Parser.AttributeName() == DATA_ATTRIBUTE )
					if ( !GetData_( Parser.Value(), RawData ) )
						return Status = sBadDataAttributeValue;

				if ( Error != NONE )
					Status = sBadIdValue;

				InProgress = true;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tValue:
			if ( Parser.TagName() == EVENT_TAG ) {

				if ( !InProgress )
					Status = sUnexpectedContext;

				if ( MetaData.Amount() == 0 )
					MetaData = Parser.Value();
				else
					Status = sUnexpectedValue;
			} else
				Status = sUnexpectedTag;
			break;
		case xml::tEndTag:
			if ( Parser.TagName() == EVENT_TAG ) {
			ERRProlog
				mscmdm::event Event;
				mscmdm::event_header__ EventHeader;
			ERRBegin
				mscmdm::PartiallyFillEventHeader( Id, MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						MetaData = RawData;

						MetaData.Remove( MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( MetaData.First() != NONE ) && ( MetaData( MetaData.First() ) & 0x80 ) )
							MetaData.Remove( MetaData.First() );

						if ( MetaData.First() != NONE )
							MetaData.Remove( MetaData.First() );
					}

					Event.Init( EventHeader, MetaData );
				} else
					Event.Init( EventHeader, RawData );

				RawData.Init();
				MetaData.Init();

				Events.Append( Event );
			ERRErr
			ERREnd
			ERREpilog
			}

			if ( !InProgress )
				Status = sOK;


			break;
		case xml::t_Processed:
			Status = sUnexpectedContext;
			break;
		case xml::t_Error:
			Status = sXML;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

