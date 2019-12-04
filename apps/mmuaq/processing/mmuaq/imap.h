/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq.

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

#ifndef IMAP_INC_
# define IMAP_INC_

namespace imap {
	// Low-level functions.
	void Capability( void );
	void List( void );
	void LSub( void );
	void Select( void );
	void Fetch( void );

	// High-level functions.
	void Folders( void );
	void RFC822( void );
	void RFC822Size( void );
	void RFC822Header( void );
	void RFC822Text( void );
	void UID( void );
	void MailAmount( void );

	void Test( void );
}

#endif
