/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

    'FaaSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FaaSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclr.h"

namespace registry {
	using namespace sclr;

	namespace parameter {
		using namespace sclr::parameter;

		namespace proxy {
            extern rEntry
                Host,
                Service;
        }

         extern rEntry _ModuleFilename;
	}

	namespace definition {
		using namespace sclr::definition;
	}

	const str::dString &GetWebHost(str::dString &Host);
}

#endif
