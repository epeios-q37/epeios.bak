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

# include "bso.h"
# include "dtr.h"
# include "lstbch.h"
# include "lstcrt.h"
# include "str.h"

namespace tasqtasks {
  qROW( SRow );

  typedef lstcrt::qLMCRATEd( str::dString, sSRow ) dStrings;
  qW( Strings );

  qROW( TRow );

  typedef dtr::qDTREEd( sTRow ) dTree;

  class sTask {
  public:
    sSRow
      Title,
      Description;
    void reset(bso::sBool P = true)
    {
      Title = Description = qNIL;
    }
    qCDTOR(sTask);
    void Init(void)
    {
      Title = Description = qNIL;
    }
  };


  typedef lstbch::qLBUNCHd( sTask, sTRow ) dTasks;
  qW(Tasks);

  class dBundle
  : public dTree
  {
  private:
    sSRow Add_(const str::dString &String)
    {
      sSRow Row = Strings.New();

      Strings(Row) = String;

      StoreStatics_();

      return Row;
    }
    const sTask GetTask_(sTRow Row) const
    {
      sTask Task;

      Task.Init();

      Tasks.Recall(Row, Task);

      return Task;
    }
  protected:
    virtual void StoreStatics_(void) = 0;
  public:
    struct s
    : public dTree::s
    {
      qASd::s AS;
      dStrings::s Strings;
      dTasks::s Tasks;
      sTRow Root; // Main tasks
    } &S_;
    dStrings Strings;
    dTasks Tasks;
    dBundle(s &S)
    : dTree(S),
      S_(S),
      Strings(S.Strings),
      Tasks(S.Tasks)
    {}
    void reset(bso::sBool P = true)
    {
      tol::reset(P, Strings, Tasks);
      dTree::reset(P);
      S_.Root = qNIL;
    }
    void plug(qASd *AS)
    {
      tol::plug(AS, Strings, Tasks);
      dTree::plug(AS);
    }
    dBundle &operator =(const dBundle &B)
    {
      Strings = B.Strings;
      Tasks = B.Tasks;
      dTree::operator=(B);
      S_.Root = B.S_.Root;

      return *this;
    }
    void Init(void)
    {
      sTask Task;
      tol::Init(Strings, Tasks);
      dTree::Init();
      Task.Init();
      S_.Root = Tasks.Add(Task);
      dTree::Allocate(Tasks.Extent());
    }
    void Flush(void)
    {
      Strings.Flush();
    }
    sTRow Root(void) const
    {
      if ( S_.Root == qNIL )
        qRGnr();

      return S_.Root;
    }
    bso::sBool IsRoot(sTRow Row) const
    {
      return Row == Root();
    }
    sTRow Next(void) const
    {
      return Next(S_.Root);
    }
    using dTree::Next;
    sTRow Add(
      const str::dString &Title,
      const str::dString &Description,
      sTRow Row = qNIL)
    {
      sTask Task;
      sTRow New = qNIL;

      if ( Row == qNIL )
        Row = S_.Root;

      Task.Init();

      Task.Title = Add_(Title);

      if ( Description.Amount() )
        Task.Description = Add_(Description);

      New = Tasks.Add(Task);
      dTree::Allocate(Tasks.Extent());

      BecomeLast(New, Row);

      StoreStatics_();

      return New;
    }
    sTRow Add(
      const str::dString &Title,
      sTRow Row)
    {
      return Add(Title, str::Empty, Row);
    }
    sTRow UpdateDescription(
      sTRow Row,
      const str::dString &Description)
    {
      sTask Task;

      Task.Init();

      Task = GetTask_(Row);

      if ( Description.Amount() ) {
        if ( Task.Description == qNIL )
          Task.Description = Add_(Description);
        else
          Strings(Task.Description) = Description;
      } else if ( Task.Description != qNIL ) {
          Strings.Remove(Task.Description);
          Task.Description = qNIL;
      }

      Tasks.Store(Task, Row);

      StoreStatics_();

      return Row;
    }
    void Get(
      sTRow Row,
      str::dString &Title,
      str::dString &Description) const
    {
      sTask Task;

      Task.Init();

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

        Task.Init();

        Task = GetTask_(Row);

        Strings.Store(Title, Task.Title);

        if ( Task.Description == qNIL) {
          Task.Description = Strings.New();
          Tasks.Store(Task, Row);
        }

        Strings.Store(Description, Task.Description);

        StoreStatics_();
      }
    };

  class rXBundle
  : public dBundle
  {
  private:
    qASd AS_;
  protected:
    virtual void StoreStatics_(void) override;
  public:
    struct s
    : public dBundle::s
    {
     qASd::s AS;
    } S_;
    rXBundle()
    : dBundle(S_),
      AS_(S_.AS)
    {}
    void reset(bso::sBool P = true)
    {
      dBundle::reset(P);
      AS_.reset(P);
    }
    void plug(uys::sHook &Hook)
    {
      AS_.plug(Hook);
      dBundle::plug(&AS_);
    }
    qDTOR(rXBundle);
    void Init(void)
    {
      AS_.Init();
      dBundle::Init();
    }
    void Immortalize(void)
    {
      Flush();
      reset(false);
    }
};

  // Returns true if db exists.
  bso::sBool Initialize(const fnm::rName &Name);

  void Immortalize(void);

  typedef mtx::rHandle hGuard;

  rXBundle &Get(hGuard &Guard);

  const rXBundle &CGet(hGuard &Guard);
}

#endif
