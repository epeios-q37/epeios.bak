/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'MIDXq' tool.

    'MIDXq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MIDXq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MIDXq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "registry.h"

using namespace registry;

rEntry registry::parameter::MIDISource( "MIDISource", sclr::Parameters );
rEntry registry::parameter::MIDITarget( "MIDITarget", sclr::Parameters );
rEntry registry::parameter::XMIDSource( "XMIDSource", sclr::Parameters );
rEntry registry::parameter::XMIDTarget( "XMIDTarget", sclr::Parameters );

