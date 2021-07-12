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

// TaSK TASKS

#ifndef TSKTASKS_INC_
# define TSKTASKS_INC_

# ifdef XXX_DBG
# define TSKXMP_DBG_
# endif

# include "tskbsc.h"

# include "cio.h"

# include "ags.h"
# include "lstbch.h"
# include "lstcrt.h"
# include "str.h"
# include "xml.h"

namespace tsktasks {
	qROW(CRow);	// Content row.
	typedef bso::sU16 sLevel;

	qENUM( Kinship ) {
	  kFirst,
    kChild,
    kSibling,
    k_amouunt,
    k_Undefined
	};

	// Static data which are not references.
	// Will contain some data in the next versions of the software.
	struct sTask_
	{
  public:
    void reset(bso::sBool = true)
    {
      // Standardization.
    }
    qCDTOR(sTask_);
    void Init(void)
    {
      // Standardization.
    }
	};

	// As stored in the repository.
	// Dynamic objects are stored apart and referenced.
	struct sTask
	: public sTask_
	{
	public:
		sCRow
			Label,
			Description;
		void reset(bso::sBool P = true)
		{
		  sTask_::reset(P);
			Label = Description = qNIL;
		}
		qCDTOR(sTask);
		void Init(void) {
			reset();

			sTask_::Init();
		}
	};

  // Used as buffer, embedding dynamic data.
	struct rTask
	: public sTask_
	{
	public:
		str::wString
			Label,
			Description;
		void reset(bso::sBool P = true)
		{
		  sTask_::reset(P);
		  tol::reset(P, Label, Description);
		}
		qCDTOR(rTask);
		void Init(void) {
			sTask_::Init();
			tol::Init(Label, Description);
		}
	};

	qROW(TRow);	// Task row.

	struct sHub {
		sTRow
			Parent,
			Prev,
			Next,
			First,
			Last;
		void reset(bso::sBool = true)
		{
			Parent = Prev = Next = First = Last = qNIL;
		}
		qCDTOR(sHub);
		void  Init(void)
		{
			reset();
		}
	};

	typedef lstcrt::qLCRATEd(str::dString, sCRow) dContents_;
  typedef lstbch::qLBUNCHd(sTask, sTRow) dTasks_;
	typedef lstbch::qLBUNCHd(sHub, sTRow) dHubs_;

	class rCore_
	{
  private:
    struct s {
      qASd::s AggregatedStorage;
      dContents_::s Contents;
      dTasks_::s Tasks;
      dHubs_::s Hubs;
    } S_;
    uys::rFOH<sizeof(S_)> Hooks_;
		qASd AggregatedStorage_;
		void WriteStatics_(void)
		{
      Contents.Flush();

      if ( Hooks_.IsInitialized() )
        Hooks_.Put((const sdr::sByte *)&S_);
		}
  public:
    dContents_ Contents;
    dTasks_ Tasks;
    dHubs_ Hubs;
    void reset(bso::sBool P = true)
    {
		  if ( P )
        WriteStatics_();

		  Hooks_.reset(P);
      tol::reset(false, Contents, Tasks, Hubs, AggregatedStorage_);
    }
    rCore_(void)
    : AggregatedStorage_(S_.AggregatedStorage),
      Contents(S_.Contents),
      Tasks(S_.Tasks),
      Hubs(S_.Hubs)
    {
      reset(false);
    }
    qDTOR(rCore_);
    bso::sBool Init(const str::dString &Repository)
    {
      WriteStatics_();

      AggregatedStorage_.plug(Hooks_);
      tol::plug(&AggregatedStorage_, Contents, Tasks, Hubs);

      if ( Hooks_.Init(Repository, uys::mReadWrite) == uys::sExists ) {
        Hooks_.Get((sdr::sByte *)&S_);
        return true;
      } else {
        tol::Init(Contents, Tasks, Hubs);
        return false;
      }
    }
    sCRow Add(const str::dString &Content)
    {
      sCRow Row = Contents.New();

      Contents(Row).Init(Content);

      return Row;
    }
    void Remove(sCRow Row)
    {
      if ( !Contents.Exists(Row) )
        qRGnr();

      Contents.Remove(Row);
    }
		sTRow Append(
			sTRow ChildRow,
			sTRow ParentRow)
			{
				sHub Child, Parent;

				tol::Init(Child, Parent);

				Hubs.Recall(ParentRow, Parent);

				if ( Parent.First == qNIL ) {
					if ( Parent.Last != qNIL )
						qRFwk();

					Parent.First = Parent.Last = ChildRow;
				} else if ( Parent.Last == qNIL ) {
					qRFwk();
				} else {
					sHub Sibling;

					Sibling.Init();

					Hubs.Recall(Parent.Last, Sibling);

					if ( Sibling.Next != qNIL)
						qRFwk();

					Sibling.Next = ChildRow;

					Hubs.Store(Sibling, Parent.Last);

					Child.Prev = Parent.Last;

					Parent.Last = ChildRow;
				}

				Child.Parent = ParentRow;

				Hubs.Store(Child, ChildRow);
				Hubs.Store(Parent, ParentRow);

				return ChildRow;
			}
			sTRow Append(
        const sTask &Task,  // Must be orphan.
        sTRow Row)
      {
        sTRow ChildRow = Tasks.New();
        Tasks.Store(Task, ChildRow);

        if ( ChildRow != Hubs.New() )
          qRFwk();

        return Append(ChildRow, Row);
      }
	};

	class cBrowser
	{
  protected:
    virtual void TSKTasks(sTRow Row) = 0; // Id Row == qNIL, all the tasks are browsed.
    virtual void TSKTask(
      eKinship Kinship,
      sLevel Level,
      sTRow Row,
      const str::dString &Label,
      const str::dString &Description) = 0;
    virtual void TSKParent(sLevel Level) = 0;
  public:
    qCALLBACK(Browser);
    void Tasks(sTRow Row)
    {
      return TSKTasks(Row);
    }
    void Task(
      eKinship Kinship,
      sLevel Level,
      sTRow Row,
      const str::dString &Label,
      const str::dString &Description)
    {
      return TSKTask(Kinship, Level, Row, Label, Description);
    }
    void Parent(sLevel Level)
    {
      return TSKParent(Level);
    }
	};

	class rTasks {
	private:
	  str::wString Repository_;
		rCore_ Core_;
		sTRow Root_;
			sHub GetHub_(sTRow Row) const
			{
				sHub Hub;

				Hub.Init();

				Core_.Hubs.Recall(Row, Hub);

				return Hub;
			}
			bso::sBool TestAndGet_(
        sTRow (rTasks::* Get)(sTRow) const,
        sTRow &Row) const
      {
        sTRow Buffer = (*this.*Get)(Row);

        if ( Buffer != qNIL ) {
          Row = Buffer;
          return true;
        } else
          return false;
      }
			sTRow GetParent_(sTRow Row) const
			{
				return GetHub_(Row).Parent;
			}
			bso::sBool TestAndGetParent_(sTRow &Row) const
			{
			  return TestAndGet_(&rTasks::GetParent_, Row);
			}
			sTRow GetPrev_(sTRow Row) const
			{
				return GetHub_(Row).Prev;
			}
			sTRow GetNext_(sTRow Row) const
			{
				return GetHub_(Row).Next;
			}
			bso::sBool TestAndGetNext_(sTRow &Row) const
			{
			  return TestAndGet_(&rTasks::GetNext_, Row);
			}
      sTRow GetFirst_(sTRow Row) const
			{
				return GetHub_(Row).First;
			}
			bso::sBool TestAndGetFirst_(sTRow &Row) const
			{
			  return TestAndGet_(&rTasks::GetFirst_, Row);
			}
			sTRow GetLast_(sTRow Row) const
			{
				return GetHub_(Row).Last;
			}
			sTask GetTask_(sTRow Row) const
			{
				sTask Task;

				Task.Init();

				Core_.Tasks.Recall(Row, Task);

				return Task;
			}
			sCRow GetLabel_(sTRow Row) const
			{
				return GetTask_(Row).Label;
			}
			sCRow GetDescription_(sTRow Row) const
			{
				return GetTask_(Row).Description;
			}
			const str::dString &GetContent_(
				sCRow Row,
				str::dString &Content) const
			{
				if ( Row != qNIL )
					Core_.Contents.Recall(Row, Content);

				return Content;
			}
			const str::dString &GetLabel_(
				sTRow Row,
				str::dString &Label) const
			{
				return GetContent_(GetLabel_(Row), Label);
			}
			const str::dString &GetDescription_(
				sTRow Row,
				str::dString &Label) const
			{
				return GetContent_(GetDescription_(Row), Label);
			}
	public:
		void reset(bso::sBool P = true)
		{
		  tol::reset(P, Repository_, Core_);
			Root_ = qNIL;
		}
		qCDTOR(rTasks);
    void Init(void);
    qRODISCLOSEr(str::dString, Repository);
		bso::sBool Exists(sTRow Row) const
		{
		  if ( Row == qNIL )
        return true;

      return Core_.Tasks.Exists(Row);
		}
		sTRow Append(
      const rTask &Task,
      sTRow Row);
    void SetDescription(
      sTRow Row,
      const str::dString &Description)
    {
      sTask Task = GetTask_(Row);

      if ( Task.Description == qNIL )
        Task.Description = Core_.Contents.New();

      Core_.Contents(Task.Description).Init(Description);

      Core_.Tasks.Store(Task, Row);
    }
    void Browse(
      sTRow Row,
      cBrowser &Browser) const;
	};
}


#endif
