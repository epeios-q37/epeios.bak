/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'prxyq.

    'prxyq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'prxyq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'prxyq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WATCHDOG__INC
# define WATCHDOG__INC

namespace watchdog {
	void Ping( void );
	void Freeze( void );
	void Crash( void );
}

#endif
