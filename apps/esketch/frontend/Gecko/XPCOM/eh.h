/*
	'eh.h' by Claude SIMON (http://q37.info/contact/).

	This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EH_INC_
# define EH_INC_

# include "xulfeh.h"

// Predeclaration.
namespace trunk {
	class trunk___;
}

namespace eh {
	typedef xulfeh::event_handlers__ _event_handlers__;

	class event_handlers__
	: public _event_handlers__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handlers__::reset( P );
		}
		E_CDTOR( event_handlers__ );
		void Init( trunk::trunk___ &Trunk );
	};
}

#endif