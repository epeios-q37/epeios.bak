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

#include "tsktasks.h"

#include "tskinf.h"
#include "tskrgstry.h"

#include "sclm.h"

using namespace tsktasks;

void tsktasks::rTasks::Init(void)
{
  Root_ = qNIL;

  Repository_.Init();
  sclm::MGetValue(tskrgstry::parameter::Repository, Repository_);

  if ( !Core_.Init(Repository_) ) {
    if ( Core_.Hubs.Add(sHub()) != Core_.Tasks.Add(sTask()) )
      qRGnr();
  }

  if ( Core_.Hubs.Amount() == 0 )
    qRFwk();

  if ( Core_.Hubs.Amount() != Core_.Tasks.Amount() )
    qRFwk();

  Root_ = Core_.Hubs.First();
}

sTRow tsktasks::rTasks::Append(
 const rTask &TaskBuffer,
 sTRow Row)
{
qRH;
  sTask Task;
qRB;
  Task.Init();

  if ( TaskBuffer.Label.Amount() == 0 )
    qRGnr();

  Task.Label = Core_.Add(TaskBuffer.Label);

  if ( TaskBuffer.Description.Amount() != 0 ) {
    Task.Description = Core_.Add(TaskBuffer.Description);
  }

  Row = Core_.Append(Task, Row);
qRR;
  if ( Task.Label != qNIL )
    Core_.Remove(Task.Label);

  if ( Task.Description != qNIL )
    Core_.Remove(Task.Description);
qRT;
qRE;
  return Row;
}

void tsktasks::rTasks::Browse(
  sTRow Row,
  cBrowser &Browser) const
{
qRH;
  str::wString Label, Description;
  sLevel Level = 0;
  eKinship Kinship = k_Undefined;
qRB;
	if ( ( Row == qNIL ) || ( Row == Root_ ) ) {
		Row = Root_;
    Browser.Tasks(qNIL);
	} else {
    tol::Init(Label, Description);
    Browser.Tasks(Row);
	}

  Kinship = kFirst;

	Row = Core_.GetFirst(Row);
	Level++;

	while ( Row != qNIL ) {
    tol::Init(Label, Description);
	  Browser.Task(Kinship, Level, Row, Core_.GetLabel(Row, Label), Core_.GetDescription(Row, Description));

		if ( Core_.TestAndGetFirst(Row)) {
      Level++;
      Kinship = kChild;
		} else if ( Core_.TestAndGetNext(Row) ) {
      Kinship = kSibling;
		} else {
		  bso::sBool Cont = true;

		  while ( Cont ) {
        if ( Core_.TestAndGetParent(Row) ) {
          Level--;
          Browser.Parent(Level);
          Cont = ( Level != 0 ) && !Core_.TestAndGetNext(Row);
        } else
          Cont = false;
		  }

		  if ( Level == 0 )
        Row = qNIL;
      else {
        Kinship = kSibling;
      }
    }
	}
qRR;
qRT;
qRE;
}
