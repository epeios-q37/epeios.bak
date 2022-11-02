/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'mscfdraftq' tool.

    'mscfdraftq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mscfdraftq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mscfdraftq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "midiq.h"

#include "melody.h"
#include "main.h"
#include "registry.h"

#include "mscmdm.h"

#include "sclm.h"

#include "sclx.h"

using namespace melody;

using namespace mscmdm;
using namespace mscmld;

using namespace midiq;

namespace {
  bso::sS8 Handle_(bso::sU8 Pitch)
  {
    bso::sS8 OctaveOverflow = 0;
  qRH
    hGuard Guard;
  qRB;
    melody::rXMelody &XMelody = melody::Get(Guard);

    OctaveOverflow = melody::Handle(sNote(Pitch, sDuration(3), XMelody.Signature), XMelody);
  qRR;
  qRT;
  qRE;
    return OctaveOverflow;
  }
}

void midiq::HandleInput(
  void *UP,
  mtk::gBlocker &Blocker)
{
qRH;
	sEventHeader Header;
  wData Data;
  wEvent Event;
	flw::rWFlow &Flow = flx::VoidOFlow;
	bso::pInt Buffer;
	str::wString MIDIDeviceIn;
qRB;
	sShared &Shared = *(sShared *)UP;
	mscmdd::rRFlow &RFlow = *Shared.RFlow;
	MIDIDeviceIn.Init(*Shared.MIDIDeviceIn);

	Blocker.Release();

	if ( MIDIDeviceIn.Amount() ) {
    RFlow.Init(MIDIDeviceIn);

    RFlow.Start();

    while ( 1 ) {
      GetEventHeader( RFlow, Header );

      Data.Init();
      GetEventData( Header, RFlow, Data );

      Event.Init( Header, Data );

      PutEvent( Event, xNone, Flow );
      Flow.Commit();

      if ( Header.EventType == etMIDI )
        if ( Header.MIDIEvent.Event == midNoteOn )
          if  ( Data( Data.Last() ) != 0 ) {
            if ( Handle_(Data(Data.First())) == 0 )
              sclx::Broadcast(str::wString("Hit"), str::wString(bso::Convert(Data(Data.First()), Buffer)));
  //					mtx::Lock( Shared.Mutex );
            /*if ( Shared.Row != qNIL ) {
              Shared.Melody->InsertAt(Note, Shared.Row);
              Shared.Row = Shared.Melody->Next( Shared.Row );
            }else
              Shared.Melody->Append( Note );

            melody::PrintSignature(cio::COut);
            cio::COut << ": " << txf::commit;
            Print(*Shared.Melody, Shared.Row, cio::COut);
  //					mtx::Unlock( Shared.Mutex );
  */
          }
    }
	}
qRR;
qRT;
qRE;
}

static bso::sU8 GetAbsolute_(const sAltPitch &Pitch)
{
	if ( ( BSO_U8_MAX / 12 ) < Pitch.Octave )
		qRGnr();

	bso::sU8 Absolute = Pitch.Octave * 12;

	switch ( Pitch.Name ) {
	case pnA:
		Absolute += 9;
		break;
	case pnB:
		Absolute += 11;
		break;
	case pnC:
		Absolute += 0;
		break;
	case pnD:
		Absolute += 2;
		break;
	case pnE:
		Absolute += 4;
		break;
	case pnF:
		Absolute += 5;
		break;
	case pnG:
		Absolute += 7;
		break;
	case pnRest:
		break;
	default:
		qRGnr();
		break;
	}

	switch ( Pitch.Accidental ) {
		case aFlat:
			if ( Absolute == 0 )
				qRGnr();
			Absolute--;
			break;
		case aNatural:
			break;
		case aSharp:
			if ( Absolute == BSO_U8_MAX )
				qRGnr();
			Absolute++;
			break;
		default:
			qRGnr();
			break;
	}

	return Absolute;
}

static bso::sU32 ComputeTime_(
	sDuration Duration,
	bso::sU32 Base )	// Base : duration (in ms) of a whole.
{
	bso::sU32 Time = Base;

	switch ( Duration.Base ) {
	case 1 :
		break;
	case 2 :
		Time >>= 1;
		break;
	case 3 :
		Time >>= 2;
		break;
	case 4 :
		Time >>= 3;
		break;
	case 5 :
		Time >>= 4;
		break;
	case 6 :
		Time >>= 5;
		break;
	case 7 :
		Time >>= 6;
		break;
	case 8 :
		Time >>= 7;
		break;
	case 9 :
		Time >>= 8;
		break;
	default:
		qRGnr();
		break;
	}

	switch ( Duration.Modifier ) {
	case 0:
		break;
	case 1:
		Time += ( Time >> 1 );
		break;
	case 2:
		Time += ( Time >> 1 ) + ( Time >> 2 );
		break;
	case 3:
		Time += ( Time >> 1 ) + ( Time >> 2 ) + ( Time >> 3 );
		break;
	default:
		qRGnr();
		break;
	}

	return Time;
}

void midiq::Play(
	const sNote &Note,
	bso::sU32 Base,
	bso::sBool Tied,
	flw::oflow__ &Flow )
{
qRH;
	mscmdm::sEventHeader Header;
	mscmdm::wData Data;
	mscmdm::wEvent Event;
	sTempo Tempo;
qRB;
	Header.Id = 144;
	Header.EventType = mscmdm::etMIDI;
	Header.DeltaTimeTicks = 0;
	Header.MIDIEvent.ChannelID = 0;
	Header.MIDIEvent.Tied = false;
	Header.MIDIEvent.Event = mscmdm::midNoteOn;

	Data.Init();

	Data.Append(Note.Pitch);
	Data.Append(120);

	Event.Init( Header, Data );

	if ( !Tied ) {
		mscmdm::PutEvent( Event, mscmdm::xNone, Flow );
		Flow.Commit();
	}

	tht::Suspend( ComputeTime_( Note.Duration, Base ) );

	Data.Store( 0, Data.Last() );
	Event.Init( Header, Data );

	if ( !Tied ) {
		mscmdm::PutEvent( Event, mscmdm::xNone, Flow );
		Flow.Commit();
	}
qRR;
qRT;
qRE;
}



