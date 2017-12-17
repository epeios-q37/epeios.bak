/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

// XDH DoWnStream.
// Stuff related to the JS part.

#ifndef XDH_DWS_INC_
# define XDH_DWS_INC_

# include"sclnjs.h"

namespace xdh_dws {
	struct rJS
	{
	public:
		sclnjs::rCallback Callback;
		sclnjs::wArguments Arguments;
		str::wString	// For the launching of an action.
			Id,
			Action;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Callback, Arguments, Id, Action );
			Arguments.reset( P );
		}
		qCDTOR( rJS );
		void Init( void )
		{
			tol::Init( Callback, Arguments, Id, Action );
		}
	};
}

#endif
