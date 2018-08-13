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

#include "prxy_send.h"

using namespace prxy_send;

using namespace prxy_cmn;

namespace {
	namespace {
		template <typename items> void Send_(
			const items &Items,
			flw::sWFlow &Flow )
		{
//			prtcl::Put( Items.Amount(), Flow );

			sdr::sRow Row = Items.First();

			if ( Row != qNIL ) {
				while ( Row != qNIL ) {
					prtcl::Put( Items( Row ), Flow );

					Row = Items.Next( Row );
				}
			}
		}
	}
}


void prxy_send::Send(
	flw::sWFlow &Flow,
	const rArguments &Arguments )
{
	prtcl::SendCommand( prtcl::GetCommand( Arguments.Command ), Flow );
	Send_( Arguments.Strings, Flow );
	Send_( Arguments.XStrings, Flow );
}
