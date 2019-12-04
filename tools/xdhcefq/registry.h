/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY_INC_
# define REGISTRY_INC_

# include "xdhujp.h"
# include "xdhujr.h"

# include "sclrgstry.h"

namespace registry {
	using namespace sclrgstry;

	namespace {
		using rgstry::entry___;
	}
	
    extern entry___ ModuleFilename;

	extern entry___ URL;

	namespace custom_item {
		using namespace xdhujr::custom_item;
	}

	namespace cef {
		extern entry___ ProcessType;
		extern entry___ SingleProcess;
		extern entry___ NoSandbox;
		extern entry___ ResourcesDirPath;
		extern entry___ LocaleDirPath;
		extern entry___ Locale;
		extern entry___ LogFile;
		extern entry___ LogSeverity;
		extern entry___ CachePath;
	}

	using namespace xdhujr;
}

#endif
