/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'MSFGq' software.

  'MSFGq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'MSFGq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'MSFGq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "registry.h"

#include "mscmdd.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

namespace {
  rXMelody Melody_;
  mtx::rMutex Mutex_ = mtx::Undefined;
}

rXMelody &main::Get(hGuard &Guard)
{
  Guard.InitAndLock(Mutex_);

  return Melody_;
}

namespace {
  namespace {
    void Fill_(
      const str::dStrings &Ids,
      const str::dStrings &Names,
      txf::rWFlow &XHTML)
    {
      sdr::sRow Row = Ids.First();

      if ( Ids.Amount() != Names.Amount() )
        qRGnr();

      while ( Row != qNIL ) {
        XHTML << "<option value=\"" << Ids(Row) <<  "\">" << Names(Row) << "</option>" << txf::nl;
        Row = Ids.Next(Row);
      }
    }
  }

  void Fill_(
    const str::dStrings &Ids,
    const str::dStrings &Names,
    str::dString &XHTML)
  {
  qRH;
    flx::rStringWDriver SDriver;
    txf::rWFlow TFlow;
  qRB;
    SDriver.Init(XHTML);
    TFlow.Init(SDriver);
    Fill_(Ids, Names, TFlow);
  qRR;
  qRT;
  qRE;
  }

  void FillMidiInDevices_(str::dString &XHTML)
  {
  qRH;
    str::wStrings Ids, Names;
  qRB;
    tol::Init(Ids, Names);

    mscmdd::GetMidiInDeviceNames(Ids, Names);

    Fill_(Ids, Names, XHTML);
  qRR;
  qRT;
  qRE;
  }

  void FillMidiOutDevices_(str::dString &XHTML)
  {
  qRH;
    str::wStrings Ids, Names;
  qRB;
    tol::Init(Ids, Names);

    mscmdd::GetMidiOutDeviceNames(Ids, Names);

    Fill_(Ids, Names, XHTML);
  qRR;
  qRT;
  qRE;
  }
}

#define D_( name )\
  namespace actions_ {\
    SCLX_ADec( sSession, name );\
  }\
  SCLX_ADef( sSession, actions_, name )

namespace {
  namespace {
    bso::sS8 Convert_(
      const melody::sNote &Note,
      bso::sU8 BaseOctave,
      txf::sWFlow &Flow)
    {
      const char *Label = mscmld::GetPitchNameLabel(Note);
      const bso::sS8 RawDuration = Note.Duration.Base;
      bso::sU8 Duration = RawDuration > 0 ? 1 << (RawDuration - 1) : 2 >> -RawDuration;
      bso::sS8 RelativeOctave = Note.Pitch.Octave - BaseOctave;
      bso::pInteger Buffer;
      bso::sChar Accidental = 0;
      char Pitch[] = {0,0,0};

      if ( RelativeOctave < 0 )
        return RelativeOctave;

      if ( RelativeOctave > 3 )
        return RelativeOctave - 3;

      if ( strlen(Label) != 1 )
        qRGnr();

      switch ( RelativeOctave ) {
      case 0:
        Pitch[1] = ',';
      case 1:
        Pitch[0] = toupper(Label[0]);
        break;
      case 3:
        Pitch[1] = '\'';
      case 2:
        Pitch[0] = tolower(Label[0]);
        break;
      default:
        qRGnr();
        break;
      }

      switch ( Note.Pitch.Accidental ) {
      case mscmld::paSharp:
        Accidental = '^';
        break;
      case mscmld::paFlat:
        Accidental = '_';
        break;
      case mscmld::paNatural:
        Accidental = ' ';
        break;
      default:
        qRGnr();
        break;
      }

      Flow << Accidental << Pitch << '/' << bso::Convert(Duration, Buffer) << ' ';

      return 0;
    }

    bso::sS8 Convert_(
      const melody::dMelody &Melody,
      bso::sU8 BaseOctave,
      txf::sWFlow &Flow)
    {
      bso::sS8 Return = 0;
      melody::sRow Row = Melody.First();

      Flow << "X: 1\\nT: Preview\\nL: 1\\nK: \\n";

      if ( Row == qNIL )
        Flow << "[|]";

      while ( ( Return == 0 ) && ( Row != qNIL ) ) {
        Return = Convert_(Melody(Row), BaseOctave, Flow);

        Row = Melody.Next(Row);
      }

      return Return;
    }
  }

  bso::sS8 GetABC_(
    const melody::dMelody &Melody,
    bso::sU8 BaseOctave,
    str::dString &ABC)
  {
    bso::sS8 Return = 0;
  qRH;
    flx::rStringTWFlow Flow;
  qRB;
    Flow.Init(ABC);

    Return = Convert_(Melody, BaseOctave, Flow);
  qRR;
  qRT;
  qRE;
    return Return;
  }

  bso::sS8 GetABC_(
    const main::rXMelody &XMelody,
    str::dString &ABC)
  {
    return GetABC_(XMelody.Melody, XMelody.BaseOctave, ABC);
  }

  bso::sS8 GetABC_(str::dString &ABC)
  {
    bso::sS8 Return = 0;
  qRH;
    main::hGuard Guard;
  qRB;
    Return = GetABC_(main::Get(Guard), ABC);
  qRR;
  qRT;
  qRE;
    return Return;
  }

  namespace {
    namespace {
      void HighlightNote_(
        melody::sRow Row,
        str::dString &Script)
      {
        bso::pInt Buffer;

        Script.Append("highlightNote(\"");
        if ( Row != qNIL)
          Script.Append(bso::Convert(*Row, Buffer));
        Script.Append("\");");
      }
    }

    void DisplayMelody_(
      const main::rXMelody &XMelody,
      main::sSession &Session)
    {
    qRH;
      str::wString ABC, Script;
      bso::sS8 OctaveOverflow = 0;
    qRB;
      tol::Init(ABC, Script);

      OctaveOverflow = GetABC_(XMelody, ABC);

      if ( OctaveOverflow != 0 )
        Session.AlertB(str::wString("Octave error !!!"));
      else {
        Script.Init("load(\"");
        Script.Append(ABC);
        Script.Append("\");");

        HighlightNote_(XMelody.Row, Script);

        Session.Execute(Script);
      }
    qRR;
    qRT;
    qRE;
    }

    void DisplayMelody_(main::sSession &Session)
    {
    qRH;
      main::hGuard Guard;
    qRB;
      DisplayMelody_(Get(Guard), Session);
    qRR;
    qRT;
    qRE;
    }
  }
}

D_( OnNewSession ) {
  str::wString Body, XHTML, Key;
  bso::pInt Buffer;

  Session.Execute("createStylesheet();");
//  Session.Execute("activate();");

  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

  XHTML.Init();
  FillMidiInDevices_(XHTML);
  Session.Inner(str::wString("MidiIn"), XHTML);

  /*
  XHTML.Init();
  FillMidiOutDevices_(XHTML);
  Session.Inner(str::wString("MidiOut"), XHTML);
*/
  DisplayMelody_(Session);
  Session.Execute("document.querySelector(\".activate-audio\").addEventListener(\"click\", activate);");

  Key.Init();
  if ( sclm::OGetValue(registry::parameter::signature::Key, Key) )
    Session.SetValue("Key", Key);

  melody::sSignatureTime SignatureTime = melody::GetSignatureTime();

  Session.SetValue("Numerator", bso::Convert(SignatureTime.Numerator(), Buffer));
  Session.SetValue("Denominator", bso::Convert(SignatureTime.Denominator(), Buffer));

//  Session.Execute("setMyKeyDownListener();");
}

D_( Hit ) {
  DisplayMelody_(Session);
}

D_( Key )
{

}

D_( Refresh )
{
  Session.Log("Refresh");
}

D_( SelectNote )
{
  main::hGuard Guard;

  main::rXMelody &XMelody = main::Get(Guard);

  str::wString(Id).ToNumber(*XMelody.Row);
  DisplayMelody_(XMelody, Session);
}

D_( Duration )
{
qRH;
  main::hGuard Guard;
qRB;
  main::rXMelody &XMelody = main::Get(Guard);

  if ( XMelody.Row != qNIL ) {
    melody::sNote Note = XMelody.Melody(XMelody.Row);

    str::wString(Id).ToNumber(Note.Duration.Base);

    XMelody.Melody.Store(Note, XMelody.Row);

    XMelody.Row = XMelody.Melody.Next(XMelody.Row);

    DisplayMelody_(XMelody, Session);
  }
qRR;
qRT;
qRE;
}

namespace {
  static void ToXML_(
    const melody::dMelody &Melody,
    txf::sWFlow &Flow)
  {
  qRH;
    xml::rWriter Writer;
  qRB;
    Writer.Init(Flow, xml::oIndent, xml::e_Default);

    Writer.PushTag("Melody");

    mscmld::WriteXML(Melody, Writer);

    Writer.PopTag();
  qRR;
  qRT;
  qRE;
  }
}

D_( Execute )
{
qRH;
	str::wString Script;
	flx::rExecWDriver XDriver;
	txf::rWFlow TFlow;
	main::hGuard Guard;
qRB;
	Script.Init();
	sclm::MGetValue(registry::definition::Script, Script);

	XDriver.Init(Script);
	TFlow.Init(XDriver);

	ToXML_(main::Get(Guard).Melody, TFlow);

	TFlow.Commit();
qRR;
qRT;
qRE;
}

#define R_( name ) Core.Add(#name, actions_::name)

qGCTOR( main ) {
  Core.Init();
  R_( OnNewSession );
  R_( Hit );
  R_( Key );
  R_( Refresh );
  R_( SelectNote );
  R_( Duration );
  R_( Execute );

  Melody_.Init();
  Mutex_ = mtx::Create();
}
