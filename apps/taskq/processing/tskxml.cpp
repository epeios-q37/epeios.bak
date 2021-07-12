/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tskxml.h"

#include "tskinf.h"

#include "sclm.h"

using namespace tskxml;

using namespace tsktasks;

namespace {
  namespace {
    qENUM( Markup ) {
      mRootTag,
      mTasksTag,
      mTaskTag,
      mDetailsTag,
      mSubTasksTag,
      mDescriptionTag,
      mVersionAttr,
      mRepositoryAttr,
      mTimestampAttr,
      mGeneratorAttr,
      mTypeAttr,
      mRowAttr,
      mLabelAttr,
      mMainVal,
      mSubVal,
      m_amount,
      m_Undefined
    };

#define G_(label, suffix)\
case m##label##suffix:\
  return #label;\
  break

#define T(label)  G_( label, Tag )
#define A(label)  G_( label, Attr )
#define V(label)  G_( label, Val )

    const char *GetLabel_(eMarkup Markup )
    {
      switch( Markup ) {
      case mRootTag:
        return TSKINF_MC;
        break;
      T( Tasks );
      T( Task );
      T( Details );
      T( SubTasks );
      T( Description );
      A( Version );
      A( Repository );
      A( Timestamp );
      A( Generator );
      A( Type );
      V( Main );
      V( Sub );
      case mRowAttr:
        return "row";
        break;
      A( Label );
      default:
        qRFwk();
        break;
      }

      return NULL;  // To avoid a warning.
    }

    stsfsm::wAutomat MarkupAutomat_;

    void FillMarkupAutomat_( void )
    {
      MarkupAutomat_.Init();
      stsfsm::Fill<eMarkup>( MarkupAutomat_, m_amount, GetLabel_ );
    }

    eMarkup GetMarkup_( const str::dString &Pattern )
    {
      return stsfsm::GetId( Pattern, MarkupAutomat_, m_Undefined, m_amount );
    }
  }

#define L(markup) GetLabel_(m##markup)

  typedef bso::sU16 sPending_;
  const sPending_ &PendingMax_ = bso::U16Max;

	class sBrowser_
	: public cBrowser
	{
  private:
    xml::rWriter Writer_;
    sPending_ PendingTasks_;
  protected:
	  virtual void TSKTasks(sTRow Row) override
    {
      Writer_.PushTag(L( TasksTag ));

      if ( Row == qNIL ) {
        Writer_.PutAttribute(L( TypeAttr ), L( MainVal ));
      } else {
        Writer_.PutAttribute(L( RowAttr ), *Row);
        Writer_.PutAttribute(L( TypeAttr ), L( SubVal ));
      }
    }
    virtual void TSKTask(
      eKinship Kinship,
      sLevel Level,
      sTRow Row,
      const str::dString &Label,
      const str::dString &Description) override
    {
      switch ( Kinship ) {
      case kFirst:
        break;
      case kChild:
        Writer_.PushTag(L( SubTasksTag ));
        break;
      case kSibling:
        Writer_.PopTag();
        break;
      default:
        qRFwk();
        break;
      }

      Writer_.PushTag(L( TaskTag ));
      Writer_.PutAttribute(L( RowAttr ), *Row);

      Writer_.PushTag(L( DetailsTag ));

      Writer_.PutAttribute(L( LabelAttr ), Label);

      if ( Description.Amount() )
        Writer_.PutValue(Description, L( DescriptionTag ));

      Writer_.PopTag();

      if ( PendingTasks_ >= PendingMax_ )
        qRLmt();

      PendingTasks_++;

    }
    virtual void TSKParent(sLevel Level) override
    {
      if ( PendingTasks_ > 0 ) {
        Writer_.PopTag();
        PendingTasks_--;
      }

      Writer_.PopTag();
    }
  public:
    void reset(bso::sBool P = true)
    {
      Writer_.reset(P);
      PendingTasks_ = 0;
    }
    qCVDTOR(sBrowser_);
    void Init(
      txf::sWFlow &Flow,
      const str::dString &Repository,
      const char *Generator)
    {
      tol::bDateAndTime Buffer;

      reset();

      Writer_.Init(Flow, xml::lIndent, xml::fEncoding());

      Writer_.PushTag(L( RootTag) );
      Writer_.PutAttribute(L( VersionAttr ), TSKINF_SOFTWARE_VERSION);
      Writer_.PutAttribute(L( RepositoryAttr ), Repository);
      Writer_.PutAttribute(L( TimestampAttr ), tol::DateAndTime(Buffer));
      Writer_.PutAttribute(L( GeneratorAttr ), Generator);
    }
  };
}

void tskxml::Export(
  const rTasks &Tasks,
  sTRow Row,
  txf::sWFlow &Flow,
  const char *Generator)
{
qRH;
  sBrowser_ Browser;
qRB;
  Browser.Init(Flow, Tasks.Repository(), Generator);

  Tasks.Browse( Row, Browser);
qRR;
qRT;
qRE;
}

namespace {
  typedef stkbch::qBSTACKdl(sTRow) dStack_;
  qW(Stack_);

  void Report_(
    const xml::rParser &Parser,
    const char *Location)
  {
    sclm::ReportAndAbort("TSKXML_MarkupError", Parser.DumpPosition().Line, Parser.DumpPosition().Column, Location);
  }

#define REPORT()  Report_(Parser, __LOC__)

  void HandleTask_(
    xml::rParser &Parser,
    dStack_ &Stack,
    rTasks &Tasks);

  // <SubTasks …>…</SubTasks>
  //           ^
  //                         ^
  void HandleSubTasks_(
    xml::rParser &Parser,
    dStack_ &Stack,
    rTasks &Tasks)
  {
    bso::sBool Cont = true;

    while( Cont ) {
      switch( Parser.Parse(xml::tfStartTag | xml::tfEndTag) ) {
      case xml::tStartTag:
        if ( GetMarkup_(Parser.GetTagName()) != mTaskTag )
          REPORT();
        HandleTask_(Parser, Stack, Tasks);
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mSubTasksTag )
          REPORT();
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  }


  // <Description …>…</Description>
  //              ^
  //                               ^
  void HandleDescription_(
    xml::rParser &Parser,
    rTask &Task)
  {
    bso::sBool Cont = true;

    while( Cont ) {
      switch( Parser.Parse(xml::tfValue | xml::tfEndTag) ) {
      case xml::tValue:
        if ( Task.Description.Amount() != 0 )
          REPORT();

        Task.Description = Parser.GetValue();
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mDescriptionTag )
          REPORT();
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  }

  // <Details …>…</Details>
  //          ^
  //                       ^
  sTRow HandleDetails_(
    xml::rParser &Parser,
    sTRow ParentRow,
    rTasks &Tasks)
  {
    sTRow Row = qNIL;
  qRH;
    rTask Task;
    bso::sBool Cont = true;
  qRB;
    Task.Init();

    while( Cont ) {
      switch( Parser.Parse( xml::tfStartTag | xml::tfAttribute | xml::tfEndTag ) ) {
      case xml::tAttribute:
        switch( GetMarkup_(Parser.GetAttributeName()) ) {
        case mLabelAttr:
          Task.Label = Parser.GetValue();
          break;
        default:
          REPORT();
          break;
        }
        break;
      case xml::tStartTag:
        switch( GetMarkup_(Parser.TagName()) ) {
        case mDescriptionTag:
          HandleDescription_(Parser, Task);
          break;
        default:
          REPORT();
          break;
        }
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mDetailsTag )
          REPORT();
        Row = Tasks.Append(Task, ParentRow);
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  qRR;
  qRT;
  qRE;
    return Row;
  }


  // <Task …>…</Task>
  //       ^
  //                 ^
  void HandleTask_(
    xml::rParser &Parser,
    dStack_ &Stack,
    rTasks &Tasks)
  {
  qRH;
    bso::sBool Cont = true;
    sTRow Row = qNIL;
  qRB;
    while( Cont ) {
      switch( Parser.Parse(xml::tfStartTag | xml::tfEndTag) ) {
      case xml::tStartTag:
        switch( GetMarkup_(Parser.TagName()) ) {
        case mDetailsTag:
          Row = HandleDetails_(Parser, Stack.Top(), Tasks);

          if ( Row == qNIL )
            REPORT();
          break;
        case mSubTasksTag:
          if ( Row == qNIL )
            REPORT();

          Stack.Push(Row),
          HandleSubTasks_(Parser, Stack, Tasks);
          Stack.Pop();
          break;
        default:
          REPORT();
          break;
        }
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mTaskTag )
          REPORT();
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  qRR;
  qRT;
  qRE;
  }

  // <Tasks …>…</Tasks>
  //           ^
  //                         ^
  void HandleTasks_(
    xml::rParser &Parser,
    dStack_ &Stack,
    rTasks &Tasks)
  {
    bso::sBool Cont = true;

    while( Cont ) {
      switch( Parser.Parse(xml::tfStartTag | xml::tfEndTag) ) {
      case xml::tStartTag:
        if ( GetMarkup_(Parser.GetTagName()) != mTaskTag )
          REPORT();
        HandleTask_(Parser, Stack, Tasks);
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mTasksTag )
          REPORT();
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  }

  // <TasQ …>…</TaskQ>
  //       ^
  //                  ^
  void HandleRoot_(
    xml::rParser &Parser,
    sTRow Row,
    rTasks &Tasks)
  {
  qRH;
    wStack_ Stack;
    bso::sBool Cont = true;
  qRB;
    Stack.Init();
    Stack.Push(Row);

    while( Cont ) {
      switch( Parser.Parse(xml::tfStartTag | xml::tfEndTag) ) {
      case xml::tStartTag:
        if ( GetMarkup_(Parser.GetTagName()) != mTasksTag )
          REPORT();
        HandleTasks_(Parser, Stack, Tasks);
        break;
      case xml::tEndTag:
        if ( GetMarkup_(Parser.GetTagName()) != mRootTag )
          REPORT();
        Cont = false;
        break;
      default:
        REPORT();
        break;
      }
    }
  qRR;
  qRT;
  qRE;
  }
}

void tskxml::Import(
    xtf::sRFlow &Flow,
    tsktasks::sTRow Row,
    tsktasks::rTasks &Tasks)
{
qRH;
  xml::rParser Parser;
qRB;
  Parser.Init(Flow, xml::eh_Default);

  if ( Parser.Parse( xml::tfStartTag ) != xml::tStartTag )
    REPORT();

  if ( GetMarkup_(Parser.GetTagName()) != mRootTag )
    REPORT();

  HandleRoot_(Parser, Row, Tasks);
qRR;
qRT;
qRE;
}


qGCTOR( tskxml )
{
	FillMarkupAutomat_();
}


