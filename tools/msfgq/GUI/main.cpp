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

#include "midiq.h"
#include "registry.h"

#include "mscmdd.h"

#include "mthrtn.h"

#include "cdgb64.h"
#include "rnd.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

mscmdd::rRFlow main::MidiRFlow;

namespace {
  namespace {
    bso::sBool Fill_(
      const str::dString  &Id,
      const str::dStrings &Ids,
      const str::dStrings &Names,
      txf::rWFlow &XHTML)
    {
      sdr::sRow Row = Ids.First();
      bso::sBool Available = false;

      if ( Ids.Amount() != Names.Amount() )
        qRGnr();

      while ( Row != qNIL ) {
        XHTML << "<option value=\"" << Ids(Row) << "\" ";

        if ( Ids(Row) == Id ) {
          Available = true;
          XHTML << " selected=\"true\"";
        }

        XHTML <<  " disabled=\"true\">" << Names(Row) << " (" << Ids(Row) << ")" << "</option>" << txf::nl;
        Row = Ids.Next(Row);
      }

      return Available;
    }
  }

  bso::sBool Fill_(
    const str::dString  &Id,
    const str::dStrings &Ids,
    const str::dStrings &Names,
    str::dString &XHTML)
  {
    bso::sBool Available = false;
  qRH;
    flx::rStringWDriver SDriver;
    txf::rWFlow TFlow;
  qRB;
    SDriver.Init(XHTML);
    TFlow.Init(SDriver);
    Available = Fill_(Id, Ids, Names, TFlow);
  qRR;
  qRT;
  qRE;
    return Available;
  }

  bso::sBool FillMidiInDevices_(
    const str::dString &Id,
    str::dString &XHTML)
  {
    bso::sBool Available = false;
  qRH;
    str::wStrings Ids, Names;
  qRB;
    tol::Init(Ids, Names);

    mscmdd::GetMidiInDeviceNames(Ids, Names);

    Available = Fill_(Id, Ids, Names, XHTML);
  qRR;
  qRT;
  qRE;
    return Available;
  }

  bso::sBool FillMidiOutDevices_(
    const str::dString &Id,
    str::dString &XHTML)
  {
    bso::sBool Available = false;
  qRH;
    str::wStrings Ids, Names;
  qRB;
    tol::Init(Ids, Names);

    mscmdd::GetMidiOutDeviceNames(Ids, Names);

    Available = Fill_(Id, Ids, Names, XHTML);
  qRR;
  qRT;
  qRE;
    return Available;
  }
}

#define D_( name )\
  namespace actions_ {\
    SCLX_ADec( sSession, name );\
  }\
  SCLX_ADef( sSession, actions_, name )

namespace {
  namespace _ {
    bso::sS8 Convert(
      const mscmld::sNote &Note,
      mscmld::sOctave BaseOctave,
      mscmld::eAccidental Accidental,
      const char *Separator,
      txf::sWFlow &Flow)
    {
      const char *Label = NULL;
      bso::sS8 RawDuration = 0;
      mthrtn::wRational Duration;
      bso::sS8 RelativeOctave;
      bso::pInteger Buffer;
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
          Flow << '^';
          break;
        case mscmld::aFlat:
          Flow << '_';
          break;
        case mscmld::aNatural:
          break;
        default:
          qRGnr();
          break;
        }

        Flow << PitchNotation;
      }

      Flow << bso::Convert(Duration.N.GetU32(), Buffer) << '/' << bso::Convert(Duration.D.GetU32(), Buffer);

      if ( Note.Duration.TiedToNext )
        Flow << '-';

      Flow << " [|]" << Separator;

      return 0;
    }

    namespace _ {
      qCDEFC(EscapedNL, "\\n");
      qCDEFC(NL, "\n");
    }

    bso::sS8 Convert(
      const mscmld::dMelody &Melody,
      mscmld::sOctave BaseOctave,
      mscmld::eAccidental Accidental,
      bso::sBool EscapeNL,
      bso::sU8 Width,
      txf::sWFlow &Flow)
    {
      bso::sS8 Return = 0;
      mscmld::sRow Row = Melody.First();
      bso::sUHuge Counter = 1;

      const char *&NL = EscapeNL ? _::EscapedNL : _::NL;

      Flow << "X: 1" << NL << "T:" << NL << "L: 1" << NL << "K: C" << NL;

      if ( Row == qNIL )
        Flow << "[|]";

      while ( ( Return == 0 ) && ( Row != qNIL ) ) {
        Return = Convert(Melody(Row), BaseOctave, Accidental, Counter++ % Width ? " " : NL, Flow);

        Row = Melody.Next(Row);
      }

      return Return;
    }
  }

  bso::sS8 GetABC_(
    const mscmld::dMelody &Melody,
    mscmld::sOctave BaseOctave,
    mscmld::eAccidental Accidental,
    bso::sBool EscapeNL,
    bso::sU8 Width,
    txf::sWFlow &Flow)
  {
    return _::Convert(Melody, BaseOctave, Accidental, EscapeNL, Width, Flow);
  }

  bso::sS8 GetABC_(
    const melody::rXMelody &XMelody,
    sWidth Width,
    bso::sBool EscapeNL,
    txf::sWFlow &Flow)
  {
    return GetABC_(XMelody, XMelody.BaseOctave, XMelody.Accidental, EscapeNL, Width, Flow);
  }

  bso::sS8 GetABC_(
    const melody::rXMelody &XMelody,
    sWidth Width,
    bso::sBool EscapeNL,
    str::dString &ABC)
  {
    bso::sS8 Return = 0;
  qRH;
    flx::rStringTWFlow Flow;
  qRB;
    Flow.Init(ABC);

    Return = GetABC_(XMelody, Width, EscapeNL, Flow);
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

    bso::sBool DisplayMelody_(
      const melody::rXMelody &XMelody,
      main::sSession &Session)
    {
      bso::sBool Success = false;
    qRH;
      str::wString ABC, Script;
      bso::sS8 OctaveOverflow = 0;
    qRB;
      tol::Init(ABC, Script);

      OctaveOverflow = GetABC_(XMelody, Session.Width, true, ABC);

      if ( OctaveOverflow != 0 )
        Session.AlertB(str::wString("Octave error !!!"));
      else {
        Script.Init("load(\"");
        Script.Append(ABC);
        Script.Append("\");");

        HighlightNote_(XMelody.Row, Script);

        Script.Append("updatePlayer();");

        Session.Execute(Script);

        Success = true;
      }
    qRR;
    qRT;
    qRE;
      return Success;
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
  void UpdateUIWidth_(
    const str::dString &Value,
    sSession &Session)
    {
    qRH;
      str::wStrings Ids, Values;
    qRB;
      tol::Init(Ids, Values);

      Ids.AppendMulti("WidthRangeInput", "WidthNumberInput");
      Values.AppendMulti(Value, Value);

      Session.SetValues(Ids, Values);
    qRR;
    qRT;
    qRE;
  }

  void UpdateUI_(
    const melody::rXMelody &XMelody,
    sSession &Session)
    {
    qRH;
      str::wString XHTML, Device;
      mscmld::eAccidental Accidental = mscmld::a_Undefined;
      bso::pInt IBuffer;
      qCBUFFERh CBuffer;
    qRB;
      Device.Init();
      midiq::GetDeviceInId(Device);

      XHTML.Init();

      if ( FillMidiInDevices_(Device, XHTML) )
        Session.RemoveAttribute(str::wString(Session.Parent("beautiful-piano", CBuffer)), str::wString("open"));
      else
        Session.SetValue("MidiIn", "None");

      Session.End(str::wString("MidiIn"), XHTML);

      /*
      XHTML.Init();
      FillMidiOutDevices_(XHTML);
      Session.Inner(str::wString("MidiOut"), XHTML);
      */

      Session.SetValue("AccidentalAmount", bso::Convert(abs(XMelody.Signature.Key), IBuffer));

      Accidental = XMelody.Signature.Key ? XMelody.Signature.Key > 0 ? mscmld::aSharp : mscmld::aFlat : XMelody.Accidental;
      Session.SetValue("Accidental", mscmld::GetLabel(Accidental));

      Session.SetValue("Numerator", bso::Convert(XMelody.Signature.Time.Numerator(), IBuffer));
      Session.SetValue("Denominator", bso::Convert(XMelody.Signature.Time.Denominator(), IBuffer));

      Session.SetValue("Octave", bso::Convert(XMelody.BaseOctave, IBuffer));

      UpdateUIWidth_(str::wString(bso::Convert(Session.Width, IBuffer)), Session);

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
  Session.Width = sclm::MGetU8(registry::parameter::Width, WidthMax);

  if ( Session.Width < WidthMin )
    sclr::ReportBadOrNoValueForEntryErrorAndAbort(registry::parameter::Width);

  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

  CXMEL();
  UpdateUI_(XMelody, Session);

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
    mscmld::sOctave BaseOctave,
    txf::sWFlow &Flow)
  {
  qRH;
    xml::rWriter Writer;
    xtf::sPos Pos;
  qRB;
    Writer.Init(Flow, xml::oIndent, xml::e_Default);
//  Writer.GetFlow() << "<?xml-stylesheet type='text/xsl' href='data:text/xsl;base64,PD94bWwgdmVyc2lvbj0iMS4wIj8+CjwhLS0gY29weS1vZi54c2wgLS0+Cjx4c2w6c3R5bGVzaGVldCB2ZXJzaW9uPSIxLjAiCiAgeG1sbnM6eHNsPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L1hTTC9UcmFuc2Zvcm0iPgoKICA8eHNsOnRlbXBsYXRlIG1hdGNoPSIvIj4KICAgIDx4c2w6Y29weS1vZiBzZWxlY3Q9IioiLz4KICA8L3hzbDp0ZW1wbGF0ZT4KPC94c2w6c3R5bGVzaGVldD4=' ?>\n";

    Writer.PushTag("Melody");
    Writer.PutAttribute("Amount", Melody.Amount() );
    Writer.PutAttribute("BaseOctave", BaseOctave);

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
    ToXML_(XMelody, XMelody.Accidental, XMelody.BaseOctave, Flow);

    Flow << txf::nl;  // Without this, with an empty melody the root 'Melody' will not be available for the script.
  }
}

namespace {
  qCDEFC(ABCBuiltInScriptId_, "_ABC");

  void ToBase64ABC_(
    const melody::rXMelody &XMelody,
    sWidth Width,
    txf::sWFlow &Flow)
    {
    qRH;
      cdgb64::rEncodingWFlow B64Flow;
      txf::sWFlow B64TWFlow;
    qRB;
      B64Flow.Init(Flow.Flow(), cdgb64::fURL);
      B64TWFlow.Init(B64Flow);

      GetABC_(XMelody, Width, false, B64TWFlow);
    qRR;
    qRT;
    qRE;
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
	bso::sBool EmbedScriptResult = false;
qRB;
  EmbedScriptResult = Session.GetBoolValue("Embedded");

  tol::Init(Mark, Script, Mime);
  Session.GetMark(Id, Mark);
	registry::GetScriptContentAndMime(Mark, Script, Mime);

	Script.Append(";echo -n '$';");

  Output.Init();
  OFlow.Init(Output);

  if ( !EmbedScriptResult )
    OFlow << "window.open().document.write('";

  OFlow << "<iframe src=\"data:" << Mime << ";base64,";

	if ( Mark == ABCBuiltInScriptId_ )
    ToBase64ABC_(melody::Get(Guard), Session.Width, OFlow);
  else {
    XDriver.Init(Script);
    WFlow.Init(XDriver);

    ToXML_(melody::Get(Guard), WFlow);

    WFlow.Commit();

    RFlow.Init(XDriver);

    while ( ( C = RFlow.Get() ) != '$' ) {
      if ( isgraph(C) )
        OFlow << (char)C;
    }
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

D_( SetTimeSignature )
{
qRH;
  qCBUFFERh Buffer;
  melody::hGuard Guard;
  mscmld::sSignatureTime Signature;
qRB;
  Signature.Init(str::wString(Session.GetValue("Numerator", Buffer)).ToU8(), str::wString(Session.GetValue("Denominator", Buffer)).ToU8());

  if ( !Signature.IsValid() )
    qRGnr();

  XMEL();

  XMelody.Signature.Time = Signature;
  mscmld::UpdateTimeSignature(Signature, XMelody);
qRR;
qRT;
qRE;
}

D_( SetOctave )
{
qRH;
  qCBUFFERh Buffer;
  melody::hGuard Guard;
  mscmld::sOctave BaseOctaveBackup = mscmld::UndefinedOctave;
qRB;
  XMEL();

  BaseOctaveBackup = XMelody.BaseOctave;

  str::wString(Session.GetValue(Id, Buffer)).ToNumber(XMelody.BaseOctave, str::sULimit<bso::sU8>(9));

  if ( !DisplayMelody_(XMelody, Session) ) {
    bso::pInt Buffer;
    XMelody.BaseOctave = BaseOctaveBackup;
    Session.SetValue("Octave", bso::Convert(XMelody.BaseOctave, Buffer));
  }
qRR;
qRT;
qRE;
}

D_( Test )
{
  Session.Log("Test");
}

D_( ChangeWidth )
{
qRH;
  str::wString Value;
  sWidth Width = 0;
  melody::hGuard Guard;
qRB;
  Value.Init();

  Session.GetValue(Id, Value);

  Value.ToNumber(Width, str::sULimit<sWidth>(WidthMax));

  if ( Width < WidthMin )
    qRGnr();

  UpdateUIWidth_(Value, Session);

  Session.Width = Width;

  CXMEL();
  DisplayMelody_(XMelody, Session);

qRR;
qRT;
qRE;
}

namespace {
  using namespace actions_;

  namespace _ {
    template <typename action> void Add(
      sclx::action_handler<sSession> &Core,
      action &Action )
    {
        Core.Add(Action.Name, Action);
    }

    template <typename action, typename... actions> void Add(
      sclx::action_handler<sSession> &Core,
      action &Action,
      actions &...Actions)
    {
        Add(Core, Action);
        Add(Core, Actions...);
    }
  }

  void Register_(void)
  {
    _::Add(Core,
      OnNewSession, Hit, SetAccidental, SetAccidentalAmount, Refresh,
      SelectNote, Rest, Duration, Dot, Tie,
      Execute, Cursor, Append, Suppr, Clear,
      Keyboard, Test, SetTimeSignature, SetOctave, ChangeWidth );
  }
}

#define R_( name ) Add_(main::Core, actions_::name)

qGCTOR( main ) {
  Core.Init();
  Register_();
}
