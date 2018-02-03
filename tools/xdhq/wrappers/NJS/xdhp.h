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

// XDH Primitives.

#ifndef XDHP_INC_
# define XDHP_INC_

# include"sclnjs.h"

namespace xdhp {
	SCLNJS_F( Register );
	SCLNJS_F( Listen );
	SCLNJS_F( Execute );
	SCLNJS_F( Alert );
	SCLNJS_F( Confirm );
	SCLNJS_F( SetLayout );
	SCLNJS_F( GetContents );
	SCLNJS_F( SetContents );
	SCLNJS_F( DressWidgets );
	SCLNJS_F( AddClasses );
	SCLNJS_F( RemoveClasses );
	SCLNJS_F( ToggleClasses );
	SCLNJS_F( EnableElements );
	SCLNJS_F( DisableElements );
	SCLNJS_F( SetAttribute );
	SCLNJS_F( GetAttribute );
	SCLNJS_F( RemoveAttribute );
	SCLNJS_F( SetProperty );
	SCLNJS_F( GetProperty );
	SCLNJS_F( Focus );
}

#endif
