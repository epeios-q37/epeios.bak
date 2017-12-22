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

// TREE Primitives.

#ifndef TREEP_INC_
# define TREEP_INC_

# include "sclnjs.h"

namespace treep {
	SCLNJS_F( New );
	SCLNJS_F( Delete );
	SCLNJS_F( PushTag );
	SCLNJS_F( PopTag );
	SCLNJS_F( PutValue );
	SCLNJS_F( PutAttribute );

	const str::dString &GetXML(
		sclnjs::sCaller &Caller,
		str::dString &XML );
}

#endif
