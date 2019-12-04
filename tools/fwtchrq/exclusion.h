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

#ifndef EXCLUSION_INC_
# define EXCLUSION_INC_

# include "misc.h"

# include "dwtxcl.h"
# include "rgstry.h"

namespace exclusion {

	typedef rgstry::multi_level_registry_ registry_;

	void Get(
		const registry_ &Registry,
		dwtbsc::ghosts_oddities_ &GO );

	void Fill(
		dwtbsc::exclusions_handling__ ExclusionsHandling,
		const registry_ &Registry,
		dwtxcl::excluder_ &Excluder );

	void Fill(
		dwtbsc::exclusions_handling__ ExclusionsHandling,
		const registry_ &Registry,
		dwtbsc::limitations__ &Limitations );

	inline void Fill(
		dwtbsc::exclusions_handling__ ExclusionsHandling,
		const registry_ &Registry,
		dwtxcl::excluder_ &Excluder,
		dwtbsc::limitations__ &Limitations )
	{
		Fill( ExclusionsHandling, Registry, Excluder );
		Fill( ExclusionsHandling, Registry, Limitations );
	}

	inline void Fill(
		const registry_ &Registry,
		dwtxcl::excluder_ &Excluder,
		dwtbsc::limitations__ &Limitations )
	{
		dwtbsc::exclusions_handling__ ExclusionsHandling = misc::GetExclusionsHandling_( Registry );

		Fill( ExclusionsHandling, Registry, Excluder );
		Fill( ExclusionsHandling, Registry, Limitations );
	}
}

#endif
