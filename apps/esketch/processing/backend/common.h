/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef COMMON__INC
# define COMMON__INC

# include "sktbsc.h"
# include "sktinf.h"

# include "fblbkd.h"

#include "sclbacknd.h"

namespace common {
	typedef sclbacknd::rBackend rBackend;	// // ('eXtended Backend').

	class rStuff	// Contains data peculiar to a backend, each (virtual) connection having its own backend.
	{
	private:
		rBackend *Backend_;
	public:
		void reset( bso::bool__ = true )
		{
			Backend_ = NULL;
		}
		E_CVDTOR( rStuff );
		void Init( rBackend &Backend )
		{
			Backend_ = &Backend;
		}
		rBackend &Backend( void ) const
		{
			if ( Backend_ == NULL )
				qRGnr();

			return *Backend_;
		}
	};
}

#endif
