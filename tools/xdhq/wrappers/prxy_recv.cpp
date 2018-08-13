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

#include "prxy_recv.h"

void prxy_recv::Recv(
	prxy_recv::eType ReturnType,
	flw::sRFlow &Flow,
	rReturn &Return )
{
	if ( prtcl::GetRequest( Flow ) != prtcl::rReady_1 )
		qRGnr();

	switch ( ReturnType ) {
	case tVoid:
		break;
	case tString:
		prtcl::Get( Flow, Return.StringToSet() );
		break;
	case tStrings:
		prtcl::Get( Flow, Return.StringsToSet() );
		break;
	default:
		qRGnr();
		break;
	}
}


