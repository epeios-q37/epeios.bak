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

// TaSK XML

#ifndef TSKXML_INC_
# define TSKXML_INC_

# ifdef XXX_DBG
# define TSKXMP_DBG_
# endif

# include "tskbsc.h"
# include "tsktasks.h"

namespace tskxml {
  void Export(
    const tsktasks::rTasks &Tasks,
    tsktasks::sTRow Row,
    txf::sWFlow &Flow,
    const char *Generator);

  void Import(
    xml::rParser &Parser,
    tsktasks::sTRow Row,
    tsktasks::rTasks &Tasks);
}


#endif
