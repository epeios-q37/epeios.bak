/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

// MUA eXaMPle

#ifndef MUAXMP__INC
# define MUAXMP__INC

# ifdef XXX_DBG
#	define MUAXMP__DBG
# endif

# include "muabsc.h"

namespace muaxmp {
	class dMyObject
	{
	private:
	public:
		struct s {
		};
		dMyObject( s &S )
		{}
		void reset( bso::bool__ P = true )
		{
		}
		void plug( qASd *AS )
		{
		}
		dMyObject &operator =( const dMyObject &M )
		{
			return *this;
		}
		bso::sBool Init( void )
		{
			return true;
		}
	};
}


#endif
