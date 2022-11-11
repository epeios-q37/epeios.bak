/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'TASq' software.

  'TASq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'TASq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'TASq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "registry.h"
#include "tasks.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

#define D_( name )\
  namespace actions_ {\
    SCLX_ADec( sSession, name );\
  }\
  SCLX_ADef( sSession, actions_, name )

namespace {
  void Build_(
    xml::rWriter &Writer,
    const tasks::dBundle &Bundle)
  {
  qRH;
    str::wString Value;
    tasks::sTask Task;
    tasks::sTRow
    Row = qNIL,
    Candidate = qNIL;
  qRB;
    Writer.PushTag("ul");
    Row = Bundle.First(Row);
    Writer.PushTag("li");

    while ( Row != qNIL ) {
      Writer.PushTag("details");

      Value.Init();
      Task.Init(Bundle.Queue);
      Bundle.Tasks.Recall(Row, Task);
      Writer.PutValue(Bundle.Strings(Task.Title), "summary");

      Writer.PopTag();  // 'details'.

      if ( ( Candidate = Bundle.First(Row) ) != qNIL ) {
        Writer.PopTag();
        Writer.PushTag("ul");
        Writer.PushTag("li");
        Row = Candidate;
      } else if( ( Candidate = Bundle.Next(Row) ) != qNIL ) {
        Writer.PopTag();  // 'li';
        Writer.PushTag("li");
        Row = Candidate;
      } else {
        Row = Bundle.Parent(Row);
        Writer.PopTag();  // 'li'.
        Writer.PopTag();  // 'uu'.
      }
    }

    Writer.PopTag();
  qRR;
  qRT;
  qRE;
  }
}


D_( OnNewSession )
{
qRH;
  str::wString Body;
  str::wString Tree;
  str::wString XML;
  flx::rStringTWFlow Flow;
  xml::rWriter Writer;
qRB;
  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

  Session.Execute("var markdown = editMarkdown('Edit','# Titre\\n```python\\ndef coucou():\\n  pass\\n```')");

  XML.Init();
  Flow.Init(XML);
  Writer.Init(Flow, xml::lIndent, xml::fEncoding());

  Build_(Writer, tasks::Bundle);
  Writer.reset();
  Flow.reset();
  Session.Inner(str::wString("Tree"), XML);
qRR;
qRT;
qRE;
}

D_( Submit )
{
  str::wString Markdown, Script;

  Markdown.Init();
  Session.Execute("markdown.value()", Markdown);

  Script.Init();
  flx::rStringTWFlow(Script) << "renderMarkdown('View','" << xdhcmn::Escape(Markdown, 0) << "');";
  Session.Execute(Script);

//  Session.Execute("renderMarkdown('View',markdown.value())");
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
    _::Add(Core, OnNewSession, Submit);
  }
}

#define R_( name ) Add_(main::Core, actions_::name)

qGCTOR( main ) {
  Core.Init();
  Register_();
}
