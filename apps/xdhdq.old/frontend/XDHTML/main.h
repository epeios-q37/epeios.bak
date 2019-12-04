/*
	'main.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef MAIN_INC_
# define MAIN_INC_

# include "base.h"

namespace main {
	BASE_ACD( show_facetious_button );
	BASE_ACD( hide_facetious_button );
	BASE_ACD( surrender );

	class action_callbacks__
	{
	public:
		BASE_ACU( show_facetious_button, ShowFacetiousButton );
		BASE_ACU( hide_facetious_button, HideFacetiousButton );
		BASE_ACU( surrender, Surrender );
	public:
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( ShowFacetiousButton );
			BASE_ACR( HideFacetiousButton );
			BASE_ACR( Surrender );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( ShowFacetiousButton );
			BASE_ACI( HideFacetiousButton );
			BASE_ACI( Surrender );
		}
	};

	void SetLayout( core::session___ &Session );

	void RefreshFields( core::session___ &Session );
}

#endif