/*
	'main.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef MAIN__INC
# define MAIN__INC

# include "base.h"

namespace main {
	BASE_ACD( submission );
	BASE_ACD( show_test_button );
	BASE_ACD( hide_test_button );
	BASE_ACD( testing );

	class action_callbacks__
	{
	public:
		BASE_ACU( submission, Submission );
		BASE_ACU( show_test_button, ShowTestButton );
		BASE_ACU( hide_test_button, HideTestButton );
		BASE_ACU( testing, Testing );
	public:
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( Submission );
			BASE_ACR( ShowTestButton );
			BASE_ACR( HideTestButton );
			BASE_ACR( Testing );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( Submission );
			BASE_ACI( ShowTestButton );
			BASE_ACI( HideTestButton );
			BASE_ACI( Testing );
		}
	};

	void SetLayout( core::session___ &Session );
}

#endif