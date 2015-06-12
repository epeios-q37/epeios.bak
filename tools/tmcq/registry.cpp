/*
	Copyright (C) 2007-2011, 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of tmcq.

    tmcq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    tmcq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with tmcq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "registry.h"

using namespace registry;

entry___ registry::FPS("FPS", sclrgstry::Parameters );
entry___ registry::Format("Format", sclrgstry::Parameters );
entry___ registry::FormatAlias("FormatAlias", sclrgstry::Parameters );
entry___ registry::Tokens("Tokens", sclrgstry::Parameters );

namespace {
	entry___ PredefinedFormats_("PredefinedFormats", sclrgstry::Definitions );
}

entry___ registry::PredefinedFormat( RGSTRY_TAGGED_ENTRY("PredefinedFormat", "id" ), PredefinedFormats_ );

namespace {
	entry___ Timecodes_("Timecodes", sclrgstry::Parameters );
}

entry___ registry::Timecode("Timecode", Timecodes_ );
