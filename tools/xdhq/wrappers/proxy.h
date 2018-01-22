/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROXY_INC_
# define PROXY_INC_

# include "prtcl.h"

# include "prxy_send.h"
# include "prxy_recv.h"

# include "flw.h"
# include "str.h"

namespace proxy {
	void Handshake(
		flw::sRFlow &Flow,
		str::dString &Language );

	void GetAction(
		flw::sRWFlow &Flow,
		str::dString &Id,
		str::dString &Action );

	using prxy_cmn::eRequest;
	using prxy_cmn::r_Undefined;

	using prxy_send::rArguments;
	using prxy_send::Send;

	using prxy_recv::rReturn;
	using prxy_recv::Recv;
}

#endif