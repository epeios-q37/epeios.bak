/*
	Copyright (C) 2018 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MelDigQ'.

    'MelDigQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MelDigQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MelDigQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

# include "meldigq.h"

#include "registry.h"

#include "mscmdd.h"
#include "mscmdm.h"
#include "mscmld.h"
#include "mscmld.h"

#include "scltool.h"
#include "sclerror.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"

using cio::CErr;
using cio::COut;
using cio::CIn;

using namespace mscmdm;
using namespace mscmld;

SCLI_DEF( meldigq, NAME_LC, NAME_MC );

const scli::sInfo &scltool::SCLTOOLInfo( void )
{
	return meldigq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build : " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

static sPitchOctave GetOctave_( bso::sU8 Absolute )
{
	return Absolute / 12;
}

static ePitchName GetName_(
	bso::sU8 Absolute,
	sSignatureKey Key)
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
		qRGnr();
		break;
	}

	qRGnr();

	return pn_Undefined;	// To avoid a warning.
}

ePitchAccidental GetAccidental_(
	bso::sU8 Absolute,
	sSignatureKey Key )
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
		qRGnr();
		break;
	}

	qRGnr();

	return pa_Undefined;	// To avoid a wanrning.
}

sPitch GetPitch_(
	bso::sU8 Absolute,
	sSignatureKey Key )
{
	return sPitch( GetOctave_( Absolute ), GetName_( Absolute, Key ), GetAccidental_( Absolute, Key ) );
}

static void Display_( const dMelody &Melody )
{
qRH;
	xml::writer Writer;
qRB;
	Writer.Init( COut, xml::oIndent, xml::e_Default );
	
	WriteXML( Melody, Writer );

	COut << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}

static bso::sU8 GetAbsolute_( const sPitch &Pitch )
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
		case paFlat:
			if ( Absolute == 0 )
				qRGnr();
			Absolute--;
			break;
		case paNatural:
			break;
		case paSharp:
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

bso::sU32 ComputeTime_(
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

struct tempo__ {
	bso::sU8 Value;
	sDuration Unit;
	tempo__( void )
	{
		// Par défaut : 120 à la noire.
		Value = 120;
		Unit = sDuration( 4, 0 );
	}
};

static const sDuration &GetTempoUnit_( sDuration &Unit )
{
qRH;
	str::wString RawUnit;
	bso::bool__ Missing = false;
	sdr::sRow Error = qNIL;
	bso::sU8 Base = 0;
	sdr::sRow Row = qNIL;
	bso::sU8 Modifier = 0;
qRB;
	RawUnit.Init();

	Missing = sclmisc::OGetValue( registry::parameter::tempo::Unit, RawUnit );

	if ( Missing )
		qRReturn;

	Base = RawUnit.ToU8( &Error, str::b10, 9 );

	if ( Error != qNIL ) {
		if ( RawUnit( Error ) != '.' )
			sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

		Row = Error;

		while ( (Row != qNIL) && (RawUnit( Row ) == '.') ) {
			Modifier++;
			Row = RawUnit.Next( Row );

			if ( Modifier >= 4 )
				sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

			if ( Row != qNIL )
				sclrgstry::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );
		}
	}

	Unit = sDuration ( Base, Modifier );

qRR;
qRT;
qRE;
	return Unit;
}

static const tempo__ &GetTempo_( tempo__ &Tempo )
{
	Tempo.Value = sclmisc::MGetU8( registry::parameter::tempo::Value );
	GetTempoUnit_( Tempo.Unit );

	return Tempo;
}

bso::sU32 ComputeBase_( const tempo__ &Tempo )
{
	return 4 * ComputeTime_( Tempo.Unit, 4 * 60 * 1000 / Tempo.Value );
}

static void Print_( 
	const sPitch &Pitch,
	txf::text_oflow__ &Flow )
{
	Flow << GetPitchNameLabel( Pitch.Name );

	switch ( Pitch.Accidental ) {
	case paNatural:
		break;
	case paSharp:
		Flow << '#';
		break;
	case paFlat:
		Flow << 'b';
		break;
	default:
		qRGnr();
		break;
	}
}

static void Print_(
	const sDuration &Duration,
	txf::text_oflow__ &Flow )
{
	bso::sU8 Modifier = Duration.Modifier;

	Flow << (bso::sU8)Duration.Base;

	while ( Modifier-- )
		Flow << '.';

	if ( Duration.TiedToNext )
		Flow << '-';
}

static void Print_(
	const sNote &Note,
	txf::text_oflow__ &Flow,
	bso::bool__ Selected )
{
	if ( Selected )
		Flow << '<';

	Print_( Note.Pitch, Flow );

	Print_( Note.Duration, Flow );

	if ( Selected )
		Flow << '>';

	Flow << ' ' << txf::commit;

}



static void Play_(
	const sNote &Note,
	bso::sU32 Base,
	flw::oflow__ &Flow )
{
qRH;
	sEventHeader Header;
	data Data;
	event Event;
	tempo__ Tempo;
qRB;
	Header.Id = 144;
	Header.EventType = etMIDI;
	Header.DeltaTimeTicks = 0;
	Header.MIDIEvent.ChannelID = 0;
	Header.MIDIEvent.Tied = false;
	Header.MIDIEvent.Event = midNoteOn;

	Data.Init();

	Data.Append( GetAbsolute_( Note.Pitch ) );
	Data.Append( 120 );

	Event.Init( Header, Data );

	mscmdm::PutEvent( Event, mscmdm::xNone, Flow );
	Flow.Commit();

	Print_( Note, COut, false );

	tht::Suspend( ComputeTime_( Note.Duration, Base ) );

	Data.Store( 0, Data.Last() );
	Event.Init( Header, Data );

	mscmdm::PutEvent( Event, mscmdm::xNone, Flow );
	Flow.Commit();
qRR;
qRT;
qRE;
}


static void Play_(
	const dMelody &Melody,
	const tempo__ &Tempo,
	flw::oflow__ &Flow )
{
qRH;
	sRow Row = qNIL;
	bso::sU32 Base = 0;
qRB;
	Base = ComputeBase_( Tempo );

	Row = Melody.First();

	while ( Row != qNIL ) {
		Play_( Melody( Row ), Base, Flow );

		COut << txf::commit;

		Row = Melody.Next( Row );
	}

	COut << txf::nl;
qRR;
qRT;
qRE;
}

static void Print_(
	const dMelody &Melody,
	sRow  Current )
{
qRH;
	sRow Row = qNIL;
qRB;
	Row = Melody.First();

	while ( Row != qNIL ) {
		Print_( Melody( Row ), COut, Current == Row );

		Row = Melody.Next( Row );
	}

	COut << txf::commit << txf::nl;
qRR;
qRT;
qRE;
}

struct shared__ {
	mtx::rHandler Mutex;
	dMelody *Melody;
	sRow Row;
	flw::oflow__ *OFlow;
	shared__( void )
	{
		Mutex = MTX_INVALID_HANDLER;
		Melody = NULL;
		Row = qNIL;
		OFlow = NULL;
	}
};

static sdr::sRow GetDevice_(
	const char *Name,
	const mscmdd::names_ &Names )
{
	sdr::sRow Row = Names.First();

	while ( ( Row != qNIL ) && ( Names( Row ) != Name ) )
		Row = Names.Next( Row );

	return Row;

}

static int GetDeviceId_(
	mscmdd::way__ Way,
	const char *Name )
{
	int Id;
qRH;
	mscmdd::names Names;
	sdr::sRow Row = qNIL;
qRB;
	Names.Init();

	mscmdd::GetMidiDevicesNames( Way, Names );

	if ( ( Row = GetDevice_( Name, Names ) ) == qNIL ) {
		switch ( Way ) {
		case mscmdd::wIn:
			sclmisc::ReportAndAbort( "NoMidiInDeviceNamed", Name );
			break;
		case mscmdd::wOut:
			sclmisc::ReportAndAbort( "NoMidiOutDeviceNamed", Name );
			break;
		default:
			qRGnr();
			break;
		}
	}

	Id = *Row;
qRR;
qRT;
qRE;
	return Id;
}

static int GetDeviceInId_( void )
{
	int Id;
qRH;
	qCBUFFERr Buffer;
qRB;
	switch ( global::GetDeviceInPolicy() ) {
	case global::dpId:
		Id = global::GetDeviceInValue();
		break;
	case global::dpName:
		Id = GetDeviceId_( mscmdd::wIn, global::GetDeviceInValue( Buffer ) );
		break;
	default:
		qRGnr();
		break;
}
qRR;
qRT;
qRE;
	return Id;
}

static int GetDeviceOutId_( void )
{
	int Id;
qRH;
	STR_BUFFER___ Buffer;
qRB;
	switch ( global::GetDeviceOutPolicy() ) {
	case global::dpId:
		Id = global::GetDeviceOutValue();
		break;
	case global::dpName:
		Id = GetDeviceId_( mscmdd::wOut, global::GetDeviceOutValue( Buffer ) );
		break;
	default:
		qRGnr();
		break;
}
qRR;
qRT;
qRE;
	return Id;
}

static inline sSignatureKey GetSignatureKey_( void )
{
	return GetRawSignatureKey();
}

static inline signature_time__ GetSignatureTime_( void )
{
	return signature_time__( GetRawSignatureTimeNumerator(), GetRawSignatureTimeDenominator() );
}

static inline signature__ GetSignature_( void )
{
	return signature__( GetSignatureKey_(), GetSignatureTime_() );
}

static void HandleMIDI_( void *UP )
{
	shared__ &Shared = *(shared__ *)UP;
ERRFProlog
	mscmdd::midi_iflow___ IFlow;
	sEventHeader Header;
	data Data;
	event Event;
	sNote Note;
	signature__ Signature;
ERRFBegin
	Signature = GetSignature_();

	IFlow.Init( GetDeviceInId_() );

	IFlow.Start();

	while ( 1 ) {
		GetEventHeader( IFlow, Header );

		Data.Init();
		GetEventData( Header, IFlow, Data );

		Event.Init( Header, Data );

		PutEvent( Event, mscmdm::xNone, *Shared.OFlow );
		Shared.OFlow->Commit();

		if ( Header.EventType == etMIDI )
			if ( Header.MIDIEvent.Event == midNoteOn )
				if  ( Data( Data.Last() ) != 0 ) {
					Note = sNote( GetPitch_( Data( Data.First() ), Signature.Key ), sDuration( 3 ), Signature );
					mtx::Lock( Shared.Mutex );
					if ( Shared.Row != qNIL ) {
						Shared.Melody->Insert( Note, Shared.Row );
						Shared.Row = Shared.Melody->Next( Shared.Row );
					}else
						Shared.Melody->Append( Note );

					Print_( *Shared.Melody, Shared.Row );
					mtx::Unlock( Shared.Mutex );
				}
	}
ERRFErr
ERRFEnd
ERRFEpilog
}

static void Save_(
	const dMelody &Melody,
	const char *FileName )
{
qRH;
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
qRB;
	if ( Flow.Init( FileName, err::hUserDefined ) != fil::sSuccess ) {
		CErr << GetTranslation( mUnableToOpenFile, FileName ) << " !" << txf::nl << txf::commit;
		ERRReturn;
	}

	COut << GetTranslation( mWritingFile, FileName ) << "..." << txf::commit;

	TFlow.Init( Flow );

	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	mscmld::WriteXML( Melody, Writer );

	COut << GetTranslation( mDone ) << '.' << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}

static const char *Save_(
	const dMelody &Melody,
	STR_BUFFER___ &Buffer )
{
qRH;
	bso::bool__ Backuped = false;
qRB;
	if ( GetTargetFileName( Buffer ) == NULL )
		qRGnr();

	CreateBackupFile( Buffer );
	Backuped = true;

	Save_( Melody, Buffer() );
qRR;
	if ( Backuped )
		RecoverBackupFile( Buffer );
qRT;
qRE;
	return Buffer;
}

static void Save_( const dMelody &Melody )
{
qRH;
	STR_BUFFER___ Buffer;
qRB;
	Save_( Melody, Buffer );
qRR;
qRT;
qRE;
}

static void Execute_( const dMelody &Melody )
{
qRH;
	STR_BUFFER___ Buffer;
	const char *FileName = NULL;
	str::string Command;
qRB;
	FileName = Save_( Melody, Buffer );

	Command.Init();
	GetCommand( Command );

	str::ReplaceTag( Command, 1, str::string( FileName ), '%' );

	COut << GetTranslation( mExecutingCommand, Command.Convert( Buffer ) ) << "..." << txf::commit;

	system( Buffer );

	COut << GetTranslation( mDone ) << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}

#include "mtk.h"
#include <conio.h>

int get_code ( void )
{
  int ch = _getch();

  if ( ch == 0 || ch == 224 )
    ch = 256 + _getch();

  return ch;
}

enum
{
  KEY_BACK = 8,
  KEY_DELETE = 256 + 83,
  KEY_UP    = 256 + 72,
  KEY_DOWN  = 256 + 80,
  KEY_LEFT  = 256 + 75,
  KEY_RIGHT = 256 + 77
};

static void Go_( const parameters &Parameters )
{
qRH;
	melody Melody;
	shared__ Shared;
	int C;
	sNote Note;
	mscmdd::midi_oflow___ Flow;
	signature__ Signature;
	tempo__ Tempo;
ERRFBegin
	Signature = GetSignature_();
	GetTempo_( Tempo );

	Melody.Init();
	Flow.Init( GetDeviceOutId_() );

	Shared.Mutex = mtx::Create( mtx::mFree );

	Shared.Melody = &Melody;
	Shared.OFlow = &Flow;

	mtx::Lock( Shared.Mutex );

	mtk::Launch( HandleMIDI_, &Shared );

	while ( 1 ) {
		mtx::Unlock( Shared.Mutex );
		C = get_code();

		mtx::Lock( Shared.Mutex );
		if ( C == KEY_RIGHT ) {
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Next( Shared.Row );
		} else if ( C == KEY_LEFT ) {
			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();
			else
				Shared.Row = Melody.Previous( Shared.Row );
		} else if ( C == KEY_DELETE ) {

			if ( Shared.Row != qNIL ) {
				Melody.Remove( Shared.Row );

				if ( !Melody.Exists( Shared.Row ) )
					Shared.Row = Melody.Last();
			}
		} else if ( C == KEY_BACK ) {

			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();
			else
				Shared.Row = Melody.Previous( Shared.Row );

			if ( Shared.Row != qNIL ) {
				Melody.Remove( Shared.Row );

				if ( !Melody.Exists( Shared.Row ) )
					Shared.Row = qNIL;
			}
		} else if ( C == '+' ) {
			if ( Tempo.Value < 241 )
				Tempo.Value += 10;
		} else if ( C == '-' ) {
			if ( Tempo.Value > 39 )
				Tempo.Value -= 10;
		} else if ( C == 'p' ) 
			Play_( Melody, Tempo, *Shared.OFlow );
		else if ( C == 's' )
			Save_( Melody );
		else if ( C == 'x' )
			Execute_( Melody );
		else if ( C == 'r' ) {
			Note = sNote( sPitch( pnRest, 0 ),  sDuration( 3 ), Signature );
			if ( Shared.Row != qNIL ) {
				Shared.Melody->Insert( Note, Shared.Row );
				Shared.Row = Shared.Melody->Next( Shared.Row );
			}else
				Shared.Melody->Append( Note );
		} else if ( C < 256 ) {
			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();

			if ( Shared.Row != qNIL ) {
				Melody.Recall( Shared.Row, Note );

				if ( C == '0' )
					Note.Duration.Modifier = 0;
				else if ( isdigit( C ) )
					Note.Duration.Base = C - '0';
				else if ( C == '.' )
					Note.Duration.Modifier++;
				else if ( C == '-' )
					Note.Duration.TiedToNext = !Note.Duration.TiedToNext;

				Melody.Store( Note, Shared.Row );

//				Shared.Row = Melody.Next( Shared.Row );
			}
		}

		Print_( Melody, Shared.Row );
	}

qRR;
qRT;
	if ( Shared.Mutex != MTX_INVALID_HANDLER )
		mtx::Delete( Shared.Mutex );

	
qRE;
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int scltool::SCLTOOLMain(
	const str::dString &Command,
	const scltool::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH;
qRH;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Test );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
