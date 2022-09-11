/*
	'login.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef LOGIN_INC_
# define LOGIN_INC_

# include "base.h"

namespace login {
	BASE_ACD( SwitchBackendType );
	BASE_ACD( DisplayEmbeddedBackendFilename );
	BASE_ACD( Connect );
	BASE_ACD( Dismiss );

	inline void Register( void ) {
		BASE_ACR( SwitchBackendType );
		BASE_ACR( DisplayEmbeddedBackendFilename );
		BASE_ACR( Connect );
		BASE_ACR( Dismiss );
	}
	void SetLayout( core::rSession &Session );
}

#endif