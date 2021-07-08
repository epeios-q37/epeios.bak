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

#ifndef PROLOG_INC_
# define PROLOG_INC_

# include "base.h"

namespace prolog {
	BASE_AD( SwitchPreset );
	BASE_AD( DisplayProjectFilename );
	BASE_AD( LoadPreset );

	inline void Register( void )
	{
		BASE_AR( SwitchPreset );
		BASE_AR( DisplayProjectFilename );
		BASE_AR( LoadPreset );
	};

	void SetLayout( core::rSession &Session );

	void Display( core::rSession &Session);
}

#endif
