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

#ifndef BROWSE__INC
# define BROWSE__INC

# include "dwtftr.h"

# include "rgstry.h"
# include "str.h"

namespace browse {
	void Browse(
		const rgstry::multi_level_registry_ &Registry,
		const str::string_ &Path,
		const str::string_ &Generator,
		const fnm::name___ &OutputFileName,
		dwtdct::exploration_observer__ &ExplorationObserver,
		dwtftr::processing_observer___ &ProcessingObserver );
}

#endif
