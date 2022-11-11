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


#include "tasks.h"

using namespace tasks;

wBundle tasks::Bundle;

namespace {
  namespace _ {
    sTRow Add(
      const str::dString &Title,
      const str::dString &Description,
      sTRow Row)
    {
      return Bundle.Add(Title, Description, Row);
    }
    sTRow Add(
      const char *Title,
      const char *Description,
      sTRow Row)
    {
      return Add(str::wString(Title), str::wString(Description), Row);
    }
  }

  void Populate_(void) {
    sTRow Row = qNIL;

    _::Add("T1", "D1", Row);
    _::Add("T2", "D2", Row);
    _::Add("T3", "D3", Row);

    Row = Bundle.First(Bundle.Next());

    _::Add("T2.1", "D2.1", Row);
    _::Add("T2.2", "D2.2", Row);

  }
}

qGCTOR(Taslks)
{
  Bundle.Init();
  Populate_();
}
