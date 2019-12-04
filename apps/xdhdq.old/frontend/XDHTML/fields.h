/*
	'fields.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef FIELDS_INC_
# define FIELDS_INC_

# include "base.h"

namespace fields {
	BASE_ACD( edit );
	BASE_ACD( refresh );


	class action_callbacks__
	{
	public:
		BASE_ACU( edit, Edit );
		BASE_ACU( refresh, Refresh );
	public:
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( Edit );
			BASE_ACR( Refresh );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( Edit );
			BASE_ACI( Refresh );
		}
	};

	void SetLayout(
		const char *Id,
		core::session___ &Session );
}

#endif