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

namespace {
  qENUM( Mode_ ) {
    mView,
    mEdition,
    m_amount,
    m_Undefined
  };

  void SetDisplay_(
    eMode_ Mode,
    sSession &Session)
  {
  qRH;
    str::wStrings
      ViewIds, ViewClasses,
      EditionIds, EditionClasses;
  qRB;
    tol::Init(ViewIds, ViewClasses);

    ViewIds.AppendMulti("Tree", "TitleView", "DescriptionView", "Edit", "New");
    ViewClasses.AppendMulti("hide", "hide", "hide" "hide", "hide");

    tol::Init(EditionIds, EditionClasses);

    EditionIds.AppendMulti("TitleEdition", "DescriptionEdition", "Submit", "Cancel");
    EditionClasses.AppendMulti("hide", "hide", "hide", "hide");

    switch( Mode ) {
    case mView:
      Session.AddClasses(EditionIds, EditionClasses);
      Session.RemoveClasses(ViewIds, ViewClasses);
      break;
    case mEdition:
      Session.AddClasses(ViewIds, ViewClasses);
      Session.RemoveClasses(EditionIds, EditionClasses);
      break;
    default:
      qRGnr();
      break;
    }
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

  XML.Init();
  Flow.Init(XML);
  Writer.Init(Flow, xml::lIndent);

  CBNDL();

  tasqxml::Write(Bundle, tasqxml::ffDisplay, Writer);
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

  SetDisplay_(mView, Session);
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

  Session.SetValue("TitleView", Title);

  Script.Init();
  flx::rStringTWFlow(Script) << "renderMarkdown('DescriptionView','" << xdhcmn::Escape(Description, 0) << "');";
  Session.Execute(Script);

  if ( Session.Selected != qNIL )
    Session.RemoveClass(bso::Convert(*Session.Selected, Buffer), "selected");

  Session.AddClass(bso::Convert(*Row, Buffer), "selected");

  Session.Selected = Row;
qRR;
qRT;
qRE;
}

namespace {
  void Edit_(
    const str::dString &Title,
    const str::dString &Description,
    sSession &Session)
  {
  qRH;
    str::wString Script, EscapedDescription;
  qRB;
    Session.SetValue("TitleEdition", Title);

    tol::Init(Script, EscapedDescription);
    flx::rStringTWFlow(Script) << "markdown = editMarkdown('DescriptionEdition','" << xdhcmn::Escape(Description, EscapedDescription, 0) << "');";
    Session.Execute(Script);

    SetDisplay_(mEdition, Session);
  qRR;
  qRT;
  qRE;
  }
}

D_( New )
{
  Session.IsNew = true;
  Edit_(str::Empty, str::Empty, Session);
}

D_( Edit )
{
qRH;
  BGRD;
  str::wString Title, Description;
qRB;
  CBNDL();

  tol::Init(Title, Description);
  Bundle.Get(Session.Selected, Title, Description);

  Session.IsNew = false;
  Edit_(Title, Description, Session);
qRR;
qRT;
qRE;
}

namespace {
  void RetrieveContent_(
    sSession &Session,
    str::dString &Title,
    str::dString &Description)
  {
    Session.GetValue("Title", Title);
    Session.Execute("let value = markdown.value(); markdown.toTextArea(); markdown = null; value;", Description);
  }
}

D_(Cancel)
{
qRH;
  BGRD;
  str::wString NewTitle, OldTitle, NewDescription, OldDescription;
qRB;
  tol::Init(NewTitle, OldTitle, NewDescription, OldDescription);

  RetrieveContent_(Session, NewTitle, NewDescription);

  if ( !Session.IsNew ) {
    CBNDL();

    Bundle.Get(Session.Selected, OldTitle, OldDescription);
  }

  if ( ( OldTitle != NewTitle) || ( OldDescription != NewDescription ) ) {
    if ( Session.ConfirmU(str::wString("Are sure you want to cancel your modifications?"), "" ) )
      SetDisplay_(mView, Session);
  }
qRR;
qRT;
qRE;
}

D_( Submit )
{
qRH;
  BGRD;
  str::wString Title, Description;
qRB;
  tol::Init(Title, Description);

  RetrieveContent_(Session, Title, Description);

  Title.StripChars();

  if ( Title.Amount() == 0 ) {
    Session.AlertB(str::wString("Title can not be empty!"));
  } else {
    BNDL();

    if ( Session.IsNew )
      Bundle.Add(Title, Description, Session.Selected);
    else
      Bundle.Set(Title, Description, Session.Selected);
  }

  Session.IsNew = false;

  SetDisplay_(mView, Session);
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
      OnNewSession, Select,
      Edit, New,
      Submit, Cancel);
  }
}

#define R_( name ) Add_(main::Core, actions_::name)

qGCTOR( main ) {
  Core.Init();
  Register_();
}
