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

// PRoToCoL
// Unlike other modules in the same directory, this one is shared between client and server.
// Other modules are only for the server part of the software.

#ifndef PRTCL_INC_
# define PRTCL_INC_

# include "csdcmn.h"
# include "flw.h"
# include "tol.h"


namespace prtcl {
	static qCDEF( char *, ProtocolId, "3f0aef6b-b893-4ccd-9316-d468588fc572" );
	qCDEF( bso::sU8, ProtocolVersion, 0 );

	static qCDEF( char *, StandBy, "StandBy_1" );	// Send as command to report that there is no more command to handle.

	using csdcmn::Put;
	using csdcmn::Get;
}

#endif