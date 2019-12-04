/*
	'login.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef LOGIN_INC_
# define LOGIN_INC_

# include "base.h"

namespace login {
	BASE_ACD( switch_backend_type );
	BASE_ACD( display_embedded_backend_filename );
	BASE_ACD( connect );
	BASE_ACD( dismiss );

	class action_callbacks__
	{
	public:
		BASE_ACU( switch_backend_type, SwitchBackendType );
		BASE_ACU( display_embedded_backend_filename, DisplayEmbeddedBackendFilename );
		BASE_ACU( connect, Connect );
		BASE_ACU( dismiss, Dismiss );
	public:
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( SwitchBackendType );
			BASE_ACR( DisplayEmbeddedBackendFilename );
			BASE_ACR( Connect );
			BASE_ACR( Dismiss );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( SwitchBackendType );
			BASE_ACI( DisplayEmbeddedBackendFilename );
			BASE_ACI( Connect );
			BASE_ACI( Dismiss );
		}
	};

	void SetLayout( core::session___ &Session );
}

#endif