/*
	'common.h' by Claude SIMON (http://zeusw.org/).

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

// $Id: example.h,v 1.5 2009/10/04 16:59:50 csimon Exp $

// BaCKend MaNGER

#ifndef COMMON__INC
# define COMMON__INC

# include "sktbsc.h"
# include "sktinf.h"

# include "fblbkd.h"

namespace common {
	class data___
	{
	public:
		void reset( bso::bool__ = true )
		{
		}
		E_CVDTOR( data___ );
		void Init( void )
		{
		}
	};
}


#endif
