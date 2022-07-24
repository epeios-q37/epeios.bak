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

#include "sclt.h"
#include "scle.h"
#include "sclm.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "mtk.h"

#ifdef CPE_S_WIN
# include <conio.h>
#elif defined( CPE_S_POSIX )
# include <termios.h>
# include "stsfsm.h"
#else
# error "Unhandled operating system!"
#endif // CPE_S_WIN

using cio::CErr;
using cio::COut;
using cio::CIn;

using namespace mscmdm;
using namespace mscmld;

SCLI_DEF( meldigq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
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

#if 0
static void Display_( const dMelody &Melody )
{
qRH;
	xml::rWriter Writer;
qRB;
	Writer.Init( COut, xml::oIndent, xml::e_Default );

	WriteXML( Melody, Writer );

	COut << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}
#endif

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

struct sTempo {
	bso::sU8 Value;
	sDuration Unit;
	sTempo( void )
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

	Missing = sclm::OGetValue( registry::parameter::tempo::Unit, RawUnit );

	if ( !Missing ) {
        Base = RawUnit.ToU8( &Error, str::b10, 9 );

        if ( Error != qNIL ) {
            if ( RawUnit( Error ) != '.' )
                sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

            Row = Error;

            while ( (Row != qNIL) && (RawUnit( Row ) == '.') ) {
                Modifier++;
                Row = RawUnit.Next( Row );

                if ( Modifier >= 4 )
                    sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );

                if ( Row != qNIL )
                    sclr::ReportBadOrNoValueForEntryErrorAndAbort( registry::parameter::tempo::Unit );
            }
        }

        Unit = sDuration ( Base, Modifier );
	}
qRR;
qRT;
qRE;
	return Unit;
}

static const sTempo &GetTempo_( sTempo &Tempo )
{
	Tempo.Value = sclm::MGetU8( registry::parameter::tempo::Value );
	GetTempoUnit_( Tempo.Unit );

	return Tempo;
}

bso::sU32 ComputeBase_( const sTempo &Tempo )
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

	Flow << (char)((bso::sU8)Duration.Base + '0');

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
	bso::sBool Tied,
	flw::oflow__ &Flow )
{
qRH;
	sEventHeader Header;
	data Data;
	event Event;
	sTempo Tempo;
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

	if ( !Tied ) {
		mscmdm::PutEvent( Event, mscmdm::xNone, Flow );
		Flow.Commit();
	}

	Print_( Note, COut, false );

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


static void Play_(
	const dMelody &Melody,
	const sTempo &Tempo,
	flw::oflow__ &Flow )
{
qRH;
	sRow Row = qNIL;
	bso::sU32 Base = 0;
	bso::sBool Tied = false;
qRB;
	Base = ComputeBase_( Tempo );

	Row = Melody.First();

	while ( Row != qNIL ) {
		Play_( Melody( Row ), Base, Tied, Flow );

		Tied = false;

		COut << txf::commit;

		Tied = Melody( Row ).Duration.TiedToNext;

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
	mtx::rMutex Mutex;
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
	const mscmdd::dNames &Names )
{
	sdr::sRow Row = Names.First();

	while ( ( Row != qNIL ) && ( Names( Row ) != Name ) )
		Row = Names.Next( Row );

	return Row;

}

static int GetDeviceId_(
	mscmdd::eWay Way,
	const char *Name )
{
	int Id;
qRH;
	mscmdd::wNames Names;
	sdr::sRow Row = qNIL;
qRB;
	Names.Init();

	mscmdd::GetMidiDeviceNames( Way, Names );

	if ( ( Row = GetDevice_( Name, Names ) ) == qNIL ) {
		switch ( Way ) {
		case mscmdd::wIn:
			sclm::ReportAndAbort( "NoMidiInDeviceNamed", Name );
			break;
		case mscmdd::wOut:
			sclm::ReportAndAbort( "NoMidiOutDeviceNamed", Name );
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

qENUM( Policy ) {
	pId,
	_pName,
	pRaw,
	p_amount,
	p_Undefined
};

static ePolicy GetPolicy_( const rgstry::sTEntry &Entry )
{
	ePolicy Policy = p_Undefined;
qRH;
	str::wString RawPolicy;
qRB;
	RawPolicy.Init();

	sclm::MGetValue( Entry, RawPolicy );

	if ( RawPolicy == "Id" )
		Policy = pId;
	else if ( RawPolicy == "Name" )
		Policy = _pName;
	else if ( RawPolicy == "Raw" )
		Policy = pRaw;
	else
		sclr::ReportBadOrNoValueForEntryErrorAndAbort( Entry );
qRR;
qRT;
qRE;
	return Policy;
}

static const str::dString &GetDeviceId_(
	mscmdd::eWay Way,
	const rgstry::sTEntry &PolicyEntry,
	const rgstry::sTEntry &ValueEntry,
	str::dString &Id)
{
qRH;
	qCBUFFERh Buffer;
qRB;
	switch ( GetPolicy_( PolicyEntry ) ) {
	case pId:
//		Id = sclm::MGetU8( ValueEntry );
    qRVct();
		break;
	case _pName:
//		Id = GetDeviceId_( Way, sclm::MGetValue( ValueEntry, Buffer ) );
    qRVct();
		break;
  case pRaw:
    sclm::MGetValue(ValueEntry, Id);
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

static const str::dString &GetDeviceInId_(str::dString &Id)
{
	return GetDeviceId_(mscmdd::wIn, registry::parameter::devices::in::Policy, registry::parameter::devices::in::Value, Id);
}

static const str::dString &GetDeviceOutId_(str::dString &Id)
{
	return GetDeviceId_(mscmdd::wOut, registry::parameter::devices::out::Policy, registry::parameter::devices::out::Value, Id);
}

static inline sSignatureKey GetSignatureKey_( void )
{
	return sclm::MGetS8( registry::parameter::signature::Key, -7, 7 );
}

inline bso::sU8 GetRawSignatureTimeDenominator( void )
{
	return sclm::MGetU8( registry::parameter::signature::time::Denominator, 8 );
}

inline bso::sU8 GetRawSignatureTimeNumerator( void )
{
	return sclm::MGetU8( registry::parameter::signature::time::Numerator, 12 );
}

static inline sSignatureTime GetSignatureTime_( void )
{
	return sSignatureTime( GetRawSignatureTimeNumerator(), GetRawSignatureTimeDenominator() );
}

inline bso::sU8 GetRawSignatureKey( void )
{
	return sclm::MGetS8( registry::parameter::signature::Key, -7, 7 );
}

static inline sSignature GetSignature_( void )
{
	return sSignature( GetSignatureKey_(), GetSignatureTime_() );
}

static void HandleMIDI_( void *UP )
{
	shared__ &Shared = *(shared__ *)UP;
qRFH;
	mscmdd::rRFlow IFlow;
	sEventHeader Header;
	data Data;
	event Event;
	sNote Note;
	sSignature Signature;
	str::wString DeviceId;
qRFB;
	Signature = GetSignature_();


	DeviceId.Init();
	IFlow.Init(GetDeviceInId_(DeviceId));

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
						Shared.Melody->InsertAt( Note, Shared.Row );
						Shared.Row = Shared.Melody->Next( Shared.Row );
					}else
						Shared.Melody->Append( Note );

					Print_( *Shared.Melody, Shared.Row );
					mtx::Unlock( Shared.Mutex );
				}
	}
qRFR;
qRFT;
qRFE( sclm::ErrorDefaultHandling() );
}

static void Save_(
	const dMelody &Melody,
	const fnm::rName &FileName )
{
qRH;
	flf::file_oflow___ Flow;
	txf::text_oflow__ TFlow;
	xml::rWriter Writer;
	str::wString Buffer;
qRB;
	if ( Flow.Init( FileName, err::hUserDefined ) != tol::rSuccess )
		sclm::ReportFileOpeningErrorAndAbort( FileName );

	Buffer.Init();
	COut << sclm::GetBaseTranslation( "WritingFile", Buffer, FileName ) << "..." << txf::commit;

	TFlow.Init( Flow );

	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	Writer.PushTag( "Melody" );

	mscmld::WriteXML( Melody, Writer );

	Writer.PopTag();

	Buffer.Init();
	COut << sclm::GetBaseTranslation( "Done", Buffer ) << '.' << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}

static const str::dString &Save_(
	const dMelody &Melody,
	str::dString &FileName )
{
qRH;
	bso::bool__ Backuped = false;
qRB;
	sclm::MGetValue( registry::parameter::TargetFileName, FileName );

	sclm::CreateBackupFile( FileName );

	Backuped = true;

	Save_( Melody, (const str::dString &)FileName );
qRR;
	if ( Backuped )
		sclm::RecoverBackupFile( FileName );
qRT;
qRE;
	return FileName;
}

static void Save_( const dMelody &Melody )
{
qRH;
	str::wString FileName;
qRB;
	FileName.Init();
	Save_( Melody, FileName );
qRR;
qRT;
qRE;
}

static void Execute_( const dMelody &Melody )
{
qRH;
	str::wString FileName;
	str::string Script;
	str::wString Buffer;
qRB;
	FileName.Init();
	Save_( Melody, FileName );

	Script.Init();
	sclm::MGetValue( registry::parameter::Script, Script );

	tagsbs::SubstituteShortTag( Script, 1, FileName, '%' );

	Buffer.Init();
	COut << sclm::GetBaseTranslation( "ExecutingCommand", FileName ) << "..." << txf::commit;

	tol::System( Script );

	Buffer.Init();
	COut << sclm::GetBaseTranslation( "Done", Buffer ) << '.' << txf::nl << txf::commit;
qRR;
qRT;
qRE;
}

namespace {
#ifdef CPE_S_WINDOWS
    int getch_( void ) {
        return _getch();
    }
#elif defined(CPE_S_POSIX)
    int getch_(void)
    {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
    }
#endif
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

namespace key_ {
  namespace {
    stsfsm::wAutomat Automat_;
    stsfsm::sParser Parser_;

    namespace {
      const char KeyUp_[] = {91, 65, 0};
      const char KeyRight_[] = {91, 67, 0};
      const char KeyLeft_[] = {91, 68, 0};
      const char KeyDelete_[] = {91, 51, 126, 0};
    }

    struct {
      int Id;
      const char *Tag;
    } Keys_ [] = {
        {KEY_DELETE, KeyDelete_},
        {KEY_UP, KeyUp_},
        {KEY_RIGHT, KeyRight_},
        {KEY_LEFT, KeyLeft_}
    };
  }

  void SetAutomat(void) {
    Automat_.Init();

    for ( int i = 0; i < sizeof(Keys_) / sizeof(Keys_[0] ); i++)
      stsfsm::Add(Keys_[i].Tag, Keys_[i].Id, Automat_);

    Parser_.Init(Automat_);
  }

  stsfsm::eStatus GetStatus(int C) {
    return Parser_.Handle(C);
  }

  int GetId(void) {
    return Parser_.GetId();
  }

  void Reset(void) {
    Parser_.Reset();
  }
}

int get_code ( void )
{
  int ch = getch_();
#ifdef CPE_S_WINDOWS
  if ( ch == 0 || ch == 224 )
    ch = 256 + getch_();
#elif defined(CPE_S_POSIX)
  if ( ch == 127 )
    ch = KEY_BACK;
  else if ( ch == 27 ) {
    stsfsm::eStatus Status = stsfsm::s_Undefined;

    while ( ( Status = key_::GetStatus(getch_() ) ) == stsfsm::sPending );

    if ( Status == stsfsm::sMatch )
      ch = key_::GetId();
    else
      ch = 0;

    key_::Reset();
  }
#endif // defined

  return ch;
}

static void Launch_( void )
{
qRFH;
	wMelody Melody;
	shared__ Shared;
	int C;
	sNote Note;
	mscmdd::rWFlow Flow;
	sSignature Signature;
	sTempo Tempo;
	str::wString DeviceId;
qRFB;
  key_::SetAutomat();

	Signature = GetSignature_();
	GetTempo_( Tempo );

	Melody.Init();

	DeviceId.Init();
	Flow.Init(GetDeviceOutId_(DeviceId));

	Shared.Mutex = mtx::Create();

	Shared.Melody = &Melody;
	Shared.OFlow = &Flow;

	mtx::Lock( Shared.Mutex );

	mtk::RawLaunch(HandleMIDI_, &Shared, true);

	while ( 1 ) {
		mtx::Unlock( Shared.Mutex );
		C = get_code();

		mtx::Lock( Shared.Mutex );
		if ( C == KEY_RIGHT ) {
			if ( Shared.Row != qNIL ) {
				Shared.Row = Melody.Next( Shared.Row );
			}
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
				Shared.Melody->InsertAt( Note, Shared.Row );
				Shared.Row = Shared.Melody->Next( Shared.Row );
			} else
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
				else if ( C == '*' )
					Note.Duration.TiedToNext = !Note.Duration.TiedToNext;

				Melody.Store( Note, Shared.Row );

				Shared.Row = Melody.Next( Shared.Row );
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

void DisplayMidiOutDevices( void )
{
qRH;
	mscmdd::wNames Names;
	sdr::sRow Row = qNIL;
	str::wString Buffer;
qRB;
	Names.Init();

	mscmdd::GetMidiOutDeviceNames( Names );

	Buffer.Init();

	if ( Names.Amount() == 0 )
		cio:: COut << sclm::GetBaseTranslation( "NoMIDIOutDevicesAvailable", Buffer );
	else
		cio::COut << sclm::GetBaseTranslation( "AvailableMIDIOutDevices", Buffer );

	cio::COut << " : " << txf::nl;

	Row = Names.First();

	while ( Row != qNIL ) {
		cio::COut << *Row << " : " << Names( Row ) << txf::nl;

		Row = Names.Next( Row );
	}

	cio::COut << txf::commit;
qRR;
qRT;
qRE;
}

void DisplayMidiInDevices( void )
{
qRH;
	mscmdd::wNames Names;
	sdr::sRow Row = qNIL;
	str::wString Buffer;
qRB;
	Names.Init();

	mscmdd::GetMidiInDeviceNames( Names );

	Buffer.Init();

	if ( Names.Amount() == 0 )
		cio::COut << sclm::GetBaseTranslation( "NoMIDIInDevicesAvailable", Buffer );
	else
		cio::COut << sclm::GetBaseTranslation( "AvailableMIDIInDevices", Buffer );

	cio::COut << " : " << txf::nl;

	Row = Names.First();

	while ( Row != qNIL ) {
		cio::COut << *Row << " : " << Names( Row ) << txf::nl;

		Row = Names.Next( Row );
	}

	cio::COut << txf::commit;
qRR;
qRT;
qRE;
}

void DisplayMidiDevices( void )
{
	DisplayMidiInDevices();
	DisplayMidiOutDevices();
}

void Info_( void )
{
	DisplayMidiDevices();
}

#define C( name )\
	else if ( Command == #name )\
		name##_()

int sclt::SCLTMain(
	const str::dString &Command,
	const sclt::fOddities &Oddities )
{
	int ExitValue = EXIT_FAILURE;
qRH;
qRB;
	if ( Command == "Version" )
		PrintHeader_();
	else if ( Command == "License" )
		epsmsc::PrintLicense( NAME_MC );
	C( Launch );
	C( Info );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
