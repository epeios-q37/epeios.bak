/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "xdwrgstry.h"

namespace registry {
	using xdwrgstry::rEntry;

	namespace parameter {
		using namespace xdwrgstry::parameter;

		extern rEntry Service;
	}

	namespace definition {
		using namespace xdwrgstry::definition;

		extern rEntry
            ErrorHead,  // HTML head used for inexisting token.
            ErrorScript;  // JS script launched on inexisting token.
	}
}

#endif
