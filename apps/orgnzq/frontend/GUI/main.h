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

#ifndef MAIN_INC_
# define MAIN_INC_

# include "base.h"

# include "record.h"

namespace main {
	BASE_ACD( NewPanel );
	BASE_ACD( SelectPanel );
	BASE_ACD( DragPanel );
	BASE_ACD( DropPanel );
	BASE_ACD( EndPanelDragging );
	BASE_ACD( Template );

	inline void Register( void ) {
		BASE_ACR( NewPanel );
		BASE_ACR( SelectPanel );
		BASE_ACR( DragPanel );
		BASE_ACR( DropPanel );
		BASE_ACR( EndPanelDragging );
		BASE_ACR( Template );
	}

	void SetLayout( core::rSession & Session );

	void SetCasting( core::rSession & Session );

	void Display(
		core::rSession &Session,
		bso::sBool Refresh = false );
	void SetRecordsLayout( core::rSession &Session );
	void SetRecordLayout( core::rSession &Session );
}

#endif