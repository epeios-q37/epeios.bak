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

#define MSCVKP_COMPILATION_

#include "mscvkp.h"

#include "mscmdf.h"

using namespace mscvkp;

void mscvkp::Fill(
	flw::iflow__ &IFlow,
	mscmdm::origin__ Origin,
	data_set_ &DataSet )
{
	bso::ubyte__ Datum, Test;

	switch ( Origin ) {
	case mscmdm::oFile:
		IFlow.Get();	// We skip the size.
		break;
	case mscmdm::oDevice:
		break;
	default:
		ERRu();
		break;
	}

	if ( IFlow.Get() != 0x41 )	// ID number
		ERRf();

	IFlow.Get();	// Device ID.

	if ( ( IFlow.Get() << 8 |IFlow.Get() ) != 0x1a )	// Model ID.
		ERRf();

	if ( IFlow.Get() != 0x12 )	// Command ID.
		ERRf();

	DataSet.SetAddress( ( IFlow.Get() << 24 ) 
		            | ( IFlow.Get() << 16 )
					| ( IFlow.Get() << 8 ) |
					  IFlow.Get() );

	Datum = IFlow.Get();

	while( ( Test = IFlow.Get() ) != 0xF7 ) {
		DataSet.Append( Datum );
		Datum = Test;
	}

	if ( Datum != Checksum( DataSet ) )
		ERRf();
}

datum__ mscvkp::Checksum( const data_set_ &DataSet )
{
	bso::ubyte__ Checksum = 0;
	address__ Address = DataSet.Address();
	epeios::row__ Row = DataSet.First();

	while( Row != NONE ) {
		Checksum += DataSet( Row );
		Row = DataSet.Next( Row );
	}

	Checksum += ( (datum__)( ( Address >> 24 ) & 0xff )
				+ (datum__)( ( Address >> 16 ) & 0xff )
				+ (datum__)( ( Address >> 8 ) & 0xff )
				+ (datum__)( Address & 0xff ) );

	return ( 128 - ( Checksum & 0x7f ) ) & 0x7f;
}

void mscvkp::Fill(
	flw::iflow__ &IFlow,
	mscmdm::origin__ Origin,
	data_sets_ &DataSets )
{
qRH
	mscmdf::header_chunk__ HeaderChunk;
	mscmdf::track_chunk_size__ TrackChunkSize;
	mscmdm::event_header__ EventHeader;
	mscvkp::data_set DataSet;
qRB
	mscmdf::GetHeaderChunk( IFlow, HeaderChunk );
	TrackChunkSize = mscmdf::GetTrackChunkSize( IFlow );

	while( HeaderChunk.TrackChunkAmount-- ) {
		mscmdm::GetEventHeader( IFlow, Origin, EventHeader );

		while ( ( ( EventHeader.EventType != mscmdm::etSystem ) || ( EventHeader.SystemEvent.Event != mscmdm::sysExclusive ) ) )
			mscmdm::GetEventHeader( IFlow, Origin, EventHeader );

		while ( ( ( EventHeader.EventType == mscmdm::etSystem ) && ( EventHeader.SystemEvent.Event == mscmdm::sysExclusive ) ) ) {
			DataSet.Init();
			mscvkp::Fill( IFlow, Origin, DataSet );
			DataSets.Append( DataSet );
			mscmdm::GetEventHeader( IFlow, Origin, EventHeader );
		}

		if ( ( EventHeader.EventType != mscmdm::etMeta ) && ( EventHeader.MetaEvent.Event = mscmdm::mtaEndOfTrack ) )
			ERRf();
	}
qRR
qRT
qRE
}


txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const descriptions__ &Descriptions )
{
	epeios::row__ Row = Descriptions.First();

	if ( Row != NONE ) {
		TFlow << Descriptions( Row );
		Row = Descriptions.Next( Row );
	}

	while( Row != NONE ) {
		TFlow << '/';
		TFlow << Descriptions( Row );
		Row = Descriptions.Next( Row );
	}

	return TFlow;

}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const data_set_ &DataSet )
{
qRH
	descriptions__ Descriptions;
	address__ Address;
qRB
	Address = DataSet.Address();

	epeios::row__ Row = DataSet.First();

	while( Row != NONE ) {
		if ( !IsName( Address ) || ( Address & 0xff ) == 0 ) {
			TFlow << AddressToString( Address ) << ' ';

			Descriptions.Init();

			GetDescription( Address, Descriptions );

			TFlow << Descriptions << " : ";
		}

		if ( IsName( Address ) ) {
			TFlow << (char)DataSet( Row );

			if ( ( Address & 0xff ) == 0x0f )
				TFlow << txf::nl;
		} else
			TFlow << DatumToString( DataSet( Row ), true ) << txf::nl;


		Row = DataSet.Next( Row );
		Address++;
	}

qRR
qRT
qRE
	return TFlow;
}
	
txf::text_oflow__ &operator <<(
	txf::text_oflow__ &TFlow,
	const data_sets_ &DataSets )
{
	ctn::E_CMITEM( data_set_ ) DataSet;
	epeios::row__ Row = DataSets.First();

	DataSet.Init( DataSets );

	while( Row != NONE ) {
		TFlow << DataSet( Row );
		Row = DataSets.Next( Row );
		TFlow << txf::nl;
	}

	return TFlow;
}

#define ADD( value )	Descriptions.Append( description__( value ) )
#define ADDC( value1, value2 )\
	if ( Flat )\
		Descriptions.Append( description__( value1 " " value2 ) );\
	else {\
		Descriptions.Append( description__( value1 ) );\
		Descriptions.Append( description__( value2 ) );\
	}


static void HandleSystem_(
	address__ Address,
	descriptions__ &Descriptions )
{

	if ( ( Address >= 0x00 ) && ( Address <= 0x0d ) )
		ADD( "Basic" );
	else if ( ( Address >= 0x0e ) && ( Address <= 0x24 ) )
		ADD( "MIDI" );
	else
		ERRu();


	switch( Address ) {
	case 0x00:
		ADD( "Master Tune" );
		break;
	case 0x01:
		ADD( "Key Transpose" );
		break;
	case 0x02:
		ADD( "Note Remain" );
		break;
	case 0x03:
		ADD( "Hold Polarity" );
		break;
	case 0x04:
		ADD( "Control Pedal 1" );
		ADD( "Polarity" );
		break;
	case 0x05:
		ADD( "Control Pedal 1" );
		ADD( "Assign" );
		break;
	case 0x06:
		ADD( "Control Pedal 2" );
		ADD( "Polarity" );
		break;
	case 0x07:
		ADD( "Control Pedal 2" );
		ADD( "Assign" );
		break;
	case 0x08:
		ADD( "PK Foot Switch Assign" );
		ADD( "Left" );
		break;
	case 0x09:
		ADD( "PK Foot Switch Assign" );
		ADD( "Right" );
		break;
	case 0x0a:
		ADD( "Tone Cabinet Effect" );
		break;
	case 0x0b:
		ADD( "Power Up Registration" );
		ADD( "Mode" );
		break;
	case 0x0c:
		ADD( "Power Up Registration" );
		ADD( "Number" );
		break;
	case 0x0d:
		ADD( "Master Reverb Level" );
		break;
	case 0x0e:
		ADD( "Thru" );
		break;
	case 0x0f:
		ADD( "Organ Orch" );
		ADD( "Tx" );
		break;
	case 0x10:
		ADD( "Organ Orch" );
		ADD( "Rx" );
		break;
	case 0x11:
		ADD( "Organ Control Channel" );
		break;
	case 0x12:
		ADD( "Organ Channel" );
		ADD( "Upper" );
		break;
	case 0x13:
		ADD( "Organ Channel" );
		ADD( "Lower" );
		break;
	case 0x14:
		ADD( "Organ Channel" );
		ADD( "Pedal" );
		break;
	case 0x15:
		ADD( "Orch Channel" );
		ADD( "Upper" );
		break;
	case 0x16:
		ADD( "Orch Channel" );
		ADD( "Lower" );
		break;
	case 0x17:
		ADD( "Orch Channel" );
		ADD( "Pedal" );
		break;
	case 0x18:
		ADD( "Ext Channel" );
		ADD( "Upper" );
		break;
	case 0x19:
		ADD( "Ext Channel" );
		ADD( "Lower" );
		break;
	case 0x1a:
		ADD( "Ext Channel" );
		ADD( "Pedal" );
		break;
	case 0x1b:
		ADD( "Harmonic Bar Mode" );
		break;
	case 0x1c:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 16'" );
		break;
	case 0x1d:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 5-1/3'" );
		break;
	case 0x1e:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 8'" );
		break;
	case 0x1f:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 4'" );
		break;
	case 0x20:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 2-2/3'" );
		break;
	case 0x21:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 2'" );
		break;
	case 0x22:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 1-3/5'" );
		break;
	case 0x23:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 1-1/3'" );
		break;
	case 0x24:
		ADD( "Harmonic Bar Ctrl" );
		ADD( "No. 1'" );
		break;
	default:
		ERRu();
		break;
	}
}

static void HandleOrchestral_(
	address__ Address,
	descriptions__ &Descriptions )
{

	char Buffer[5];

	sprintf( Buffer, "#%s", GetVoiceLabel( Address ) );

	ADD( Buffer );

	Address &= 0xffff;

	switch( Address ) {
	case 0x00:
		ADD( "Instrument" );
		break;
	case 0x01:
		ADD( "Level" );
		break;
	case 0x02:
		ADD( "Velocity" );
		ADD( "Sensitivity" );
		break;
	case 0x03:
		ADD( "Velocity" );
		ADD( "Offset" );
		break;
	case 0x04:
		ADD( "Key Shift" );
		break;
	case 0x05:
		ADD( "Fine Tune" );
		break;
	case 0x06:
		ADD( "LFO" );
		ADD( "Pitch Depth" );
		break;
	case 0x07:
		ADD( "LFO" );
		ADD( "Rate" );
		break;
	case 0x08:
		ADD( "Brilliance" );
		break;
	case 0x09:
		ADD( "Envelope" );
		ADD( "Attack" );
		break;
	case 0x0a:
		ADD( "Envelope" );
		ADD( "Release" );
		break;
	case 0x0b:
		ADD( "Glide" );
		ADD( "Switch" );
		break;
	case 0x0c:
		ADD( "Glide" );
		ADD( "Rate" );
		break;
	case 0x0d:
		ADD( "Glide" );
		ADD( "Depth" );
		break;
	case 0x0e:
		ADD( "Pitch Bend Control Sensitivity" );
		break;
	case 0x0f:
		ADD( "Modulation Control Sensitivity" );
		break;
	case 0x10:
		ADD( "Aftertouch Sensitivity" );
		break;
	case 0x11:
		ADD( "Reverb Send Level" );
		break;
	case 0x12:
		ADD( "Chorus" );
		ADD( "Type" );
		break;
	case 0x13:
		ADD( "Chorus" );
		ADD( "Rate (Time)" );
		break;
	case 0x14:
		ADD( "Chorus" );
		ADD( "Depth (FeedBack)" );
		break;
	case 0x15:
		ADD( "Chorus" );
		ADD( "Level" );
		break;
	default:
		ERRu();
		break;
	}
}

static void HandleCommon_(
	address__ Address,
	descriptions__ &Descriptions,
	bso::bool__ Flat )
{
	switch( Address & 0xff ) {
	case 0x00:
		ADDC( "Name", "1" );
		break;
	case 0x01:
		ADDC( "Name", "2" );
		break;
	case 0x02:
		ADDC( "Name", "3" );
		break;
	case 0x03:
		ADDC( "Name", "4" );
		break;
		break;
	case 0x04:
		ADDC( "Name", "5" );
		break;
		break;
	case 0x05:
		ADDC( "Name", "6" );
		break;
		break;
	case 0x06:
		ADDC( "Name", "7" );
		break;
		break;
	case 0x07:
		ADDC( "Name", "8" );
		break;
		break;
	case 0x08:
		ADDC( "Name", "9" );
		break;
		break;
	case 0x09:
		ADDC( "Name", "10" );
		break;
		break;
	case 0x0a:
		ADDC( "Name", "11" );
		break;
		break;
	case 0x0b:
		ADDC( "Name", "12" );
		break;
		break;
	case 0x0c:
		ADDC( "Name", "13" );
		break;
		break;
	case 0x0d:
		ADDC( "Name", "14" );
		break;
		break;
	case 0x0e:
		ADDC( "Name", "15" );
		break;
		break;
	case 0x0f:
		ADDC( "Name", "16" );
		break;
		break;
	case 0x10:
		ADD( "Pedal to Lower" );
		break;
	case 0x11:
		ADD( "Lower Keyboead Split Point" );
		break;
	case 0x12:
		ADD( "Organ Part" );
		ADD( "Bender Assign" );
		break;
	case 0x13:
		ADD( "Organ Part" );
		ADD( "Modulation Assign" );
		break;
	case 0x14:
		ADD( "Organ Part" );
		ADD( "Aftertouch Assign" );
		break;
	case 0x15:
		ADD( "Organ Part" );
		ADD( "Bender Range" );
		break;
	case 0x16:
		ADD( "Organ Part" );
		ADD( "Aftertouch Switch" );
		break;
	case 0x17:
		ADD( "Bend/Mod Organ Control Switch" );
		break;
	case 0x18:
		ADD( "Orch" );
		ADD( "Reverb" );
		ADD( "Character" );
		break;
	case 0x19:
		ADD( "Orch" );
		ADD( "Reverb" );
		ADD( "Time" );
		break;
	case 0x1a:
		ADD( "Orch" );
		ADD( "Delay" );
		ADD( "Time" );
		break;
	case 0x1b:
		ADD( "Orch" );
		ADD( "Delay" );
		ADD( "Feedback" );
		break;
	default:
		ERRu();
		break;
	}
}

static void HandlePart_(
	address__ Address,
	descriptions__ &Descriptions )
{
	switch( Address &0xff ) {
	case 0x00:
		ADD( "Keyboard Assign" );
		ADD( "Organ" );
		break;
	case 0x01:
		ADD( "Keyboard Orch" );
		ADD( "Organ" );
		break;
	case 0x02:
		ADD( "Keyboard Ext" );
		ADD( "Organ" );
		break;
	case 0x03:
		ADD( "Organ Hold Switch" );
		break;
	case 0x04:
		ADD( "Orch" );
		ADD( "Part Level" );
		break;
	case 0x05:
		ADD( "Orch" );
		ADD( "Hold Switch" );
		break;
	case 0x06:
		ADD( "Orch" );
		ADD( "Bend Switch" );
		break;
	case 0x07:
		ADD( "Orch" );
		ADD( "Modulation Switch" );
		break;
	case 0x08:
		ADD( "Orch" );
		ADD( "Expression Switch" );
		break;
	case 0x09:
		ADD( "Orch" );
		ADD( "Voice Select" );
		break;
	case 0x0a:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Strings" );
		break;
	case 0x0b:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Choir" );
		break;
	case 0x0c:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Piano" );
		break;
	case 0x0d:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Bass" );
		break;
	case 0x0e:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Wind/Brass" );
		break;
	case 0x0f:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Attack" );
		break;
	case 0x10:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Synth" );
		break;
	case 0x11:
		ADD( "Orch" );
		ADD( "Variation" );
		ADD( "Others" );
		break;
	case 0x12:
		ADD( "Ext" );
		ADD( "Tone Select" );
		ADD( "PC Change Send Switch" );
		break;
	case 0x13:
		ADD( "Ext" );
		ADD( "Tone Select" );
		ADD( "Bank Select Send Switch" );
		break;
	case 0x14:
		ADD( "Ext" );
		ADD( "Tone Select" );
		ADD( "PC Number" );
		break;
	case 0x15:
		ADD( "Ext" );
		ADD( "Tone Select" );
		ADD( "Bank Select" );
		ADD( "MSB" );
		break;
	case 0x16:
		ADD( "Ext" );
		ADD( "Tone Select" );
		ADD( "Bank Select" );
		ADD( "LSB" );
		break;
	case 0x17:
		ADD( "Ext" );
		ADD( "Key Shift" );
		break;
	case 0x18:
		ADD( "Ext" );
		ADD( "Velocity" );
		ADD( "Sense" );
		break;
	case 0x19:
		ADD( "Ext" );
		ADD( "Velocity" );
		ADD( "Offset" );
		break;
	case 0x1a:
		ADD( "Ext" );
		ADD( "Volume Switch" );
		break;
	case 0x1b:
		ADD( "Ext" );
		ADD( "Hold Switch" );
		break;
	case 0x1c:
		ADD( "Ext" );
		ADD( "Expression Switch" );
		break;
	case 0x1d:
		ADD( "Ext" );
		ADD( "Modulation Switch" );
		break;
	case 0x1e:
		ADD( "Ext" );
		ADD( "Bender" );
		ADD( "Switch" );
		break;
	case 0x1f:
		ADD( "Ext" );
		ADD( "Bender" );
		ADD( "Range" );
		break;
	case 0x20:
		ADD( "Ext" );
		ADD( "Aftertouch Switch" );
		break;
	case 0x21:
		ADD( "Ext" );
		ADD( "Panpot" );
		ADD( "Switch" );
		break;
	case 0x22:
		ADD( "Ext" );
		ADD( "Panpot" );
		ADD( "Value" );
		break;
	case 0x23:
		ADD( "Ext" );
		ADD( "Coarse Tune" );
		ADD( "Transmit Switch" );
		break;
	case 0x24:
		ADD( "Ext" );
		ADD( "Coarse Tune" );
		break;
	case 0x25:
		ADD( "Ext" );
		ADD( "Fine Tune" );
		ADD( "Transmit Switch" );
		break;
	case 0x26:
		ADD( "Ext" );
		ADD( "Fine Tune" );
		break;
	case 0x27:
		ADD( "Ext" );
		ADD( "Reverb" );
		ADD( "Transmit Switch" );
		break;
	case 0x28:
		ADD( "Ext" );
		ADD( "Reverb" );
		ADD( "Level" );
		break;
	case 0x29:
		ADD( "Ext" );
		ADD( "Chorus" );
		ADD( "Transmit Switch" );
		break;
	case 0x2A:
		ADD( "Ext" );
		ADD( "Chorus" );
		ADD( "Level" );
		break;
	default:
		ERRu();
		break;
	}
}

static void HandleBasic_(
	address__ Address,
	descriptions__ &Descriptions )
{
	switch( Address &0xff ) {
	case 0x00:
		ADD( "Organ" );
		ADD( "Level" );
		break;
	case 0x01:
		ADD( "Organ" );
		ADD( "Wheel Type" );
		break;
	case 0x02:
		ADD( "Fold Back Switch" );
		break;
	case 0x03:
		ADD( "Wheel Level Table" );
		break;
	case 0x04:
		ADD( "Leakage Level" );
		break;
	case 0x05:
		ADD( "Key Click Level" );
		ADD( "On" );
		break;
	case 0x06:
		ADD( "Key Click Level" );
		ADD( "Off" );
		break;
	case 0x07:
		ADD( "Percussion" );
		ADD( "Switch" );
		break;
	case 0x08:
		ADD( "Percussion" );
		ADD( "Harmonic" );
		break;
	case 0x09:
		ADD( "Percussion" );
		ADD( "Soft" );
		break;
	case 0x0a:
		ADD( "Percussion" );
		ADD( "Decay" );
		break;
	case 0x0b:
		ADD( "Percussion" );
		ADD( "Level" );
		ADD( "Soft" );
		break;
	case 0x0c:
		ADD( "Percussion" );
		ADD( "Level" );
		ADD( "Normal" );
		break;
	case 0x0d:
		ADD( "Percussion" );
		ADD( "Decay Time" );
		ADD( "Slow" );
		break;
	case 0x0e:
		ADD( "Percussion" );
		ADD( "Decay Time" );
		ADD( "Fast" );
		break;
	case 0x0f:
		ADD( "Percussion" );
		ADD( "Recharge Time" );
		break;
	case 0x10:
		ADD( "Percussion" );
		ADD( "1' Cancel" );
		break;
	case 0x11:
		ADD( "Percussion" );
		ADD( "H.Bar Level" );
		break;
	case 0x12:
		ADD( "Percussion" );
		ADD( "Direct Switch" );
		break;
	case 0x13:
		ADD( "Upper" );
		ADD( "H.Bar 16'" );
		break;
	case 0x14:
		ADD( "Upper" );
		ADD( "H.Bar 5-1/3'" );
		break;
	case 0x15:
		ADD( "Upper" );
		ADD( "H.Bar 8'" );
		break;
	case 0x16:
		ADD( "Upper" );
		ADD( "H.Bar 4'" );
		break;
	case 0x17:
		ADD( "Upper" );
		ADD( "H.Bar 2-2/3'" );
		break;
	case 0x18:
		ADD( "Upper" );
		ADD( "H.Bar 2'" );
		break;
	case 0x19:
		ADD( "Upper" );
		ADD( "H.Bar 1-3/5'" );
		break;
	case 0x1a:
		ADD( "Upper" );
		ADD( "H.Bar 1-1/3'" );
		break;
	case 0x1b:
		ADD( "Upper" );
		ADD( "H.Bar 1'" );
		break;
	case 0x1c:
		ADD( "Lower" );
		ADD( "H.Bar 16'" );
		break;
	case 0x1d:
		ADD( "Lower" );
		ADD( "H.Bar 5-1/3'" );
		break;
	case 0x1e:
		ADD( "Lower" );
		ADD( "H.Bar 8'" );
		break;
	case 0x1f:
		ADD( "Lower" );
		ADD( "H.Bar 4'" );
		break;
	case 0x20:
		ADD( "Lower" );
		ADD( "H.Bar 2-2/3'" );
		break;
	case 0x21:
		ADD( "Lower" );
		ADD( "H.Bar 2'" );
		break;
	case 0x22:
		ADD( "Lower" );
		ADD( "H.Bar 1-3/5'" );
		break;
	case 0x23:
		ADD( "Lower" );
		ADD( "H.Bar 1-1/3'" );
		break;
	case 0x24:
		ADD( "Lower" );
		ADD( "H.Bar 1'" );
		break;
	case 0x25:
		ADD( "Pedal" );
		ADD( "Setting" );
		ADD( "H.Bar 16'" );
		break;
	case 0x26:
		ADD( "Pedal" );
		ADD( "Setting" );
		ADD( "H.Bar 8'" );
		break;
	case 0x27:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "16'" );
		break;
	case 0x28:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "5-1/3'" );
		break;
	case 0x29:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "8'" );
		break;
	case 0x2a:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "4'" );
		break;
	case 0x2b:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "2-2/3'" );
		break;
	case 0x2c:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "2'" );
		break;
	case 0x2d:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "1-3/5'" );
		break;
	case 0x2e:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "1-1/3'" );
		break;
	case 0x2f:
		ADD( "Pedal" );
		ADD( "16' H.Bar Mixing Ratio" );
		ADD( "1'" );
		break;
	case 0x30:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "16'" );
		break;
	case 0x31:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "5-1/3'" );
		break;
	case 0x32:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "8'" );
		break;
	case 0x33:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "4'" );
		break;
	case 0x34:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "2-2/3'" );
		break;
	case 0x35:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "2'" );
		break;
	case 0x36:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "1-3/5'" );
		break;
	case 0x37:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "1-1/3'" );
		break;
	case 0x38:
		ADD( "Pedal" );
		ADD( "8' H.Bar Mixing Ratio" );
		ADD( "1'" );
		break;
	case 0x39:
		ADD( "Pedal" );
		ADD( "Sustain" );
		ADD( "Switch" );
		break;
	case 0x3a:
		ADD( "Pedal" );
		ADD( "Sustain" );
		ADD( "Time" );
		break;
	case 0x3b:
		ADD( "Pedal" );
		ADD( "Attack" );
		ADD( "Switch" );
		break;
	case 0x3c:
		ADD( "Pedal" );
		ADD( "Attack" );
		ADD( "Time" );
		break;
	case 0x3d:
		ADD( "Pedal" );
		ADD( "Attack" );
		ADD( "Level" );
		break;
	default:
		ERRu();
		break;
	}
}


static void HandleEffect_(
	address__ Address,
	descriptions__ &Descriptions )
{
	switch( Address & 0xff ) {
	case 0x00:
		ADD( "Vibrato/Chorus" );
		ADD( "Switch" );
		ADD( "Upper" );
		break;
	case 0x01:
		ADD( "Vibrato/Chorus" );
		ADD( "Switch" );
		ADD( "Lower" );
		break;
	case 0x02:
		ADD( "Vibrato/Chorus" );
		ADD( "Type" );
		break;
	case 0x03:
		ADD( "Vibrato/Chorus" );
		ADD( "Vintage" );
		break;
	case 0x04:
		ADD( "Vibrato/Chorus" );
		ADD( "Lower" );
		break;
	case 0x05:
		ADD( "Amp & Speaker Type" );
		break;
	case 0x06:
		ADD( "Overdrive" );
		ADD( "Character" );
		break;
	case 0x07:
		ADD( "Overdrive" );
		ADD( "Level" );
		break;
	case 0x08:
		ADD( "Equalize Gain" );
		ADD( "Bass" );
		break;
	case 0x09:
		ADD( "Equalize Gain" );
		ADD( "Middle" );
		break;
	case 0x0a:
		ADD( "Equalize Gain" );
		ADD( "Treble" );
		break;
	case 0x0b:
		ADD( "Rotary" );
		ADD( "Sound" );
		break;
	case 0x0c:
		ADD( "Rotary" );
		ADD( "Speed" );
		break;
	case 0x0d:
		ADD( "Rotary" );
		ADD( "Rotation" );
		break;
	case 0x0e:
		ADD( "Rotary" );
		ADD( "Type" );
		break;
	case 0x0f:
		ADD( "Rotary" );
		ADD( "Level" );
		ADD( "Woofer" );
		break;
	case 0x10:
		ADD( "Rotary" );
		ADD( "Level" );
		ADD( "Tweeter" );
		break;
	case 0x11:
		ADD( "Rotary" );
		ADD( "Woofer" );
		ADD( "Rise Time" );
		break;
	case 0x12:
		ADD( "Rotary" );
		ADD( "Woofer" );
		ADD( "Fall Time" );
		break;
	case 0x13:
		ADD( "Rotary" );
		ADD( "Tweeter" );
		ADD( "Rise Time" );
		break;
	case 0x14:
		ADD( "Rotary" );
		ADD( "Tweeter" );
		ADD( "Fall Time" );
		break;
	case 0x15:
		ADD( "Rotary" );
		ADD( "Woofer Speed" );
		ADD( "Slow" );
		break;
	case 0x16:
		ADD( "Rotary" );
		ADD( "Woofer Speed" );
		ADD( "Fast" );
		break;
	case 0x17:
		ADD( "Rotary" );
		ADD( "Tweeter Speed" );
		ADD( "Slow" );
		break;
	case 0x18:
		ADD( "Rotary" );
		ADD( "Tweeter Speed" );
		ADD( "Fast" );
		break;
	case 0x19:
		ADD( "Rotary" );
		ADD( "Mic Distance" );
		break;
	case 0x1a:
		ADD( "Rotary" );
		ADD( "Spread" );
		ADD( "Woofer" );
		break;
	case 0x1b:
		ADD( "Rotary" );
		ADD( "Spread" );
		ADD( "Tweeter" );
		break;
	case 0x1c:
		ADD( "Rotary" );
		ADD( "Randomize" );
		break;
	case 0x1d:
		ADD( "Ring Modulator" );
		ADD( "Switch" );
		break;
	case 0x1e:
		ADD( "Ring Modulator" );
		ADD( "Mode" );
		break;
	case 0x1f:
		ADD( "Ring Modulator" );
		ADD( "Frequency" );
		break;
	case 0x20:
		ADD( "Organ" );
		ADD( "Reverb" );
		ADD( "Structure" );
		break;
	case 0x21:
		ADD( "Organ" );
		ADD( "Reverb" );
		ADD( "Type" );
		break;
	case 0x22:
		ADD( "Organ" );
		ADD( "Reverb" );
		ADD( "Level" );
		break;
	case 0x23:
		ADD( "Organ" );
		ADD( "Reverb" );
		ADD( "Time" );
		break;
	case 0x24:
		ADD( "Organ" );
		ADD( "Delay" );
		ADD( "Type" );
		break;
	case 0x25:
		ADD( "Organ" );
		ADD( "Delay" );
		ADD( "Level" );
		break;
	case 0x26:
		ADD( "Organ" );
		ADD( "Delay" );
		ADD( "Feedback" );
		break;
	default:
		ERRu();
		break;
	}
}


static void HandleRegistration_(
	address__ Address,
	descriptions__ &Descriptions,
	bso::bool__ Flat )
{
	switch( ( Address >> 8 ) & 0x7f ) {
	case 0x00:
		ADD( "Common" );
		HandleCommon_( Address, Descriptions, Flat );
		break;
	case 0x10:
		ADD( "Upper" );
		HandlePart_( Address, Descriptions );
		break;
	case 0x12:
		ADD( "Lower" );
		HandlePart_( Address, Descriptions );
		break;
	case 0x14:
		ADD( "Pedal" );
		HandlePart_( Address, Descriptions );
		break;
	case 0x40:
		ADD( "Basic" );
		HandleBasic_( Address, Descriptions );
		break;
	case 0x50:
		ADD( "Effect" );
		HandleEffect_( Address, Descriptions );
		break;
	default:
		ERRu();
		break;
	}
}

	
void mscvkp::GetDescription(
	address__ Address,
	descriptions__ &Descriptions,
	bso::bool__ Flat )
{
	switch( Address >> 24 ) {
	case 0x00:
		ADD( "System" );
		HandleSystem_( Address, Descriptions );
		break;
	case 0x01:
		ADD( "Registration" );
		ADD( "Temporary" );
		HandleRegistration_( Address, Descriptions, Flat );
		break;
	case 0x10:
		ADD( "Registration" );
		ADD( GetRegistrationLabel( Address ) );
		HandleRegistration_( Address, Descriptions, Flat );
		break;
	case 0x20:
		ADD( "Orchestral Voice" );
		HandleOrchestral_( Address, Descriptions );
		break;
	default:
		ERRu();
		break;
	}
}

#include "windows.h"

static void BuildHeader_(
	const data_set_ &DataSet,
	str::string_ &Header )
{
	Header.Append( "\x41\x10\x00\x1a\x12", 5 );	//'(...,5)' becuase of the '\0'.
	Header.Append( (char)( DataSet.Address() >> 24 ) );
	Header.Append( (char)( ( DataSet.Address() >> 16 ) & 0xff ) );
	Header.Append( (char)( ( DataSet.Address() >> 8 ) & 0xff ) );
	Header.Append( (char)( DataSet.Address() & 0xff ) );
}

bso::size__ mscvkp::Send(
	const data_set_ &DataSet,
	flw::oflow__ &Flow )
{
	bso::size__ Length = 0;
qRH
	str::string Header;
	epeios::row__ Row = NONE;
qRB

	Flow.Put( '\xf0' );
	Length += 1;

	Header.Init();
	BuildHeader_( DataSet, Header );

	Row = Header.First();

	while ( Row != NONE ) {
		Flow.Put( Header( Row ) );

		Row = Header.Next( Row );
	}

	Length += Header.Amount();

	Row = DataSet.First();

	while ( Row != NONE ) {
		Flow.Put( DataSet( Row ) );

		Row = DataSet.Next( Row );
	}

	Length += DataSet.Amount();

	Flow.Put( Checksum( DataSet ) );
	Flow.Put( '\xf7' );

	Length += 2;
qRR
qRT
qRE
	return Length;
}

void mscvkp::Send(
	const data_sets_ &DataSets,
	flw::oflow__ &Flow )
{
	epeios::row__ Row = DataSets.First();
	ctn::E_CMITEM( data_set_ ) DataSet;
	bso::size__ Length = 0;

	DataSet.Init( DataSets );

	while ( Row != NONE ) {
		Length += Send( DataSet( Row ), Flow );

		if ( Length > 128 ) {
			Sleep( 40 );
			Length = 0;
		}

		Flow.Synchronize();

		Row = DataSets.Next( Row );
	}
}

static void BuildEvent_(
	const data_set_ &DataSet,
	mscmdm::delta_time_ticks__ DeltaTimeTicks,
	mscmdm::event_ &Event )
{
qRH
	mscmdm::event_header__ EventHeader;
	mscmdm::data Data;
qRB
	EventHeader.DeltaTimeTicks = DeltaTimeTicks;
	EventHeader.EventType = mscmdm::etSystem;
	EventHeader.Id = 0xf0;	// 'system exclusive'.

	EventHeader.SystemEvent.Event = mscmdm::sysExclusive;

	Data.Init();
	BuildHeader_( DataSet, Data );

	Data.Append( DataSet );
	Data.Append( Checksum( DataSet ) );
//	Data.Append( '\xf7' );	// 'end of system exclusive'.

	Event.Init( EventHeader, Data );
qRR
qRT
qRE
}

void mscvkp::BuildTrack(
	const data_sets_ &DataSets,
	mscmdm::track_ &Track )
{
qRH
	epeios::row__ Row = NONE;
	ctn::E_CMITEM( data_set_ ) DataSet;
	mscmdm::event Event;
	bso::ulong__ Length = 0;
qRB
	Row = DataSets.First();

	DataSet.Init( DataSets );

	if ( Row != NONE ) {
			BuildEvent_( DataSet( Row ), 1, Event );

			Track.Append( Event );

			Length += Event.Data.Amount();

			Row = DataSets.Next( Row );
	}

	while ( Row != NONE ) {
		Event.Init();

		if ( ( Length + DataSet( Row ).Amount() ) < 110 ){
			BuildEvent_( DataSet( Row ), 0, Event );
		} else {
			BuildEvent_( DataSet( Row ), 5, Event );
			Length = 0;
		}

		Track.Append( Event );

		Length += Event.Data.Amount();

		Row = DataSets.Next( Row );
	}
qRR
qRT
qRE
}
