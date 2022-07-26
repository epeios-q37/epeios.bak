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


#include "mscfdraftq.h"

#include "keyboard.h"
#include "melody.h"
#include "midiq.h"
#include "registry.h"

#include "mscmdd.h"
#include "mscmld.h"

#include "sclm.h"
#include "sclt.h"

#include "err.h"
#include "cio.h"
#include "epsmsc.h"
#include "xpp.h"
#include "fnm.h"
#include "flf.h"
#include "mtk.h"

using namespace melody;

using cio::CErr;
using cio::COut;
using cio::CIn;

SCLI_DEF( mscfdraftq, NAME_LC, NAME_MC );

const scli::sInfo &sclt::SCLTInfo( void )
{
	return mscfdraftq::Info;
}

namespace {
	void PrintHeader_( void )
	{
		COut << NAME_MC " V" VERSION << " (" WEBSITE_URL ")" << txf::nl;
		COut << "Copyright (C) " COPYRIGHT << txf::nl;
		COut << txf::pad << "Build: " __DATE__ " " __TIME__ << " (" << cpe::GetDescription() << ')' << txf::nl;
	}
}

void DisplayMidiDevices(
  const str::dStrings &Ids,
  const str::dStrings &Names,
  const char *NoAvailableDevicesLabel,
  const char *AvailableDevicesLabel)
{
qRH;
	sdr::sRow Row = qNIL;
	str::wString Buffer;
qRB;
  Buffer.Init();

  if ( Names.Amount() != Ids.Amount() )
    qRGnr();

	if ( Names.Amount() == 0 )
		COut << sclm::GetBaseTranslation( NoAvailableDevicesLabel, Buffer ) << txf::nl;
	else {
		COut << sclm::GetBaseTranslation( AvailableDevicesLabel, Buffer );

    COut << ": " << txf::nl;

    Row = Names.First();

    while ( Row != qNIL ) {
      COut << Ids(Row) << " : " << Names( Row ) << txf::nl;

      Row = Names.Next( Row );
    }
	}

	COut << txf::commit;
qRR;
qRT;
qRE;
}

void DisplayMidiOutDevices( void )
{
qRH;
	str::wStrings Ids, Names;
qRB;
	tol::Init(Ids, Names);

	mscmdd::GetMidiOutDeviceNames(Ids, Names);

	DisplayMidiDevices(Ids, Names, "NoAvailableMIDIOutDevices", "AvailableMIDIOutDevices");
qRR;
qRT;
qRE;
}

void DisplayMidiInDevices( void )
{
qRH;
	str::wStrings Ids, Names;
qRB;
	tol::Init(Ids, Names);

	mscmdd::GetMidiOutDeviceNames(Ids, Names);

	DisplayMidiDevices(Ids, Names, "NoAvailableMIDIInDevices", "AvailableMIDIInDevices");
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

static void Draft_( void )
{
qRFH;
	wMelody Melody;
	midiq::sShared Shared;
	sNote Note;
	mscmdd::rWFlow Flow;
	sSignature Signature;
	sTempo Tempo;
	str::wString DeviceId;
	int KeyCode = 0;
	bso::sBool Continue = true;
qRFB;
  Signature = melody::GetSignature();
	melody::GetTempo( Tempo );

	Melody.Init();

	DeviceId.Init();
	Flow.Init(midiq::GetDeviceOutId(DeviceId));

	Shared.Mutex = mtx::Create();

	Shared.Melody = &Melody;
	Shared.OFlow = &Flow;

	mtx::Lock( Shared.Mutex );

	mtk::Launch(midiq::HandleInput, &Shared);

	while ( Continue ) {
		mtx::Unlock( Shared.Mutex );

		KeyCode = keyboard::GetCode();

		mtx::Lock( Shared.Mutex );

		switch ( KeyCode ) {
    case keyboard::cBack:
			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();
			else
				Shared.Row = Melody.Previous( Shared.Row );

			if ( Shared.Row != qNIL ) {
				Melody.Remove( Shared.Row );

				if ( !Melody.Exists( Shared.Row ) )
					Shared.Row = qNIL;
			}
      break;
    case keyboard::cUp:
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Previous(Shared.Row, 5);
      break;
    case keyboard::cDown:
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Next(Shared.Row, 5);
      break;
    case keyboard::cRight:
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Next(Shared.Row);
      break;
    case keyboard::cLeft:
			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();
			else
				Shared.Row = Melody.Previous( Shared.Row );
      break;
    case keyboard::cEnd:
			Shared.Row = Melody.Last();
      break;
    case keyboard::cBegin:
			Shared.Row = Melody.First();
      break;
    case keyboard::cDelete:
			if ( Shared.Row != qNIL ) {
				Melody.Remove( Shared.Row );

				if ( !Melody.Exists( Shared.Row ) )
					Shared.Row = Melody.Last();
			}
      break;
    case keyboard::cPageUp:
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Previous(Shared.Row, 10);
      break;
    case keyboard::cPageDown:
			if ( Shared.Row != qNIL )
				Shared.Row = Melody.Next(Shared.Row, 10);
      break;
    case '+':
      if ( Tempo.Value < 241 )
        Tempo.Value += 10;
      break;
    case '-':
			if ( Tempo.Value > 39 )
				Tempo.Value -= 10;
      break;
    case 'p':
			melody::Play(Melody, Tempo, *Shared.OFlow);
      break;
    case 's':
			Save_( Melody );
      break;
    case 'x':
			Execute_( Melody );
      break;
    case 'r':
			Note = sNote( sPitch( pnRest, 0 ),  sDuration( 3 ), Signature );
			if ( Shared.Row != qNIL ) {
				Shared.Melody->InsertAt( Note, Shared.Row );
				Shared.Row = Shared.Melody->Next( Shared.Row );
			} else
				Shared.Melody->Append( Note );
      break;
    case 'h':
      qRVct();
      break;
    case 'q':
      Continue = false;
      break;
    default:
			if ( Shared.Row == qNIL )
				Shared.Row = Melody.Last();

			if ( Shared.Row != qNIL ) {
				Melody.Recall( Shared.Row, Note );

				switch ( KeyCode ) {
        case '0':
					Note.Duration.Modifier = 0;
          break;
        case '.':
					Note.Duration.Modifier++;
          break;
        case '*':
					Note.Duration.TiedToNext = !Note.Duration.TiedToNext;
          break;
        default:
          if ( isdigit( KeyCode ) )
            Note.Duration.Base = KeyCode - '0';
          break;
				}

				Melody.Store( Note, Shared.Row );

				Shared.Row = Melody.Next( Shared.Row );
			}
      break;
		}

		melody::Print(Melody, Shared.Row, cio::COut);
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
	C(Info );
	C( Draft );
	else
		qRGnr();

	ExitValue = EXIT_SUCCESS;
qRR;
qRT;
qRE;
	return ExitValue;
}
