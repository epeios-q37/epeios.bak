/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XPPq.

    XPPq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XPPq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XPPq.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON_INC_
# define COMMON_INC_

# include "flx.h"

namespace common {
	struct sRelay {
	private:
		flx::rDRelay<> Core_;	// 'node.js' functions send up to 64K of data in one chunk.
	public:
		flx::sIRelay In;
		flx::sORelay Out;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, In, Out );
		}
		qCDTOR( sRelay );
		void Init( void )
		{
			Core_.Init();
			In.Init( Core_ );
			Out.Init( Core_ );
		}
	};
}

#endif
