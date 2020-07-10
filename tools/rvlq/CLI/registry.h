/*
	Copyright (C) 2000 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'RVLq' tool.

    'RVLq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'RVLq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'RVLq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclr.h"

namespace registry {
	using namespace sclr;

	namespace parameter {
		using namespace sclr::parameter;

		extern rEntry
			Tags,
			Input,
			Output,
			Delimiter,
			CommentMarker,
			TextMarker,
			FileMarker,
			;
	}

	namespace definition {
		using namespace sclr::definition;
	}
}

#endif
