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

#ifndef SERVER_INC_
# define SERVER_INC_

# include "flw.h"

namespace server {
	void Handshake(
		flw::sRFlow &Flow,
		str::dString &Language );

	void GetAction(
		flw::sRWFlow &Flow,
		str::dString &Id,
		str::dString &Action );

#define SERVER_SETP_ 	const str::dString &, const str::dString &, const str::dString &, const char *, flw::sWFlow &Flow

	typedef void(* fSet)( SERVER_SETP_ );

	void SetLayout( SERVER_SETP_ );
	void SetCasting( SERVER_SETP_ );

	void GetContent(
		const str::dString &Id,
		flw::sRWFlow &Flow,
		str::dString &Content );

	void SetContent(
		const str::dString &Id,
		const str::dString &Content,
		flw::sWFlow &Flow );
}

#endif