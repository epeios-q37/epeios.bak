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


#include "registry.h"

using namespace registry;

rEntry registry::parameter::UseCurrentDir("@UseCurrentDir", sclr::Parameters);
rEntry registry::parameter::Tags( "Tags", sclr::Parameters );
rEntry registry::parameter::Input( "Input", sclr::Parameters );
rEntry registry::parameter::Output( "Output", sclr::Parameters );

namespace {
  rEntry Delimiters_("Delimiters", sclr::Parameters);
}

rEntry registry::parameter::TagDelimiter("@Tag", Delimiters_);

namespace {
   rEntry Markers_("Markers", sclr::Parameters);
}

rEntry registry::parameter::CommentMarker("@Comment", Markers_);
rEntry registry::parameter::TextMarker("@Text", Markers_);
rEntry registry::parameter::FileMarker("@File", Markers_);
rEntry registry::parameter::CommandMarker("@Command", Markers_);
