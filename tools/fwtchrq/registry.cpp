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

#include "registry.h"

using namespace registry;

entry___ registry::Path( "Path", sclrgstry::Parameters );
entry___ registry::Output( "Output", sclrgstry::Parameters );
entry___ registry::ThreadAmountMax( "ThreadAmountMax", sclrgstry::Parameters );

namespace {
	entry___ Exclusions_( "Exclusions", sclrgstry::Parameters );
}

entry___ registry::ExclusionsListFileName( "ListFilename", Exclusions_ );
entry___ registry::ExclusionsHandling( "@Handling", Exclusions_ );
entry___ registry::ExclusionsFileSizeMax( "FileSizeMax", Exclusions_ );
entry___ registry::ExclusionsNameLengthMax( "NameLengthMax", Exclusions_ );

namespace {
	entry___ Ghosts_( "Ghosts", sclrgstry::Parameters );
}

entry___ registry::GhostsPrefix( "Prefix", Ghosts_ );
entry___ registry::GhostsSuffix( "Suffix", Ghosts_ );

