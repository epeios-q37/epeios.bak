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

#include "tasqtasks.h"
#include "tasqxml.h"

using namespace main;

sclx::action_handler<sSession> main::Core;

#define D_( name )\
  namespace actions_ {\
    SCLX_ADec( sSession, name );\
  }\
  SCLX_ADef( sSession, actions_, name )

namespace tasks = tasqtasks;

namespace {
  void Select_(
    tasks::sTRow Row,
    const tasks::dBundle &Bundle,
    sSession &Session)
  {
  qRH;
    bso::pInteger Buffer;
    tasks::sTRow Looper = qNIL;
    qCBUFFERh Parent;
  qRB;
    Looper = Row;
    while ( Looper != qNIL ) {
      Session.SetAttribute(Session.Parent(bso::Convert(*Looper, Buffer), Parent), "open", "true");

      Looper = Bundle.Parent(Looper);
    }

    Session.ScrollTo(bso::Convert(*Row, Buffer));

    Session.AddClass(Buffer(), "selected");
  qRR;
  qRT;
  qRE;
  }
}

#define BGRD  tasks::hGuard BundleGuard
#define BNDL()  tasks::rXBundle &Bundle = tasks::Get(BundleGuard)
#define CBNDL()  const tasks::rXBundle &Bundle = tasks::CGet(BundleGuard)


D_( OnNewSession )
{
qRH;
  BGRD;
  str::wString Body;
  str::wString Tree;
  str::wString XML;
  str::wString XSL, Base64XSL, XSLAsURI;
  flx::rStringTWFlow Flow;
  xml::rWriter Writer;
qRB;
  Body.Init();
  sclm::MGetValue(registry::definition::Body, Body);

  Session.Inner(str::Empty, Body);

//  Session.Execute("var markdown = editMarkdown('Edit','# Titre\\n```python\\ndef coucou():\\n  pass\\n```')");

  XML.Init();
  Flow.Init(XML);
  Writer.Init(Flow, xml::lIndent, xml::fEncoding());

  CBNDL();

  tasqxml::Write(Bundle, Writer);
  Writer.reset();
  Flow.reset();

  XSL.Init();
  sclm::MGetValue(registry::definition::XSLFiles::Items, XSL);

  Base64XSL.Init();
  cdgb64::Encode(XSL, cdgb64::fOriginal, Base64XSL);

  XSLAsURI.Init("data:text/xml;base64,");
  XSLAsURI.Append(Base64XSL);

  XSL.StripLeadingChars('\n');
  Session.Inner(str::wString("Tree"), XML, XSLAsURI);
qRR;
qRT;
qRE;
}

D_( Select )
{
qRH;
  BGRD;
  tasks::sTRow Row = qNIL;
  str::wString Title, Description, Script;
  bso::pInteger Buffer;
qRB;
  str::wString(Id).ToNumber(*Row);

  BNDL();

  tol::Init(Title, Description);
  Bundle.Get(Row, Title, Description);

  Session.SetValue("Title", Title);

  Script.Init();
  flx::rStringTWFlow(Script) << "renderMarkdown('DescriptionView','" << xdhcmn::Escape(Description, 0) << "');";
  Session.Execute(Script);

  if ( Bundle.Selected != qNIL )
    Session.RemoveClass(bso::Convert(*Bundle.Selected, Buffer), "selected");

  Session.AddClass(bso::Convert(*Row, Buffer), "selected");

  Bundle.Selected = Row;
qRR;
qRT;
qRE;
}

D_( Edit )
{
qRH;
  BGRD;
  str::wString Title, Description, Script;
qRB;
  Session.AddClass("DescriptionView", "hide");
  Session.AddClass("Tree", "hide");

  CBNDL();

  tol::Init(Title, Description);
  Bundle.Get(Bundle.Selected, Title, Description);

  Script.Init();
  flx::rStringTWFlow(Script) << "markdown = editMarkdown('DescriptionEdition','" << xdhcmn::Escape(Description, 0) << "');";
  Session.Execute(Script);

  Session.RemoveClass("DescriptionEdition", "hide");
qRR;
qRT;
qRE;
}

D_( Submit )
{
qRH;
  BGRD;
  str::wString Title, Description, Script;
qRB;
  BNDL();

  tol::Init(Title, Description);
  Bundle.Get(Bundle.Selected, Title, Description);

  Description.Init();
  Session.Execute("let value = markdown.value(); markdown.toTextArea(); markdown = null; value;", Description);

  Bundle.Set(Title, Description, Bundle.Selected);

  Session.SetValue("Title", Title);

  Script.Init();
  flx::rStringTWFlow(Script) << "renderMarkdown('DescriptionView','" << xdhcmn::Escape(Description, 0) << "');";
  Session.Execute(Script);

  Session.AddClass("DescriptionEdition", "hide");
  Session.RemoveClass("DescriptionView", "hide");
  Session.RemoveClass("Tree", "hide");
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
    _::Add(Core, OnNewSession, Select, Edit, Submit);
  }
}

#define R_( name ) Add_(main::Core, actions_::name)

qGCTOR( main ) {
  Core.Init();
  Register_();
}
