/*
	Header for the 'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhcefq' tool.
	Copyright (C) 2014 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "xdhjsp.h"
# include "xdhjsr.h"

# include "sclrgstry.h"

namespace registry {
	using namespace sclrgstry;

	namespace {
		using rgstry::entry___;
	}
	
    extern entry___ ModuleFilename;

	extern entry___ URL;

	namespace custom_item {
		using namespace xdhjsr::custom_item;
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
	}

	using namespace xdhjsr;
}

#endif
