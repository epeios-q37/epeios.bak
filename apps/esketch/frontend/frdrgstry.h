/*
	Copyright (C) 2018 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef FRDRGSTRY__INC
# define FRDRGSTRY__INC

# include "sclrgstry.h"
# include "sclfrntnd.h"

namespace frdrgstry {
	using sclrgstry::dRegistry;
	using sclrgstry::wRegistry;

	using rgstry::rEntry;

	namespace parameter {
		using namespace sclfrntnd::registry::parameter;
	}

	namespace definition {
		using namespace sclfrntnd::registry::definition;
	}
}

#endif
