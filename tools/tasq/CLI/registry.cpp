/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'tasq' tool.

    'tasq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'tasq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'tasq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "registry.h"

using namespace registry;

using sclr::rEntry;

sclr::rEntry registry::parameter::XMLFilename( "XMLFilename", sclr::Parameters );

