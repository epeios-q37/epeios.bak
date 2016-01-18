/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// OrGaniZer eXaMPle

#ifndef OGZXMP__INC
# define OGZXMP__INC

# ifdef XXX_DBG
#	define OGZXMP__DBG
# endif

# include "ogzbsc.h"
# include "ags.h"

namespace ogzxmp {

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
		void plug( qAS_ &AS )
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
