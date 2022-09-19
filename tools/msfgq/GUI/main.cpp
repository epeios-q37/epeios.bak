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
    void Convert_(
      const melody::sNote &Note,
      txf::sWFlow &Flow)
    {
      const char *SName = mscmld::GetPitchNameLabel(Note);

      if ( strlen(SName) != 1 )
        qRGnr();

      int Name = SName[0];

      Flow << (char)toupper(Name) << ' ';
    }

    void Convert_(
      const melody::dMelody &Melody,
      txf::sWFlow &Flow)
    {
      Flow << "X: 1\\nT: Preview\\nK: G\\n";

      melody::sRow Row = Melody.First();

      if ( Row == qNIL )
        Flow << "[|]";

      while ( Row != qNIL ) {
        Convert_(Melody(Row), Flow);

        Row = Melody.Next(Row);
      }
    }
  }

  str::dString &GetABC(str::dString &ABC)
  {
  qRH;
    main::hGuard Guard;
    flx::rStringTWFlow Flow;
  qRB;
    const main::rXMelody &XMelody = main::Get(Guard);

    Flow.Init(ABC);

    Convert_(XMelody.Melody, Flow);
  qRR;
  qRT;
  qRE;
    return ABC;
  }

  namespace {
    void DisplayMelody_(main::sSession &Session)
    {
    qRH;
      str::wString ABC, Script;
    qRB;
      tol::Init(ABC, Script);

      GetABC(ABC);

      Script.Init("load(\"");
      Script.Append(ABC);
      Script.Append("\");");

      Session.Execute(Script);
    qRR;
    qRT;
    qRE;
    }
  }
}

D_( OnNewSession ) {
  str::wString Body;
  str::wString XHTML;

  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

  XHTML.Init();
  FillMidiInDevices_(XHTML);
  Session.Inner(str::wString("MidiIn"), XHTML);

  XHTML.Init();
  FillMidiOutDevices_(XHTML);
  Session.Inner(str::wString("MidiOut"), XHTML);

  DisplayMelody_(Session);
}

D_( Hit ) {
  DisplayMelody_(Session);
}

#define R_( name ) Core.Add(#name, actions_::name)

qGCTOR( main ) {
  Core.Init();
  R_( OnNewSession );
  R_( Hit );

  Melody_.Init();
  Mutex_ = mtx::Create();
}
