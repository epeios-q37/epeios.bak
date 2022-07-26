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

#define MSCMDD_COMPILATION_

#include "mscmdd.h"

using namespace mscmdd;

#include "mscmdm.h"

#ifdef MSCMDD__ALSA

#endif

#ifdef MSCMDD__WINDOWS
static void Fill_(
	const char *Buffer,
	bso::size__ Amount,
	rData_ &Data )
{
	bso::bool__ Wait = false;
	bso::size__ AwareAmount = 0;

	if ( Data.Purge )	// Pas de protection; c'est un boolen uniquement accd en lecture.
		return;

	while ( Amount != 0 ) {
		mtx::Lock( Data.Full );	// On attend tant que 'Data.Buffer' est plein.
		mtx::Unlock( Data.Full );

		mtx::Lock( Data.Access ) ;

		AwareAmount = Emptyness_( Data );

		if ( AwareAmount > Amount )
			AwareAmount = Amount;

		memcpy( Data.Buffer + Data.Available + Data.Position, Buffer, AwareAmount );

		if ( IsEmpty_( Data ) )
			mtx::Unlock( Data.Empty );	// Si 'Data.Buffer' tait vide, on signale au consommateur que ce n'est plus le cas.

		Amount -= AwareAmount;
		Buffer += AwareAmount;

		Data.Available += AwareAmount;

		if( IsFull_( Data ) )
			mtx::Lock( Data.Full );

		mtx::Unlock( Data.Access );
	}
}

static void CALLBACK MidiInProc_(
  HMIDIIN hMidiIn,
  UINT wMsg,
  DWORD dwInstance,
  DWORD dwParam1,
  DWORD dwParam2 )
{
qRH
	rData_ &Data = *(rData_ *)dwInstance;
	bso::u8__ Event = 0;
	MIDIHDR *Header = NULL;
	bso::bool__ Wait = false;
qRB
	switch( wMsg ) {
	case MIM_DATA:
		char Buffer[3];
		Buffer[0] = dwParam1 & 0xff;
		switch ( mscmdm::DetermineEvent( dwParam1 & 0xff, ( dwParam1 & 0xff00 ) >> 8, Event ) ) {
		case mscmdm::etMIDI:
			switch ( mscmdm::GetMIDIEventDataSize( (mscmdm::eMidiEvent)Event ) ) {
			case 0:
				Fill_( Buffer, 1, Data );
				break;
			case 1:
				Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
				Fill_( Buffer, 2, Data );
				break;
			case 2:
				Buffer[1] = ( ( dwParam1 & 0xff00 ) >> 8 );
				Buffer[2] =(fdr::sByte)( ( dwParam1 & 0xff0000 ) >> 16 );
				Fill_( Buffer, 3, Data );
				break;
			default:
				qRFwk();
				break;
			}
			break;
		case mscmdm::etSystem:
			Fill_( Buffer, 1, Data );
			break;
		default:
			qRFwk();
			break;
		}
		break;
	case MIM_OPEN:
		break;
	case MIM_LONGDATA:
		Header = (MIDIHDR *)dwParam1;

		Fill_( Header->lpData, Header->dwBytesRecorded, Data );

		Header->dwUser = 0;

		break;
	case MIM_CLOSE:
		break;
	default:
		qRSys();
		break;
	}


qRR
qRT
qRE
}

fdr::size__ mscmdd::rIn::Read(
	fdr::size__ Maximum,
	fdr::sByte *Buffer )
{
	bso::u8__ Amount = sizeof( _Header ) / sizeof( *_Header );

	while ( Amount-- ) {
		if( _Header[Amount].dwUser == 0 ) {
			_Header[Amount].lpData = _HeaderBuffer[Amount];
			_Header[Amount].dwUser = _Header[Amount].dwBufferLength = sizeof( _HeaderBuffer ) / sizeof( *_HeaderBuffer );

			if ( midiInUnprepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				qRSys();

			_Header[Amount].dwFlags = 0;

			if ( midiInPrepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				qRSys();

			if ( midiInAddBuffer( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
				qRSys();
		}
	}

	mtx::Lock( _Data.Access );

	if ( IsEmpty_( _Data ) ) {
		if ( !_Started ) {
				if ( midiInStart( _Handle ) != MMSYSERR_NOERROR )
					qRFwk();
			_Started = true;
		}

		mtx::Unlock( _Data.Access );

		mtx::Lock( _Data.Empty );	// On attend si ncessaire que des donnes soient didponibles.
		mtx::Unlock( _Data.Empty );

		mtx::Lock( _Data.Access );
	}

	// Bien que '_Data.Empty' ne soit pas verrouilles  ce point, il peut ne pas y avoir de donnes disponibles, si toutes les donnes ont t lues.

	if ( Maximum > _Data.Available )
		Maximum = _Data.Available;

	if ( IsFull_( _Data ) )
		mtx::Unlock( _Data.Full );	// Si '_Data.Buffer' tait plein, on signale au producteur que ce n'est plus le cas.

	memcpy( Buffer, _Data.Buffer + _Data.Position, Maximum );

	_Data.Available -= Maximum;
	_Data.Position += Maximum;

	if ( _Data.Available == 0 ) {
		_Data.Position = 0;
		mtx::Lock( _Data.Empty );
	}

	mtx::Unlock( _Data.Access );

	return Maximum;
}

bso::bool__ mscmdd::rIn::Init(
	int Device,
	err::handling__ ErrHandling )
{
	bso::u8__ Amount = sizeof( _Header ) / sizeof( *_Header );
	reset();

	// '_Data' n'est pas initialis, mais ce n'est pas grave, car ne sera ps utilis tant qu'un 'Start' n'aura pas t lanc.
	if ( midiInOpen( &_Handle, Device, (DWORD)MidiInProc_, (DWORD)&_Data, CALLBACK_FUNCTION ) != MMSYSERR_NOERROR ) {
		if ( ErrHandling != err::hUserDefined )
			qRFwk();
		else
			return false;
	}

	_Data.Access = mtx::Create();
	_Data.Full = mtx::Create();
	_Data.Empty = mtx::Create();
	_Data.Purge = false;
	_Data.Buffer = _Cache;

	// '_Data' seulement initialis maintenant pour signaler la russite de l'opration d'ouverture.

	while( Amount-- ) {
		_Header[Amount].lpData = _HeaderBuffer[Amount];
		_Header[Amount].dwUser = _Header[Amount].dwBufferLength = sizeof( _HeaderBuffer ) / sizeof( *_HeaderBuffer );
		_Header[Amount].dwFlags = 0;

		if ( midiInPrepareHeader( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
			qRSys();

		if ( midiInAddBuffer( _Handle, &_Header[Amount], sizeof( _Header[Amount] ) ) != MMSYSERR_NOERROR )
			qRSys();
	}

	_Data.Size = sizeof( _Cache ) / sizeof( *_Cache );

	mtx::Lock( _Data.Empty );

	return true;
}

static void Convert_(
	const WCHAR *WString,
	str::dString &Name )
{
	while ( *WString ) {
		Name.Append( (char)*WString );

		WString++;
	}

}

// Pour g++ sous 'cygwin'.
static inline void Convert_(
	const CHAR *RawString,
	str::dString &Name )
{
	Name.Append( RawString );
}
#endif

#ifdef MSCMDD__ALSA
static bso::sSize GetMIDIDevices_(
	str::dStrings &Ids,
	str::dStrings &Names,
	snd_rawmidi_stream_t Direction )
{
	bso::sSize Count = 0;
qRH
	str::string Id, Name;
	bso::integer_buffer__ Buffer;
	int	cardNum = -1;	// Start with first card
	snd_ctl_t *cardHandle = NULL;
	char str[64] = "";
	int devNum = -1;	// Start with the first MIDI device.
	snd_rawmidi_info_t *rawMidiInfo = NULL;
qRB
	for (;;) {
		// Get next sound card's card number. When "cardNum" == -1, then ALSA
		// fetches the first card
		if ( snd_card_next (&cardNum ) < 0)
			qRSys();

		// No more cards? ALSA sets "cardNum" to -1 if so
		if ( cardNum < 0 ) break;

		// Open this card's control interface. We specify only the card number -- not
		// any device nor sub-device too
		sprintf(str, "hw:%i", cardNum);

		if ( snd_ctl_open(&cardHandle, str, 0) < 0)
			qRSys();

		// Start with the first MIDI device on this card
		devNum = -1;

		for (;;) {
			// Get the number of the next MIDI device on this card
			if ( snd_ctl_rawmidi_next_device(cardHandle, &devNum) )
				qRSys();

			// No more MIDI devices on this card? ALSA sets "devNum" to -1 if so.
			// NOTE: It's possible that this sound card may have no MIDI devices on it
			// at all, for example if it's only a digital audio card
			if (devNum < 0) break;

			// To get some info about the subdevices of this MIDI device (on the card), we need a
			// snd_rawmidi_info_t, so let's allocate one on the stack
			snd_rawmidi_info_alloca(&rawMidiInfo);
			memset(rawMidiInfo, 0, snd_rawmidi_info_sizeof());

			// Tell ALSA which device (number) we want info about
			snd_rawmidi_info_set_device(rawMidiInfo, devNum);

			// Get info on the MIDI outs of this device
			snd_rawmidi_info_set_stream(rawMidiInfo, Direction);

			snd_rawmidi_info_set_subdevice(rawMidiInfo, 0);

			if ( snd_ctl_rawmidi_info(cardHandle, rawMidiInfo) < 0 )
				qRSys();

			Id.Init("hw:");
			Id.Append( bso::Convert( (bso::sUInt)cardNum, Buffer ) );
			Id.Append( ',' );
			Id.Append( bso::Convert( (bso::sUInt)devNum, Buffer ) );
			Ids.Append(Id);

/*			Name.Append( ' ' );
			Name.Append( bso::Convert((bso::sUInt)snd_rawmidi_info_get_subdevices_count(rawMidiInfo), Buffer ) );
			Name.Append( ") " );
*/			Name.Init(snd_rawmidi_info_get_name( rawMidiInfo ));
			Name.Append( ", " );
			Name.Append(snd_rawmidi_info_get_id( rawMidiInfo ));

			Names.Append( Name );

			Count++;
		}
		// Close the card's control interface after we're done with it
		snd_ctl_close(cardHandle);
	}

	snd_config_update_free_global();
qRR
qRT
qRE
	return Count;
}

bso::bool__ mscmdd::GetMIDIInDeviceName(
	int Device,
	str::dString &Name )
{
	bso::bool__ Success = false;
qRH
	str::wStrings Ids, Names;
qRB
  tol::Init(Ids, Names);

  if ( GetMIDIDevices_(Ids,  Names, SND_RAWMIDI_STREAM_INPUT) > Device ) {
    Success = true;

    Name = Names( Device );
   }
qRR
qRT
qRE
	 return Success;
}

bso::bool__ mscmdd::GetMIDIOutDeviceName(
	int Device,
	str::string_ &Name )
{
	bso::bool__ Success = false;
qRH
	str::wStrings Ids, Names;
qRB
  tol::Init(Ids, Names);

  if ( GetMIDIDevices_(Ids,  Names, SND_RAWMIDI_STREAM_OUTPUT) > Device ) {
    Success = true;

    Name = Names( Device );
   }
qRR
qRT
qRE
	 return Success;
}

#endif


bso::sSize mscmdd::GetMidiInDeviceNames(
  str::dStrings &Ids,
  str::dStrings &Names)
{
	bso::sSize Count = 0;
#ifdef MSCMDD__WINDOWS
qRH
	MIDIINCAPS InCaps;
	bso::u32__ Counter = 0;
	wName Name;
qRB
	Count =  midiInGetNumDevs();

	while ( Counter < Count ) {
		midiInGetDevCaps( Counter++, &InCaps, sizeof( InCaps ) );

		Name.Init();

		Convert_( InCaps.szPname, Name );

		Names.Append( Name );
	}
qRR
qRT
qRE
#elif defined( MSCMDD__ALSA )
	Count = GetMIDIDevices_(Ids, Names, SND_RAWMIDI_STREAM_INPUT);
#endif
	return Count;
}

bso::sSize mscmdd::GetMidiOutDeviceNames(
  str::dStrings &Ids,
  str::dStrings &Names )
{
	bso::sSize Count = 0;
#ifdef MSCMDD__WINDOWS
qRH
	MIDIOUTCAPS OutCaps;
	bso::u32__ Counter = 0;
	wName Name;
qRB
	Count =  midiOutGetNumDevs();

	while ( Counter < Count ) {
		midiOutGetDevCaps( Counter++, &OutCaps, sizeof( OutCaps ) );

		Name.Init();

		Convert_( OutCaps.szPname, Name );

		Names.Append( Name );
	}
qRR
qRT
qRE
#elif defined( MSCMDD__ALSA )
	Count = GetMIDIDevices_(Ids, Names, SND_RAWMIDI_STREAM_OUTPUT);
#endif
	return Count;
}

#define CASE( label )\
	case s##label:\
		return "MSCMDD_" #label;\
		break;

const char *mscmdd::Label( status__ Status )
{
	switch ( Status ) {
	CASE( UnableToOpenMIDIInDevice );
	CASE( UnableToOpenMIDIOutDevice );
	default:
		qRFwk();
		break;
	}

	return NULL;	// Pour viter un 'warning'.
}
