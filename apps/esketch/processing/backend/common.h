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

	qENUM( Message )
	{
		mTestMessage,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( eMessage Message );

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
# define REPORT( message, ... ) sclmisc::ReportAndAbort( common::GetLabel( common::m##message ), ##__VA_ARGS__  )

# define BACKEND_ ( *(sclbacknd::rBackend *)BaseBackend.UP() )
# define STUFF_ ( *(common::rStuff *)BACKEND_.Stuff() )

// 'h' suffix : to put in error header  (between 'qRH' and 'qRB').
// 'b' suffix : to put in error body  (just after and 'qRB').

#define BACKENDb sclbacknd::rBackend &Backend = BACKEND_

#define STUFFb common::rStuff &Stuff = STUFF_

#endif
