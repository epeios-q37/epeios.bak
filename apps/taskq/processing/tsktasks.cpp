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

using namespace tsktasks;

void tsktasks::rTasks::Export(
  sTRow Row,
  xml::rWriter &Writer,
  const char *Generator) const
{
qRH;
  str::wString Value;
  tol::bDateAndTime Buffer;
  bso::sU16 Level = 0;
qRB;

  Writer.PushTag(TSKINF_MC);
  Writer.PutAttribute("Version", TSKINF_SOFTWARE_VERSION);
  Writer.PutAttribute("Timestamp", tol::DateAndTime(Buffer));
  Writer.PutAttribute("Generator", Generator);

	if ( ( Row == qNIL ) || ( Row == Root_ ) ) {
		Row = Root_;
		Writer.PushTag("RootTask");
    Writer.PutAttribute("Amount", Core_.Tasks.Amount() - 1);
	} else {
    Writer.PushTag("Task");
    Value.Init();
    Writer.PutAttribute("Label", GetLabel_(Row, Value));
	}

	Writer.PutAttribute("row", *Row);

	Row = GetFirst_(Row);
	Level++;

	while ( Row != qNIL ) {
		Writer.PushTag("Task");
    Writer.PutAttribute("row", *Row);

    Value.Init();
    Writer.PutAttribute("Label", GetLabel_(Row, Value));

		Value.Init();
		GetDescription_(Row, Value);
		if ( Value.Amount() )
      Writer.PutValue(Value, "Description");

		if ( TestAndGetFirst_(Row)) {
      Writer.PushTag("SubTasks");
      Level++;
		} else if ( !TestAndGetNext_(Row) ) {
      Writer.PopTag();
      Level--;

		  while ( TestAndGetParent_(Row) && ( Level != 0 ) && !TestAndGetNext_(Row) ) {
        Writer.PopTag();
        Level--;
		  }

		  if ( Level == 0 )
        Row = qNIL;
      else
        Writer.PopTag();
		} else
      Writer.PopTag();
	}

	Writer.PopTag();
	Writer.PopTag();
qRR;
qRT;
qRE;
}

void tsktasks::rTasks::Display(
  sTRow Row,
  txf::sWFlow &Out) const
{
qRH;
  str::wString Value;
  const char *Indent = "";
qRB;

	if ( ( Row == qNIL ) || ( Row == Root_ ) ) {
		Row = Root_;
	} else {
    Value.Init();
    Out << *Row << ": " << GetLabel_(Row, Value) << txf::nl;
    Indent = "  ";
	}

	Row = GetFirst_(Row);

	while ( Row != qNIL ) {
    Value.Init();
    Out << Indent << *Row << ": " << GetLabel_(Row, Value) << txf::nl;

    Row = GetNext_(Row);
	}

qRR;
qRT;
qRE;
}
