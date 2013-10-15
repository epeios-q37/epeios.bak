/*
	'sktxmp.h' by Claude SIMON (http://zeusw.org/).

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

// $Id: mbdmng.h,v 1.23 2009/10/07 10:12:50 csimon Exp $

// eSKeTch eXaMPle

#ifndef SKTXMP__INC
#define SKTXMP__INC

# ifdef XXX_DBG
#	define SKTXMP__DBG
# endif

# include "sktbsc.h"

namespace sktxmp {

	class myobject_
	{
	private:
	public:
		struct s {
		};
		myobject_( s &S )
		{}
		void reset( bso::bool__ P = true )
		{
		}
		void plug( ags::E_ASTORAGE_ &AS )
		{
		}
		myobject_ &operator =( const myobject_ &M )
		{
			return *this;
		}
		bso::bool__ Init( void )
		{
			return true;
		}
	};
}


#endif
