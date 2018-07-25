/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XDWMAIN_INC_
# define XDWMAIN_INC_

# include "query.h"
# include "session.h"

namespace xdwmain {
	using query::dPairs;
	using query::wPairs;

	using session::rAgent;
	using session::dSessions;
	using session::wSessions;
	using session::rSessions;

	void Handle(
		const dPairs &Pairs,
		rSessions &Sessions,
		str::dString &Response );
}

#endif
