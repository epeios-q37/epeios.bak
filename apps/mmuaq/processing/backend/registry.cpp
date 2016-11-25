/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

namespace definition_ {
	namespace {
		rEntry Demo_("Demo", Definitions );
	}

	namespace demo {
		rEntry Agents( "Agents", Demo_ );
		rEntry FreeAgent( "Agent", Agents );
		rEntry TaggedAgent( RGSTRY_TAGGING_ATTRIBUTE("id"), FreeAgent );
	}
}

rEntry registry::definition::demo::agent::Id( "@id", definition_::demo::FreeAgent );
rEntry registry::definition::demo::agent::TaggedHostPort( "HostPort", definition_::demo::TaggedAgent );
rEntry registry::definition::demo::agent::TaggedUsername( "Username", definition_::demo::TaggedAgent );
rEntry registry::definition::demo::agent::TaggedPassword( "Password", definition_::demo::TaggedAgent );

namespace definition_ {
	rEntry Limitations("Limitations", Definitions );
}

rEntry registry::definition::limitation::AgentNameLength( "AgentNameLength", definition_::Limitations );
rEntry registry::definition::limitation::HostPortLength( "HostPortLength", definition_::Limitations );
rEntry registry::definition::limitation::UsernameLength( "UsernameLength", definition_::Limitations );
rEntry registry::definition::limitation::PasswordLength( "Length", definition_::Limitations );
