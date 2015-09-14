/*
	'ui_base.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: ui_base.cpp,v 1.1 2012/12/04 15:28:45 csimon Exp $

#include "ui_base.h"

#include "trunk.h"

using namespace ui_base;

void ui_base::event_handler__::NSXPCMOnErr( const char *Message )
{
	xulfbs::_Report( Trunk(), Message );
}
