/*
	'login.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef LOGIN_INC_
# define LOGIN_INC_

# include "base.h"

namespace login {
	BASE_AD( SwitchBackendType );
	BASE_AD( DisplayEmbeddedBackendFilename );
	BASE_AD( Connect );
	BASE_AD( Dismiss );

	inline void Register( void ) {
		BASE_AR( SwitchBackendType );
		BASE_AR( DisplayEmbeddedBackendFilename );
		BASE_AR( Connect );
		BASE_AR( Dismiss );
	}

	void SetLayout( core::rSession &Session );

	void Display( core::rSession &Session );
}

#endif
