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

#ifndef XDWRGSTRY_INC_
# define XDWRGSTRY_INC_

# include "sclrgstry.h"

# include "xdhujr.h"

namespace xdwrgstry {
	using namespace sclrgstry;

	using namespace xdhujr;

	namespace parameter {
		using namespace sclrgstry::parameter;

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

