/*
	Copyright (C) 2022 Claude SIMON (http://q37.info/contact/).

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


#include "tasqxml.h"

using namespace tasqxml;

namespace tasks = tasqtasks;

namespace {
  qENUM(Token_) {
    tItems,
    tItem,
    tId,
    tSelected,
    tTitle,
    tDescription,
    t_amount,
    t_Undefined
  };

  const char *GetLabel_(eToken_ Token)
  {
    const char *Label = NULL;

    switch ( Token ) {
    case tItems:
      Label = "Items";
      break;
    case tItem:
      Label = "Item";
      break;
    case tId:
      Label = "id";
      break;
    case tSelected:
      Label = "Selected";
      break;
    case tTitle:
      Label = "Title";
      break;
    case tDescription:
      Label = "Description";
      break;
/*
    case:
      Label = "";
      break;
*/
    default:
      qRGnr();
    }

    return Label;
  }

  namespace _ {
    stsfsm::wAutomat Automat;
  }

  void FillAutomat_( void )
  {
    _::Automat.Init();
    stsfsm::Fill<eToken_>(_::Automat, t_amount, GetLabel_);
  }

  eToken_ GetToken_( const str::dString &Pattern )
  {
    return stsfsm::GetId(Pattern, _::Automat, t_Undefined, t_amount);
  }
}

#define L_(token)  GetLabel_( t##token )

namespace {
  void WriteDescription_(
    const str::dString &Description,
    xml::rWriter &Writer)
  {
    if ( Description.Amount() ) {
      Writer.PushTag(L_( Description ));
      Writer.PutCData(Description);
      Writer.PopTag();
    }
  }
}

void tasqxml::Write(
  const tasqtasks::dBundle &Bundle,
  int Flags,
  xml::rWriter &Writer)
{
qRH;
  tasks::sTask Task;
  tasks::sTRow Row = qNIL,
  Candidate = qNIL;
  bso::sBool SkipChildren = false;
qRB;
  Writer.PushTag(L_( Items ));
  Row = Bundle.First(Row);

  while ( Row != qNIL ) {
    if ( !SkipChildren ) {
      Writer.PushTag(L_( Item ));
      if ( Flags & ffId )
        Writer.PutAttribute(L_( Id ), *Row);

      Task.Init(Bundle.Queue);
      Bundle.Tasks.Recall(Row, Task);
      Writer.PutAttribute(L_( Title ), Bundle.Strings(Task.Title));
      if ( ( Flags & ffDescription ) && ( Task.Description != qNIL ) )
        WriteDescription_(Bundle.Strings(Task.Description), Writer);
    }

    if ( !SkipChildren && ( Candidate = Bundle.First(Row) ) != qNIL ) {
      Writer.PushTag(L_( Items ) );
      Row = Candidate;
    } else if( ( Candidate = Bundle.Next(Row) ) != qNIL ) {
      Writer.PopTag();  // 'Item'
      Row = Candidate;
      SkipChildren = false;
    } else {
      Row = Bundle.Parent(Row);
      Writer.PopTag();  // 'Item'
      Writer.PopTag();  // 'Items'.
      SkipChildren = true;
    }
  }

qRR;
qRT;
qRE;
}

namespace {
  void ParseItems_(
    xml::rParser &Parser,
    tasqtasks::dBundle &Bundle)
    {
    qRH;
      bso::sBool Continue = true;
      str::wString Title, Description;
      stkbch::qBSTACKwl( eToken_ ) Tags;
      tasqtasks::sTRow Row = qNIL;
    qRB;
      if ( GetToken_(Parser.TagName()) != tItems )
        qRGnr();

      tol::Init(Tags, Title, Description);

      Tags.Push(tItems);

      while ( Continue ) {
        switch ( Parser.Parse(xml::tfObvious) ) {
        case xml::tStartTag:
          Tags.Push(GetToken_(Parser.TagName()));

          switch ( Tags.Top() ) {
          case tItems:
            break;
          case tItem:
            tol::Init(Title, Description);
            break;
          case tDescription:
            if ( Description.Amount() )
              qRGnr();
            break;
          default:
            qRGnr();
            break;
          }
          break;
          case xml::tAttribute:
            if ( Tags.Top() != tItem )
              qRFwk();

            switch( GetToken_(Parser.AttributeName())) {
            case tId:
              qRGnr();
              break;
            case tTitle:
              if ( Title.Amount() )
                qRGnr();

              Title.Init(Parser.Value());
              break;
            default:
              qRGnr();
              break;
            }
            break;
          case xml::tValue:
          case xml::tCData:
            switch ( Tags.Top() ) {
            case tDescription:
              if ( Description.Amount() )
                qRGnr();
              Description.Init(Parser.Value());
              break;
            default:
              break;
            }
            break;
          case xml::tEndTag:
            if ( Tags.Top() != GetToken_(Parser.TagName()) )
              qRGnr();

            switch ( Tags.Pop() ) {
            case tItems:
              if ( Tags.Amount() )
                Row = Bundle.Parent(Row);
              else
                Continue = false;
              break;
            case tItem:
              if ( !Title.Amount() )
                qRGnr();

              Row = Bundle.Add(Title, Description, Row);
              break;
            case tDescription:
              break;
            default:
              qRGnr();
              break;
            }
            break;
          default:
            qRGnr();
            break;
        }
      }
    qRR;
    qRT;
    qRE;
    }
}

void tasqxml::Parse(
  xml::rParser &Parser,
  tasqtasks::dBundle &Bundle)
{
qRH;
  bso::sBool Continue = true;
qRB;
  while ( Continue ) {
    switch ( Parser.Parse(xml::tfObvious) ) {
    case xml::tStartTag:
      switch ( GetToken_(Parser.TagName() ) ) {
      case tItems:
        ParseItems_(Parser, Bundle);
        break;
      default:
        qRGnr();
        break;
    }
    case xml::t_Processed:
      Continue = false;
      break;
    case xml::t_Error:
      qRGnr();
      break;
    default:
      qRGnr();
      break;
    }
  }
qRR;
qRT;
qRE;
}

qGCTOR( tasqxml )
{
  FillAutomat_();
}
