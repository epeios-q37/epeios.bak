/*
	'registry.cpp' by Claude SIMON (http://q37.info/contact/).

	 This file is part of 'eGeckoCOM' software.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: registry.cpp,v 1.5 2012/02/24 18:50:15 csimon Exp $

#include "registry.h"

using namespace registry;

static rgstry::entry___ Components_( "Components" );
rgstry::entry___ registry::TaggedComponent( RGSTRY_TAGGED_ENTRY( "Component", "id" ), Components_ );

