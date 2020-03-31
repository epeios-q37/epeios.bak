/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'Trial' tool.

    'Trial' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Trial' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'Trial'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclrgstry.h"

namespace registry {
	using namespace sclrgstry;

	namespace parameter {
		using namespace sclrgstry::parameter;

		namespace t1 {
            extern rEntry Value;
		}
	}

	namespace definition {
		using namespace sclrgstry::definition;
	}
}

#endif