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

bso::sBool tsktasks::rCore_::Init(void)
{
  bso::sBool Exists = false;
qRH;
  str::wString Repo;
qRB;
  Repo.Init();

  sclm::MGetValue(tskrgstry::parameter::Repository, Repo);

  AggregatedStorage_.plug(Hooks_);
  tol::plug(&AggregatedStorage_, Contents, Tasks, Hubs);

  if ( Hooks_.Init(Repo, uys::mReadWrite) == uys::sExists ) {
    Hooks_.Get((sdr::sByte *)&S_);
    Exists = true;
  } else {
    tol::Init(Contents, Tasks, Hubs);
    Exists = false;
  }
qRR;
qRT;
qRE;

  return Exists;
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

	Row = GetFirst_(Row);
	Level++;

	while ( Row != qNIL ) {
    tol::Init(Label, Description);
	  Browser.Task(Kinship, Level, Row, GetLabel_(Row, Label), GetDescription_(Row, Description));

		if ( TestAndGetFirst_(Row)) {
      Level++;
      Kinship = kChild;
		} else if ( TestAndGetNext_(Row) ) {
      Kinship = kSibling;
		} else {
		  bso::sBool Cont = true;

		  while ( Cont ) {
        if ( TestAndGetParent_(Row) ) {
          Level--;
          Browser.Parent(Level);
          Cont = ( Level != 0 ) && !TestAndGetNext_(Row);
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
