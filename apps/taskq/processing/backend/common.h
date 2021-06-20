/*
	Copyright (C) 2021 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'TaskQ' software.

    'TaskQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'TaskQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'TaskQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON_INC_
# define COMMON_INC_

# include "tskbsc.h"

# include "fblbkd.h"

#include "sclb.h"

namespace common {

	void Initialize( void );

	bso::bool__ IsInitialized( void );

	class rStuff	// Contains data peculiar to a backend, each (virtual) connection having its own backend.
	{
	public:
		void reset( bso::bool__ P  = true )
		{
		}
		E_CVDTOR( rStuff );
		void Init( void )
		{
		}
	};
}

// '##' needed by 'clang++'/'g++'.
# define REPORT( message, ... ) sclm::ReportAndAbort( message##_, ##__VA_ARGS__  )

# define BACKEND_ ( *(sclbacknd::rBackend *)BaseBackend.UP() )
# define STUFF_ ( *(common::rStuff *)BACKEND_.Stuff() )

// 'h' suffix : to put in error header  (between 'qRH' and 'qRB').
// 'b' suffix : to put in error body  (just after and 'qRB').

#define BACKENDb sclb::rBackend &Backend = BACKEND_

#define STUFFb common::rStuff &Stuff = STUFF_

#endif
