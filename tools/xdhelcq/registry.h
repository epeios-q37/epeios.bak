/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHELCq.

    'XDHELCq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHELCq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHELCq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "sclrgstry.h"

# include "xdhujr.h"

namespace registry {
	using namespace sclrgstry;

	namespace parameter {
		using namespace sclrgstry::parameter;

		extern rEntry
			ModuleFilename,
			Token;	// If defined, DEMO, otherwise PROD.
	}

	namespace definition {
		using namespace sclrgstry::definition;

		namespace script {
			extern rEntry
				OpenFileDialogScript,
				OpenFilesDialogScript,
				SaveFileDialogScript;
		}
	}

	using namespace xdhujr;
}

#endif
