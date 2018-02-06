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

// XDH(TML) related Primitives.

#ifndef XDHP_INC_
# define XDHP_INC_

# include "sclznd.h"

namespace xdhp {
	SCLZND_F( Listen );
	SCLZND_F( New );
	SCLZND_F( GetAction );
	SCLZND_F( Execute );
	SCLZND_F( Alert );
	SCLZND_F( Confirm );
	SCLZND_F( SetLayout );
	SCLZND_F( GetContents );
	SCLZND_F( SetContents );
	SCLZND_F( DressWidgets );
	SCLZND_F( AddClasses );
	SCLZND_F( RemoveClasses );
	SCLZND_F( ToggleClasses );
	SCLZND_F( EnableElements );
	SCLZND_F( DisableElements );
	SCLZND_F( SetAttribute );
	SCLZND_F( GetAttribute );
	SCLZND_F( RemoveAttribute );
	SCLZND_F( SetProperty );
	SCLZND_F( GetProperty );
	SCLZND_F( Focus );
}

#endif
