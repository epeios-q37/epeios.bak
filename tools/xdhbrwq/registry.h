/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'xdhbrwq.

    'xdhbrwq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhbrwq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhbrwq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclrgstry.h"

# include "xdhujr.h"

namespace registry {
	using namespace sclrgstry;

	using namespace xdhujr;

	namespace parameter {
		using namespace sclrgstry::parameter;

		extern rEntry Service;

		namespace script {
			extern rEntry Fallback;
		}

	    extern rEntry ModuleFilename;
	}

	namespace definition {
		using namespace sclrgstry::definition;
	}
}

#endif

