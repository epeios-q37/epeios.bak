/*
	'global.h' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'xdhdq' software.

    'xdhdq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhdq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhdq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLOBAL_INC_
# define GLOBAL_INC_

# include "base.h"

namespace global {
	BASE_ACD( about );
	BASE_ACD( refresh );

	class action_callbacks__
	{
	public:
		BASE_ACU( about, About );
		BASE_ACU( refresh, Refresh );
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( About );
			BASE_ACR( Refresh );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( About );
			BASE_ACI( Refresh );
		}
	};
}

#endif