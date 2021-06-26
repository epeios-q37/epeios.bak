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
  typedef osd::sDriver sOffsetStorageDriver;

  typedef flsq::rFileOffsetDriver rFileOffsetStorageDriver;

  typedef uys::rH_<rFileOffsetStorageDriver> rH_;

  class rHook
  : public rH_
  {
  public:
    void reset(bso::sBool P = true)
    {
      Driver_.reset(P);
    }
    qCDTOR(rHook);
    bso::sBool Init(sdr::sSize Offset)
    {
      Driver_.Init(flsq::Undefined, Offset, "Essai.q37");
      Driver_.Persistent();

      return Driver_.FileExists();
    }
    bso::sBool IsInitialized(void) const
    {
      return Driver_.IsInitialized();
    }
    void Store(
      const sdr::sByte *Buffer,
      sdr::sSize Amount)
      {
        return Driver_.Store(Buffer, Amount);
      }
    void Recall(
      sdr::sSize Amount,
      sdr::sByte *Buffer)
      {
        return Driver_.Recall(Amount, Buffer);
      }
  };

	qROW(CRow);	// Content row.

	struct sTask {
	public:
		sCRow
			Label,
			Description;
		void reset(bso::sBool = true)
		{
			Label = Description = qNIL;
		}
		qCDTOR(sTask);
		void Init(void) {
			reset();
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
		rHook Hooks_;
		qASd AggregatedStorage_;
    struct s {
      qASd::s AggregatedStorage;
      dContents_::s Contents;
      dTasks_::s Tasks;
      dHubs_::s Hubs;
    } S_;
  public:
    dContents_ Contents;
    dTasks_ Tasks;
    dHubs_ Hubs;
    void reset(bso::sBool P = true)
    {
		  if ( P ) {
        Contents.Flush();
        if ( Hooks_.IsInitialized() )
          Hooks_.Store((const sdr::sByte *)&S_, sizeof(S_));
		  }

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
    bso::sBool Init(void)
    {
      AggregatedStorage_.plug(Hooks_);
      tol::plug(&AggregatedStorage_, Contents, Tasks, Hubs);

      if ( !Hooks_.Init(sizeof(S_)) ) {
        tol::Init(Contents, Tasks, Hubs);
        return false;
      } else {
 			  Hooks_.Recall(sizeof(S_), (sdr::sByte *)&S_);
        return true;
      }
    }
	};

	class rTasks {
	private:
		rCore_ Core_;
		sTRow Root_;
		sTRow UpdateHubs_(
			sTRow ParentRow,
			sTRow NewRow)	// Target hub must be orphan.
			{
				sHub New, Parent;

				tol::Init(New, Parent);

				Core_.Hubs.Recall(ParentRow, Parent);

				if ( Parent.First == qNIL ) {
					if ( Parent.Last != qNIL )
						qRFwk();

					Parent.First = Parent.Last = NewRow;
				} else if ( Parent.Last == qNIL ) {
					qRFwk();
				} else {
					sHub Sibling;

					Sibling.Init();

					Core_.Hubs.Recall(Parent.Last, Sibling);

					if ( Sibling.Next != qNIL)
						qRFwk();

					Sibling.Next = NewRow;

					Core_.Hubs.Store(Sibling, Parent.Last);

					New.Prev = Parent.Last;

					Parent.Last = NewRow;
				}

				New.Parent = ParentRow;

				Core_.Hubs.Store(New, NewRow);
				Core_.Hubs.Store(Parent, ParentRow);

				return NewRow;
			}
			sHub GetHub_(sTRow Row) const
			{
				sHub Hub;

				Hub.Init();

				Core_.Hubs.Recall(Row, Hub);

				return Hub;
			}
			sTRow GetParent_(sTRow Row) const
			{
				return GetHub_(Row).Parent;
			}
			sTRow GetPrev_(sTRow Row) const
			{
				return GetHub_(Row).Prev;
			}
			sTRow GetNext_(sTRow Row) const
			{
				return GetHub_(Row).Next;
			}
			sTRow GetFirst_(sTRow Row) const
			{
				return GetHub_(Row).First;
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
      Core_.reset(P);
			Root_ = qNIL;
		}
		qCDTOR(rTasks);
    void Init(void)
		{
			reset();

			if ( !Core_.Init() ) {
        if ( Core_.Hubs.Add(sHub()) != Core_.Tasks.Add(sTask()) )
          qRGnr();
			}

		  if ( Core_.Hubs.Amount() == 0 )
        qRFwk();

      if ( Core_.Hubs.Amount() != Core_.Tasks.Amount() )
        qRFwk();

      Root_ = Core_.Hubs.First();
		}
		sTRow Append(
			const str::dString &Label,
			sTRow Row)
			{
				sTRow NewRow = qNIL;
				sTask Task;

				if (Row == qNIL)
					Row = Root_;

				Task.Label = Core_.Contents.New();
				Core_.Contents(Task.Label).Init(Label);

				NewRow = Core_.Tasks.New();
				Core_.Tasks.Store(Task, NewRow);

				if ( NewRow != Core_.Hubs.New() )
					qRFwk();

				return UpdateHubs_(Row, NewRow);
			}
		void DumpChildren(
			sTRow Row,
			xml::rWriter &Writer) const;
	};
}


#endif
