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

#ifndef MAIN_INC_
# define MAIN_INC_

# include "base.h"

namespace main {
	BASE_ACD( Check );
	BASE_ACD( Configuration );

	inline void Register( void  )
	{
		BASE_ACR( Check );
		BASE_ACR( Configuration );
	};

	void SetLayout( core::rSession &Session );

	void SetFoldersLayout( core::rSession &Session );
	void SetFoldersCasting( core::rSession &Session );

	void SetMailsLayout( core::rSession &Session );
	void SetMailsCasting( core::rSession &Session );
}

#endif