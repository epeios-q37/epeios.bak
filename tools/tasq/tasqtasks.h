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


#ifndef TASQTASKS_INC_
# define TASQTASKS_INC_

# include "lstbch.h"
# include "lstcrt.h"
# include "que.h"
# include "str.h"

namespace tasqtasks {
  qROW( SRow );

  typedef lstcrt::qLMCRATEd( str::dString, sSRow ) dStrings;
  qW( Strings );

  qROW( TRow );

  typedef qQUEUEd( sTRow ) dQueue;
  qW( Queue );

  typedef qQUEUEMs( sTRow ) sQManager;

  class sTask {
  public:
    sSRow
      Title,
      Description;
    sTRow Parent;
    sQManager Children;
    void reset(bso::sBool P = true)
    {
      Title = Description = qNIL;
      Parent = qNIL;
      tol::reset( P, Children );
    }
    qCDTOR(sTask);
    void Init(const dQueue &Queue)
    {
      Title = Description = qNIL;
      Parent = qNIL;
      Children.Init(Queue);
    }
  };


  typedef lstbch::qLBUNCHd( sTask, sTRow ) dTasks;
  qW(Tasks);

  qHOOKS3(lstcrt::sHooks, Strings, lstbch::sHooks, Tasks, que::sHook, Queue);

  class dBundle {
  private:
    qASd AS_;
    void StoreMain_(void);
    void Retrieve_(
      sTRow Row,
      sQManager &Manager)
    {
      if ( Row == qNIL )
        Manager = S_.Main;
      else if ( Tasks.Exists(Row) )
        Manager = Tasks(Row).Children;
      else
        qRGnr();
    }
    void Store_(
      const sQManager &Manager,
      sTRow Row)
    {
      if ( Row == qNIL ) {
        S_.Main = Manager;
        StoreMain_();
      } else if ( Tasks.Exists(Row) ) {
        sTask Task;
        Task.Init(Queue);

        Tasks.Recall(Row, Task);
        Task.Children = Manager;
        Tasks.Store(Task, Row);
      } else
        qRGnr();
    }
    sSRow Add_(const str::dString &String)
    {
      sSRow Row = Strings.New();

      Strings(Row) = String;

      StoreMain_();

      return Row;
    }
    const sTask GetTask_(sTRow Row) const
    {
      sTask Task;

      Task.Init(Queue);

      Tasks.Recall(Row, Task);

      return Task;
    }
  public:
    struct s {
      qASd::s AS;
      dStrings::s Strings;
      dTasks::s Tasks;
      dQueue::s Queue;
      sQManager Main; // Main tasks
    } &S_;
    dStrings Strings;
    dTasks Tasks;
    dQueue Queue;
    dBundle( s &S)
    : S_(S),
      AS_(S.AS),
      Strings(S.Strings),
      Tasks(S.Tasks),
      Queue(S.Queue)
    {}
    void reset(bso::sBool P = true)
    {
      tol::reset(P, AS_, Strings, Tasks, Queue, S_.Main);
    }
    void plug(uys::sHook &Hook)
    {
      AS_.plug(Hook);
      plug(&AS_);
    }
    void plug(qASd *AS)
    {
      Strings.plug(AS);
      Tasks.plug(AS);
      Queue.plug(AS);
    }
    dBundle &operator =(const dBundle &B)
    {
      Strings = B.Strings;
      Tasks = B.Tasks;
      Queue = B.Queue;
      S_.Main = B.S_.Main;

      return *this;
    }
    void Init(void)
    {
      tol::Init(AS_, Strings, Tasks, Queue);
      S_.Main.Init(Queue);
    }
    sTRow Add(
      const str::dString &Title,
      const str::dString &Description,
      sTRow Row = qNIL)
    {
      sQManager Manager;
      sTask Task;
      sTRow New = qNIL;

      Manager.Init(Queue);
      Retrieve_(Row, Manager);

      Task.Init(Queue);

      Task.Title = Add_(Title);

      if ( Description.Amount() )
        Task.Description = Add_(Description);

      Task.Parent = Row;

      New = Tasks.Add(Task);
      Queue.Allocate(Tasks.Extent());

      Manager.BecomeLast(New, Queue);

      Store_(Manager, Row);
      StoreMain_();

      return New;
    }
    sTRow Add(
      const str::dString &Title,
      sTRow Row)
    {
      return Add(Title, str::Empty, Row);
    }
    sTRow Parent(sTRow Row) const
    {
      return GetTask_(Row).Parent;
    }
    sTRow Next(sTRow Row = qNIL) const
    {
      if ( Row == qNIL )
        return S_.Main.First(Queue);
      else
        return Queue.Next(Row);
    }
    sTRow Previous(sTRow Row = qNIL) const
    {
      if ( Row == qNIL )
        return S_.Main.Last(Queue);
      else
        return Queue.Previous(Row);
    }
    sTRow First(sTRow Row) const
    {
      if ( Row == qNIL )
        return S_.Main.First(Queue);
      else
        return GetTask_(Row).Children.First(Queue);
    }
    sTRow Last(sTRow Row) const
    {
      if ( Row == qNIL )
        return S_.Main.Last(Queue);
      else
        return GetTask_(Row).Children.Last(Queue);
    }
    void Get(
      sTRow Row,
      str::dString &Title,
      str::dString &Description) const
    {
      sTask Task;

      Task.Init(Queue);

      Task = GetTask_(Row);

      Strings.Recall(Task.Title, Title);

      if ( Task.Description != qNIL )
        Strings.Recall(Task.Description, Description);
    }
    void Set(
      const str::dString &Title,
      const str::dString &Description,
      sTRow Row)
      {
        sTask Task;

        Task.Init(Queue);

        Task = GetTask_(Row);

        Strings.Store(Title, Task.Title);

        if ( Task.Description == qNIL) {
          Task.Description = Strings.New();
          Tasks.Store(Task, Row);
        }

        Strings.Store(Description, Task.Description);

        StoreMain_();
      }
    };

  qW(Bundle);


  class rXBundle
  : public wBundle
  {
  public:
    sTRow Selected;
    void reset(bso::sBool P = true)
    {
      wBundle::reset(P);
      Selected = qNIL;
    }
    qCDTOR(rXBundle);
    void Init(void)
    {
      wBundle::Init();
      Selected = qNIL;
    }
  };

  void Initialize(void);

  typedef mtx::rHandle hGuard;

  rXBundle &Get(hGuard &Guard);

  const rXBundle &CGet(hGuard &Guard);
}

#endif
