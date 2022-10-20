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

#include "mthrtn.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

mscmdd::rRFlow main::MidiRFlow;

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
      const mscmld::sNote &Note,
      bso::sU8 BaseOctave,
      mscmld::eAccidental Accidental,
      txf::sWFlow &Flow)
    {
      const char *Label = NULL;
      bso::sS8 RawDuration = 0;
      mthrtn::wRational Duration;
      bso::sS8 RelativeOctave;
      bso::pInteger Buffer;
      bso::sChar AccidentalMark = 0;
      char PitchNotation[] = {0,0,0};
      mscmld::sAltPitch Pitch;

      Pitch.Init();

      mscmld::Convert(Note, Accidental, Pitch);

      Label = mscmld::GetLabel(Pitch);
      RawDuration = Note.Duration.Base;

      Duration.Init(mthrtn::wRational(1,1 << (RawDuration - 1)) * mthrtn::wRational(( 2 << Note.Duration.Modifier ) - 1, 1 << Note.Duration.Modifier));
      RelativeOctave = Pitch.Octave - BaseOctave;


      if ( Pitch.Name == mscmld::pnRest ) {
        Flow << "z";
      } else {
        if ( RelativeOctave < 0 )
          return RelativeOctave;

        if ( RelativeOctave > 3 )
          return RelativeOctave - 3;

        if ( strlen(Label) != 1 )
          qRGnr();

        switch ( RelativeOctave ) {
        case 0:
          PitchNotation[1] = ',';
        case 1:
          PitchNotation[0] = toupper(Label[0]);
          break;
        case 3:
          PitchNotation[1] = '\'';
        case 2:
          PitchNotation[0] = tolower(Label[0]);
          break;
        default:
          qRGnr();
          break;
        }

        switch ( Pitch.Accidental ) {
        case mscmld::aSharp:
          AccidentalMark = '^';
          break;
        case mscmld::aFlat:
          AccidentalMark = '_';
          break;
        case mscmld::aNatural:
          AccidentalMark = ' ';
          break;
        default:
          qRGnr();
          break;
        }

        Flow << AccidentalMark << PitchNotation;
      }

      Flow << bso::Convert(Duration.N.GetU32(), Buffer) << '/' << bso::Convert(Duration.D.GetU32(), Buffer);

      if ( Note.Duration.TiedToNext )
        Flow << '-';

      Flow << "[|]" << ' ';

      return 0;
    }

    bso::sS8 Convert_(
      const mscmld::dMelody &Melody,
      bso::sU8 BaseOctave,
      mscmld::eAccidental Accidental,
      txf::sWFlow &Flow)
    {
      bso::sS8 Return = 0;
      mscmld::sRow Row = Melody.First();

      Flow << "X: 1\\nT:\\nL: 1\\nK: C\\n";

      Flow << "[|]";

      while ( ( Return == 0 ) && ( Row != qNIL ) ) {
        Return = Convert_(Melody(Row), BaseOctave, Accidental, Flow);

        Row = Melody.Next(Row);
      }

      return Return;
    }
  }

  bso::sS8 GetABC_(
    const mscmld::dMelody &Melody,
    bso::sU8 BaseOctave,
    mscmld::eAccidental Accidental,
    str::dString &ABC)
  {
    bso::sS8 Return = 0;
  qRH;
    flx::rStringTWFlow Flow;
  qRB;
    Flow.Init(ABC);

    Return = Convert_(Melody, BaseOctave, Accidental, Flow);
  qRR;
  qRT;
  qRE;
    return Return;
  }

  bso::sS8 GetABC_(
    const melody::rXMelody &XMelody,
    str::dString &ABC)
  {
    return GetABC_(XMelody, XMelody.BaseOctave, XMelody.Accidental, ABC);
  }

  bso::sS8 GetABC_(str::dString &ABC)
  {
    bso::sS8 Return = 0;
  qRH;
    melody::hGuard Guard;
  qRB;
    Return = GetABC_(melody::Get(Guard), ABC);
  qRR;
  qRT;
  qRE;
    return Return;
  }

  namespace {
    namespace {
      void HighlightNote_(
        mscmld::sRow Row,
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
      const melody::rXMelody &XMelody,
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

        Script.Append("updatePlayer();");

        Session.Execute(Script);
      }
    qRR;
    qRT;
    qRE;
    }
  }

  namespace _ {
    void GetScriptsXHTML(
      const str::dStrings &Ids,
      txf::sWFlow &XHTML)
      {
      qRH;
        str::wString Label, Description;
        sdr::sRow Row = qNIL;
      qRB;
        Row = Ids.First();

        while ( Row != qNIL ) {
          tol::Init(Label, Description);

          registry::GetScriptFeature(Ids(Row), Label, Description);

          XHTML << "<button xdh:mark=\"" << Ids(Row) << "\" xdh:onevent=\"Execute\">" << Label << "</button>" << txf::nl;

          Row = Ids.Next(Row);
        }
      qRR;
      qRT;
      qRE;
      }
  }

  void GetScriptsXHTML_(str::wString &XHTML)
  {
  qRH;
    str::wStrings Ids;
    flx::rStringTWFlow Flow;
  qRB;
    Ids.Init();
    Flow.Init(XHTML);

    registry::GetScriptIds(Ids);
    _::GetScriptsXHTML(Ids, Flow);
  qRR;
  qRT;
  qRE;
  }
}

namespace {
  void UpdateInterface_(
    const melody::rXMelody &XMelody,
    sSession &Session)
    {
    qRH;
      str::wString XHTML, Device;
      mscmld::eAccidental Accidental = mscmld::a_Undefined;
      bso::pInt Buffer;
    qRB;
      XHTML.Init();
      FillMidiInDevices_(XHTML);
      Session.Inner(str::wString("MidiIn"), XHTML);

      Device.Init();
      sclm::MGetValue(registry::parameter::devices::in::Value, Device);
      Session.SetValue("MidiIn", Device);

      /*
      XHTML.Init();
      FillMidiOutDevices_(XHTML);
      Session.Inner(str::wString("MidiOut"), XHTML);
      */

      Session.SetValue("AccidentalAmount", bso::Convert(abs(XMelody.Signature.Key), Buffer));

      Accidental = XMelody.Signature.Key ? XMelody.Signature.Key > 0 ? mscmld::aSharp : mscmld::aFlat : XMelody.Accidental;
      Session.SetValue("Accidental", mscmld::GetLabel(Accidental));

      Session.SetValue("Numerator", bso::Convert(XMelody.Signature.Time.Numerator(), Buffer));
      Session.SetValue("Denominator", bso::Convert(XMelody.Signature.Time.Denominator(), Buffer));

      Session.SetValue("Octave", bso::Convert(XMelody.BaseOctave, Buffer));

      XHTML.Init();
      GetScriptsXHTML_(XHTML);
      Session.End(str::wString("Scripts"), XHTML);
    qRR;
    qRT;
    qRE;
    }
}

#define XMEL() melody::rXMelody &XMelody = melody::Get(Guard)
#define CXMEL() const XMEL()

D_( OnNewSession )
{
qRH;
  melody::hGuard Guard;
  str::wString Body;
qRB;
  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

  CXMEL();
  UpdateInterface_(XMelody, Session);

  Session.Execute("createStylesheet();");

  DisplayMelody_(XMelody, Session);
  Session.Execute("activate()");
qRR;
qRT;
qRE;
}

D_( Hit )
{
qRH;
  str::wString Script;
  melody::hGuard Guard;
qRB;
  Script.Init("ABCJS.synth.playEvent([{\"cmd\":\"note\", \"pitch\":");
  Script.Append(Id);
  Script.Append(",\"durationInMeasures\":0.125,\"start\":0, \"volume\":70,\"instrument\":0,\"gap\":0}], [], 1000).then(function (response) {console.log(\"note played\");}).catch(function (error) {	console.log(\"error playing note\", error);});");
  Session.Execute(Script);

  CXMEL();
  DisplayMelody_(XMelody, Session);
qRR;
qRT;
qRE;
}

namespace {
  void HandleKeyAndAccidental_(
    melody::rXMelody &XMelody,
    sSession &Session)
  {
  qRH;
    str::wString RawKey, RawAccidental;
  qRB;
    tol::Init(RawAccidental, RawKey);
    Session.GetValue("AccidentalAmount", RawKey);
    Session.GetValue("Accidental", RawAccidental);

    melody::HandleKeyAndAccidental(RawKey.ToU8(), mscmld::GetAccidental(RawAccidental), XMelody);
  qRR;
  qRT;
  qRE;
  }
}

D_( SetAccidental )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  HandleKeyAndAccidental_(XMelody, Session);

  DisplayMelody_(XMelody, Session);
qRR;
qRT;
qRE;
}


D_( SetAccidentalAmount )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  HandleKeyAndAccidental_(XMelody, Session);
qRR;
qRT;
qRE;
}

D_( Refresh )
{
  Session.Log("Refresh");
  main::MidiRFlow.reset();
}

D_( SelectNote )
{
  melody::hGuard Guard;

  melody::rXMelody &XMelody = melody::Get(Guard);

  str::wString(Id).ToNumber(*XMelody.Row);
  DisplayMelody_(XMelody, Session);
}

D_( Rest )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  if ( XMelody.Row != qNIL ) {
    mscmld::sNote Note = XMelody(XMelody.Row);

    Note.Pitch = mscmld::pRest;
    Note.Duration.TiedToNext = false;

    XMelody.Store(Note, XMelody.Row);

    XMelody.Row = XMelody.Next(XMelody.Row);

    DisplayMelody_(XMelody, Session);
  }
qRR;
qRT;
qRE;
}

D_( Duration )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  if ( XMelody.Row != qNIL ) {
    mscmld::sNote Note = XMelody(XMelody.Row);

    str::wString(Id).ToNumber(Note.Duration.Base);

    XMelody.Store(Note, XMelody.Row);

    XMelody.Row = XMelody.Next(XMelody.Row);

    DisplayMelody_(XMelody, Session);
  }
qRR;
qRT;
qRE;
}

D_( Dot )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  if ( XMelody.Row != qNIL ) {
    mscmld::sNote Note = XMelody(XMelody.Row);

    if ( Note.Duration.Modifier >= 3 )
      Note.Duration.Modifier = 0;
    else
      Note.Duration.Modifier++;

    XMelody.Store(Note, XMelody.Row);

    DisplayMelody_(XMelody, Session);
  }
qRR;
qRT;
qRE;
}

D_( Tie )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  if ( XMelody.Row != qNIL ) {
    mscmld::sNote Note = XMelody(XMelody.Row);

    if ( Note.Pitch != mscmld::pRest ) {
      if ( Note.Duration.TiedToNext ) {
        Note.Duration.TiedToNext = false;
        XMelody.Store(Note, XMelody.Row);
      } else {
        Note.Duration.TiedToNext = true;
        XMelody.Store(Note, XMelody.Row);
        Note.Duration.TiedToNext = false;
        Note.Duration.Modifier = 0;
        Note.Duration.Base = 3;

        mscmld::sRow Row = XMelody.Next(XMelody.Row);

        if ( Row == qNIL )
          XMelody.Append(Note);
        else
          XMelody.InsertAt(Note, Row);
      }

      DisplayMelody_(XMelody, Session);
    }
  }
qRR;
qRT;
qRE;
}

namespace {
  void ToXML_(
    const mscmld::dMelody &Melody,
    mscmld::eAccidental Accidental,
    txf::sWFlow &Flow)
  {
  qRH;
    xml::rWriter Writer;
  qRB;
    Writer.Init(Flow, xml::oIndent, xml::e_Default);

    Writer.PushTag("Melody");
    Writer.PutAttribute("Amount", Melody.Amount() );

    mscmld::WriteXML(Melody, Accidental, Writer);

    Writer.PopTag();
  qRR;
  qRT;
  qRE;
  }

  void ToXML_(
    const melody::rXMelody &XMelody,
    txf::sWFlow &Flow)
  {
    ToXML_(XMelody, XMelody.Accidental, Flow);
  }
}

D_( Execute )
{
qRH;
	str::wString Mark, Script, Mime;
	flx::rExecDriver XDriver;
	txf::rWFlow WFlow;
	flw::rDressedRFlow<> RFlow;
	melody::hGuard Guard;
	flx::rStringTWFlow OFlow;
	str::wString Output;
	qCBUFFERh Buffer;
	int C;
	bso::sBool EmbedScriptResult = true;
qRB;
  tol::Init(Mark, Script, Mime);
  Session.GetMark(Id, Mark);
	registry::GetScriptContentAndMime(Mark, Script, Mime);

	Script.Append(";echo -n '$';");

	XDriver.Init(Script);
	WFlow.Init(XDriver);

	ToXML_(melody::Get(Guard), WFlow);

	WFlow.Commit();

	Output.Init();
  OFlow.Init(Output);

  if ( !EmbedScriptResult )
    OFlow << "window.open().document.write('";

  OFlow << "<iframe src=\"data:" << Mime << ";base64,";

	RFlow.Init(XDriver);

	while ( ( C = RFlow.Get() ) != '$' ) {
    if ( isgraph(C) )
      OFlow << (char)C;
	}

	OFlow << "\" frameborder=\"0\" style=\"border: 0; top: 0px; left: 0px; bottom: 0px; right: 0px; width: 100%; height: ";

	if ( EmbedScriptResult )
    OFlow << "400px";
  else
    OFlow << "100%";

  OFlow <<  ";\" allowfullscreen></iframe>";

	if ( !EmbedScriptResult)
    OFlow << "');";

	OFlow.reset();

	if ( EmbedScriptResult ) {
    Session.Inner(str::wString("Output"), Output);
    Session.Execute(str::wString("resizeOutputIFrame();"));
    Session.SetAttribute(Session.Parent("Output", Buffer), "open", "true");
    Session.ScrollTo("Output");
  } else
    Session.Execute(Output);
qRR;
qRT;
qRE;
}

D_( Cursor )
{
qRH;
  str::wString Cursor;
  melody::hGuard Guard;
qRB;
  Cursor.Init();

  Session.GetValue(Id, Cursor);

  XMEL();
  XMelody.Overwrite = Cursor == "Overwrite";
qRR;
qRT;
qRE;
}

D_( Append )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  XMelody.Row = qNIL;

  DisplayMelody_(XMelody, Session);
qRR;
qRT;
qRE;
}

D_( Suppr )
{
qRH;
  melody::hGuard Guard;
  bso::sBool IsLast = false;
qRB;
  XMEL();

  if ( XMelody.Row != qNIL ) {
    IsLast = XMelody.Row == XMelody.Last();

    XMelody.Remove(XMelody.Row);

    if ( IsLast )
      XMelody.Row = qNIL;

    DisplayMelody_(XMelody, Session);
  }
qRR;
qRT;
qRE;
}

D_( Clear )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  XMelody.wMelody::Init();
  XMelody.Row = qNIL;

  DisplayMelody_(XMelody, Session);
qRR;
qRT;
qRE;
}

D_( Keyboard )
{
qRH;
  melody::hGuard Guard;
qRB;
  XMEL();

  melody::Handle(mscmld::sNote(str::wString(Id+1).ToU8() + 5 + XMelody.BaseOctave * 12, mscmld::sDuration(3), XMelody.Signature), XMelody);

  DisplayMelody_(XMelody, Session);
qRR;
qRT;
qRE;
}

D_( Test )
{
  Session.Log("Test");
}

#define R_( name ) Core.Add(#name, actions_::name)

qGCTOR( main ) {
  Core.Init();
  R_( OnNewSession );
  R_( Hit );
  R_( SetAccidental );
  R_( SetAccidentalAmount );
  R_( Refresh );
  R_( SelectNote );
  R_( Rest );
  R_( Duration );
  R_( Dot );
  R_( Tie );
  R_( Execute );
  R_( Cursor );
  R_( Append );
  R_( Suppr );
  R_( Clear );
  R_( Keyboard );
  R_( Test );
}
