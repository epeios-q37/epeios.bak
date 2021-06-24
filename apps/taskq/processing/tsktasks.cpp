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

using namespace tsktasks;


void tsktasks::rTasks::DumpChildren(
  sTRow Row,
  xml::rWriter &Writer) const
{
qRH;
  str::wString Value;
qRB;
	if ( Row == qNIL )
		Row = Root_;

	Writer.PushTag("Tasks");
	Writer.PutAttribute("row", *Row);
	Writer.PutAttribute("Amount", Core_.Tasks.Amount() - 1);

	Row = GetFirst_(Row);

	while ( Row != qNIL ) {
		Writer.PushTag("Task");
    Writer.PutAttribute("row", *Row);

		Value.Init();
		GetLabel_(Row, Value);
		Writer.PutValue(Value, "Label");

		Value.Init();
		GetDescription_(Row, Value);
		Writer.PutValue(Value, "Description");

		Writer.PopTag();

		Row = GetNext_(Row);
	}

	Writer.PopTag();
qRR;
qRT;
qRE;
}
