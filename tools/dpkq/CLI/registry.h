/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dpkq.

    dpkq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dpkq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dpkq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "sclrgstry.h"

namespace registry {
	using namespace sclrgstry;

	namespace {
		using rgstry::entry___;
	}

	extern entry___ NamespaceLabel;
	extern entry___ NamespaceURI;
	
	extern entry___ Data;
	extern entry___ Output;
	extern entry___ XSL;
	extern entry___ Context;
	extern entry___ Viewer;
	extern entry___ SessionMaxDuration;

	extern entry___ Id;
	extern entry___ RandomName;
	extern entry___ TaggedRandomLimit;
}

#endif
