/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of fwtchrq.

    fwtchrq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    fwtchrq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with fwtchrq.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef EXCLUSION__INC
# define EXCLUSION__INC

# include "dwtxcl.h"
# include "rgstry.h"

namespace exclusion {

	typedef rgstry::multi_level_registry_ registry_;

	void Get(
		const registry_ &Registry,
		dwtbsc::ghosts_oddities_ &GO );

	void Fill(
		const registry_ &Registry,
		dwtxcl::excluder_ &Excluder );

	void Fill(
		const registry_ &Registry,
		dwtbsc::limitations__ &Limitations );
}

#endif
