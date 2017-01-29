/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'mmuaqcli'.

    'mmuaqcli' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mmuaqcli' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mmuaqcli'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

#define P( name ) rEntry registry::parameter::name( #name, sclrgstry::Parameters )

P( Agent );
P( Folder );
P( Protocol );
P( HostPort );
P( Mail );
P( Name );
P( Parent );
P( Username );
P( Password );

#undef P