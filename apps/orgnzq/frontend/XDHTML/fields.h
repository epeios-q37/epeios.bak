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

#ifndef FIELDS_INC_
# define FIELDS_INC_

# include "base.h"

namespace fields {

	BASE_ACD( CreateField );
	BASE_ACD( DefineField );
	BASE_ACD( DragField );
	BASE_ACD( DropField );
	BASE_ACD( EndFieldDragging );


	inline void Register( void ) {
		BASE_ACR( CreateField );
		BASE_ACR( DefineField );
		BASE_ACR( DragField );
		BASE_ACR( DropField );
		BASE_ACR( EndFieldDragging );
	}

	void SetLayout(
		const char * Id,
		core::rSession & Session );

	void SetCasting(
		const char *Id,
		core::rSession &Session );

	void Display(
		const char *Id,
		core::rSession &Session );

	void SetFieldLayout( core::rSession &Session );
	void SetFieldCasting( core::rSession &Session );
}

#endif