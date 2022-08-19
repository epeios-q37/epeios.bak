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

#define MSCMDX_COMPILATION_

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

static const char *GetHex_( flw::sByte Datum )
{
	static char Buffer[3];

	sprintf( Buffer, "%02x", Datum );

	return Buffer;
}

static void GetHex_(
	const mscmdm::dData &Data,
	str::string_ &Result )
{
	sdr::sRow Row = Data.First();

	if ( Row != qNIL ) {
		Result.Append( GetHex_( Data( Row ) ) );

		Row = Data.Next( Row );
	}

	while ( Row != qNIL )  {
		Result.Append( ' ' );

		Result.Append( GetHex_( Data( Row ) ) );

		Row = Data.Next( Row );
	}
}

static bso::bool__ ParseTrackChunk_(
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	xml::rWriter &Writer,
	err::handling__ ErrHandling )
{
	bso::bool__ Success = false;
qRH
	bso::integer_buffer__ Buffer;
	mscmdf::track_chunk_size__ TrackChunkSize = 0;
	mscmdm::sEventHeader EventHeader;
	mscmdm::wData Data, EncodedSize;
	str::string HexBuffer;
qRB
	TrackChunkSize = mscmdf::GetTrackChunkSize( IFlow, ErrHandling );

	if ( TrackChunkSize != 0 ) {
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

            Data.Init();
            mscmdm::GetEventData( EventHeader, IFlow, Extraneous, Data );

            HexBuffer.Init();

            if ( EventHeader.EventType == mscmdm::etMeta ) {
                HexBuffer.Append( GetHex_ ( EventHeader.MetaEvent.Id ) );

                HexBuffer.Append( ' ' );

                EncodedSize.Init();

                mscmdm::MIDIEncode( EventHeader.MetaEvent.Size, EncodedSize );

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

        Success = true;
	}
qRR
qRT
qRE
	return Success;
}

bso::bool__ mscmdx::MIDIToXMID(
	flw::iflow__ &IFlow,
	eExtraneous Extraneous,
	xml::rWriter &Writer,
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
	mscmdf::sSMFType _SMFType;
	mscmdf::sDeltaTimeTicks _HeaderChunkDeltaTimeTicks;
	mscmdm::sEventId _Id, _MetaId;
	bso::bool__ _TiedEvent;
	mscmdm::sDeltaTimeTicks _EventDeltaTimeTicks;
	flw::oflow__ *_OFlow;
	mscmdm::wData _RawData, _MetaData;
	mscmdm::wEvents _Events;
	mscmdm::tracks _Tracks;
	eExtraneous _Extraneous;
	bso::bool__ _GetData(
		const str::string_ &Value,
		mscmdm::dData &Data )
	{
		sdr::sRow Error = qNIL, Row = Value.First();

		while ( Row != qNIL ) {
			Error = qNIL;

			Data.Append(Value.ToU8(Row, &Error, str::sBase(16)));

			if ( Error != qNIL ) {
				if ( Value( Error ) != ' ' )
					return false;

				Row = Value.Next( Error );

			} else
				Row = qNIL;
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
			bso::sChar Delimiter,
			const str::string_ &Value,
			const dump_ &Dump )
		{
			sdr::sRow Error = qNIL;

			if ( TagName == MIDI_FILE_TAG ) {
				if ( Name == SMF_TYPE_ATTRIBUTE )
					_SMFType = Value.ToU8( &Error );
				else if ( Name == DELTA_TIME_TICKS_ATTRIBUTE )
					_HeaderChunkDeltaTimeTicks = Value.ToU16( &Error );
			} else if ( TagName == TRACK_CHUNK_TAG ) {
			} else if ( TagName == EVENT_TAG ) {
				if ( Name == DELTA_TIME_TICKS_ATTRIBUTE )
					_EventDeltaTimeTicks = Value.ToU32( &Error );
				else if ( Name == ID_ATTRIBUTE )
					_Id = Value.ToU8(&Error, str::sBase(16));
				else if ( Name == TIED_ATTRIBUTE )
					_TiedEvent = true;
				else if ( Name == META_ID_ATTRIBUTE ) {
					_MetaId = Value.ToU8(&Error, str::sBase(16));
				} else if ( Name == DATA_ATTRIBUTE )
					if ( !_GetData( Value, _RawData ) )
						return false;
			}

			return Error == qNIL;

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
		qRH
			mscmdm::wEvent Event;
			mscmdm::sEventHeader EventHeader;
		qRB
			if ( Name == EVENT_TAG ) {
				mscmdm::PartiallyFillEventHeader( _Id, _MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = _EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = _TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						_MetaData = _RawData;

						_MetaData.Remove( _MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( _MetaData.First() != qNIL ) && ( _MetaData( _MetaData.First() ) & 0x80 ) )
							_MetaData.Remove( _MetaData.First() );

						if ( _MetaData.First() != qNIL )
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
					qRFwk();

				mscmdf::PutHeaderChunk( _SMFType, (mscmdf::sTrackChunkAmount)_Tracks.Amount(), _HeaderChunkDeltaTimeTicks, *_OFlow );
				mscmdm::PutTracks( _Tracks, _Extraneous, *_OFlow );

				_Tracks.Init();
			}
		qRR
		qRT
		qRE
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
		eExtraneous Extraneous )
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
	eExtraneous Extraneous,
	flw::oflow__ &OFlow )
{
	xml::status__ Status = xml::s_Undefined;
qRH
	callback Callback;
qRB
	Callback.Init( OFlow, Extraneous );

	Status = xml::Parse( IFlow, xml::eh_Default, Callback );
qRR
qRT
qRE
	return Status;
}

static inline bso::bool__ GetData_(
	const str::string_ &Value,
	mscmdm::dData &Data )
{
	sdr::sRow Error = qNIL;

	sdr::sRow Row = Value.First();

	while ( Row != qNIL ) {
		Error = qNIL;

		Data.Append(Value.ToU8(Row, &Error, str::sBase(16)));

		if ( Error != qNIL ) {
			if ( Value( Error ) != ' ' )
				return false;

			Row = Value.Next( Error );

		} else
			Row = qNIL;
	}

	return true;
}

status__ mscmdx::ParseEvent(
	xml::parser___ &Parser,
	mscmdm::dEvent &Event )
{
	status__ Status = s_Undefined;
qRH
	bso::bool__ TiedEvent = false;
	mscmdm::wData RawData, MetaData;
	mscmdm::sDeltaTimeTicks EventDeltaTimeTicks;
	mscmdm::sEventId Id, MetaId;
	bso::bool__ InProgress = false;
qRB
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
				sdr::sRow Error = qNIL;

				if ( Parser.AttributeName() == DELTA_TIME_TICKS_ATTRIBUTE )
					EventDeltaTimeTicks = Parser.Value().ToU32( &Error );
				else if ( Parser.AttributeName() == ID_ATTRIBUTE )
					Id = Parser.Value().ToU8(&Error,str::sBase(16));
				else if ( Parser.AttributeName() == TIED_ATTRIBUTE )
					TiedEvent = true;
				else if ( Parser.AttributeName() == META_ID_ATTRIBUTE ) {
					MetaId = Parser.Value().ToU8(&Error, str::sBase(16));
				} else if ( Parser.AttributeName() == DATA_ATTRIBUTE )
					if ( !GetData_( Parser.Value(), RawData ) )
						return Status = sBadDataAttributeValue;

				if ( Error != qNIL )
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
				mscmdm::sEventHeader EventHeader;

				mscmdm::PartiallyFillEventHeader( Id, MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						MetaData = RawData;

						MetaData.Remove( MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( MetaData.First() != qNIL ) && ( MetaData( MetaData.First() ) & 0x80 ) )
							MetaData.Remove( MetaData.First() );

						if ( MetaData.First() != qNIL )
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
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Status;
}

status__ mscmdx::ParseEvents(
	xml::parser___ &Parser,
	mscmdm::dEvents &Events )
{
	status__ Status = s_Undefined;
qRH
	bso::bool__ TiedEvent = false;
	mscmdm::wData RawData, MetaData;
	mscmdm::sDeltaTimeTicks EventDeltaTimeTicks;
	mscmdm::sEventId Id, MetaId;
	bso::bool__ InProgress = false;
qRB
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
				sdr::sRow Error = qNIL;

				if ( Parser.AttributeName() == DELTA_TIME_TICKS_ATTRIBUTE )
					EventDeltaTimeTicks = Parser.Value().ToU32( &Error );
				else if ( Parser.AttributeName() == ID_ATTRIBUTE )
					Id = Parser.Value().ToU8(&Error, str::sBase(16));
				else if ( Parser.AttributeName() == TIED_ATTRIBUTE )
					TiedEvent = true;
				else if ( Parser.AttributeName() == META_ID_ATTRIBUTE ) {
					MetaId = Parser.Value().ToU8(&Error, str::sBase(16));
				} else if ( Parser.AttributeName() == DATA_ATTRIBUTE )
					if ( !GetData_( Parser.Value(), RawData ) )
						return Status = sBadDataAttributeValue;

				if ( Error != qNIL )
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
			qRH
				mscmdm::wEvent Event;
				mscmdm::sEventHeader EventHeader;
			qRB
				mscmdm::PartiallyFillEventHeader( Id, MetaId, EventHeader );

				EventHeader.DeltaTimeTicks = EventDeltaTimeTicks;
				EventHeader.MIDIEvent.Tied = TiedEvent;

				if ( EventHeader.EventType == mscmdm::etMeta ) {
					if ( !mscmdm::IsMetaDataText( EventHeader.MetaEvent.Event ) ) {

						MetaData = RawData;

						MetaData.Remove( MetaData.First() );	// Removing Meta id

						// Removing size.
						while ( ( MetaData.First() != qNIL ) && ( MetaData( MetaData.First() ) & 0x80 ) )
							MetaData.Remove( MetaData.First() );

						if ( MetaData.First() != qNIL )
							MetaData.Remove( MetaData.First() );
					}

					Event.Init( EventHeader, MetaData );
				} else
					Event.Init( EventHeader, RawData );

				RawData.Init();
				MetaData.Init();

				Events.Append( Event );
			qRR
			qRT
			qRE
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
			qRFwk();
			break;
		}
	}
qRR
qRT
qRE
	return Status;
}
