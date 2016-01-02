/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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
	typedef sclbacknd::backend___ xbackend___;	// // 'xbackend' ('eXtended' 'backend').

	class stuff___	// Contient les donnes propres  chaque 'backend', chaque connection (virtuelle) ayant son propre backend.
	{
	private:
		xbackend___ *_XBackend;
	public:
		void reset( bso::bool__ = true )
		{
			_XBackend = NULL;
		}
		E_CVDTOR( stuff___ );
		void Init( xbackend___ &XBackend )
		{
			_XBackend = &XBackend;
		}
		xbackend___ &XBackend( void ) const
		{
			if ( _XBackend == NULL )
				qRGnr();

			return *_XBackend;
		}
	};
}

#endif
