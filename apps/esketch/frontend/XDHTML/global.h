/*
	'global.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLOBAL__INC
# define GLOBAL__INC

# include "base.h"

namespace global {
	BASE_ACD( about );
	BASE_ACD( refresh );
	BASE_ACD( test );

	class action_callbacks__
	{
	public:
		BASE_ACU( about, About );
		BASE_ACU( refresh, Refresh );
		BASE_ACU( test, Test );
		void reset( bso::bool__ P = true )
		{
			BASE_ACR( About );
			BASE_ACR( Refresh );
			BASE_ACR( Test );
		}
		E_CVDTOR( action_callbacks__ );
		void Init( void )
		{
			BASE_ACI( About );
			BASE_ACI( Refresh );
			BASE_ACI( Test );
		}
	};
}

#endif