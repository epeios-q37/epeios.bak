/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// Field
#ifndef FIELD_INC_
# define FIELD_INC_

# include "base.h"

namespace field {
	BASE_ACD( DefineNewEntry );
	BASE_ACD( DefineEntry );
	BASE_ACD( DragEntry );
	BASE_ACD( DropEntry );
	BASE_ACD( EndEntryDragging );

	inline void Register( void )
	{
		BASE_ACR( DefineNewEntry );
		BASE_ACR( DefineEntry );
		BASE_ACR( DragEntry );
		BASE_ACR( DropEntry );
		BASE_ACR( EndEntryDragging );
	};

	void SetLayout(
		const char * Id,
		core::rSession & Session );

	void SetCasting(
		const char *Id,
		core::rSession &Session );

	void Display(
		const char *Id,
		core::rSession &Session );
}

#endif