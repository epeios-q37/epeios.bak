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

#ifndef TSKXMP_INC_
# define TSKXMP_INC_

# ifdef XXX_DBG
# define TSKXMP_DBG_
# endif

# include "tskbsc.h"

# include "lstbch.h"
# include "lstcrt.h"
# include "str.h"
# include "xml.h"

namespace tsktasks {
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
		void  Init(void)
		{
			reset();
		}
	};

	class rTasks {
	private:
		lstcrt::qLCRATEw(str::dString, sCRow) Contents_;
		lstbch::qLBUNCHw(sTask, sTRow) Tasks_;
		lstbch::qLBUNCHw(sHub, sTRow) Hubs_;
		sTRow Root_;
		sTRow UpdateHubs_(
			sTRow ParentRow,
			sTRow NewRow)	// Target hub must be orphan.
			{
				sHub New, Parent;

				tol::Init(New, Parent);

				Hubs_.Recall(ParentRow, Parent);

				if ( Parent.First == qNIL ) {
					if ( Parent.Last != qNIL )
						qRFwk();

					Parent.First = Parent.Last = NewRow;
				} else if ( Parent.Last == qNIL ) {
					qRFwk();
				} else {
					sHub Sibling;

					Sibling.Init();

					Hubs_.Recall(Parent.Last, Sibling);

					if ( Sibling.Next != qNIL)
						qRFwk();

					Sibling.Next = NewRow;

					Hubs_.Store(Sibling, Parent.Last);

					New.Prev = Parent.Last;

					Parent.Last = NewRow;
				}

				New.Parent = ParentRow;

				Hubs_.Store(New, NewRow);
				Hubs_.Store(Parent, ParentRow);

				return NewRow;
			}
			sHub GetHub_(sTRow Row) const
			{
				sHub Hub;

				Hub.Init();

				Hubs_.Recall(Row, Hub);

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

				Tasks_.Recall(Row, Task);

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
			const str::dString &GetLabel(
				sTRow Row,
				str::dString &Value)
			{
			}
	public:
		void reset(bso::sBool P = true)
		{
			tol::reset(P, Contents_, Tasks_, Hubs_);

			Root_ = qNIL;
		}
		void Init(void)
		{
			reset();

			tol::Init(Contents_, Tasks_, Hubs_);

			Root_ = Hubs_.Add(sHub());
		}
		sTRow Append(
			const str::dString &Label,
			sTRow Row)
			{
				sTRow NewRow = qNIL;
				sTask Task;

				if (Row == qNIL)
					Row = Root_;

				Task.Label = Contents_.New();
				Contents_(Task.Label).Init(Label);

				NewRow = Tasks_.New();
				Tasks_.Store(Task, NewRow);

				if ( NewRow != Hubs_.New() )
					qRFwk();

				return UpdateHubs_(Row, NewRow);
			}
		void DumpChildren(
			sTRow Row,
			xml::rWriter &Writer) const;
	};

	class dMyObject
	{
	private:
	public:
		struct s {
		};
		dMyObject( s &S )
		{}
		void reset( bso::bool__ P = true )
		{
		}
		void plug( qASd *AS )
		{
		}
		dMyObject &operator =( const dMyObject &M )
		{
			return *this;
		}
		bso::sBool Init( void )
		{
			return true;
		}
	};
}


#endif
