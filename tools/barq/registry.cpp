/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'barq'.

    'barq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'barq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'barq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

rEntry registry::parameter::BackendFilename( "BackendFilename", sclr::Parameters );
rEntry registry::parameter::HostService( "HostService", sclr::Parameters );
rEntry registry::parameter::Identifier( "Identifier", sclr::Parameters );
rEntry registry::parameter::OutputFilename( "OutputFilename", sclr::Parameters );

namespace definitions_ {
	rEntry Plugins( "Plugins", sclr::Definitions );
	rEntry Plugin( "Plugin", Plugins );
}

rEntry registry::definition::TaggedPluginFilename( RGSTRY_TAGGING_ATTRIBUTE( "id" ), definitions_::Plugin );
