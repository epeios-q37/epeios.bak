/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "prtcl.h"

#include "stsfsm.h"

using namespace prtcl;

# if 0
namespace {
	inline void Put_(
		const char *Identifier,
		flw::oflow__ &Flow )
	{
		Flow.Write( Identifier, strlen( Identifier ) + 1 );	// '+1' to put the final 0.
	};

	template <typename t> void Put_(
		t T,
		flw::oflow__ &Flow,
		const char *( *Get )( t T ) )
	{
		Put_( Get( T ), Flow );
	}
}
# endif