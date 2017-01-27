/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// MUA ACCount IM(AP).

#ifndef MUAACCIM_INC_
# define MUAACCIM_INC_

# ifdef XXX_DBG
# define MUAACCIM_DBG
# endif

# include "muabsc.h"
# include "muaagt.h"
# include "muadir.h"
# include "muatrk.h"

namespace muaaccim {
	void Update(
		muaima::rSession &Session,
		muatrk::dTracker &Tracker,
		muadir::dDirectory &Directory );

	void GetFields(
		const muamel::dRows &Wanted,
		const muamel::dIds &Ids,
		muaagt::sRow Agent,
		muaima::rSession &Session,
		str::dStrings &Subjects,
		muaagt::dRows &CorrespondingAgents,
		muamel::dRows &Available );

	const str::dString &GetMail(
		const muamel::dId &Id,
		muaima::rSession &Session,
		str::dString &Mail );
}


#endif
