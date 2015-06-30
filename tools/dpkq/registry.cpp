/*
	Copyright (C) 2010-2015 Claude SIMON (http://q37.info/contact/).

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

#include "registry.h"

using namespace registry;

namespace {
	entry___ Namespace_("Namespace", Parameters );
}

entry___ registry::NamespaceLabel( "Label", Namespace_ );
entry___ registry::NamespaceURI( "URI", Namespace_ );


entry___ registry::Data( "Data", sclrgstry::Parameters );
entry___ registry::Output( "Output", sclrgstry::Parameters );
entry___ registry::XSL( "XSL", sclrgstry::Parameters );
entry___ registry::Context( "Context", sclrgstry::Parameters );
entry___ registry::Viewer( "Viewer", sclrgstry::Parameters );
entry___ registry::SessionMaxDuration( "SessionMaxDuration", sclrgstry::Parameters );

entry___ registry::Id( "Id", sclrgstry::Parameters );

static entry___ FreeRandom_( "Random", sclrgstry::Parameters );
entry___ registry::RandomName( "@Name", FreeRandom_ );
static entry___ TaggedRandom_( RGSTRY_TAGGING_ATTRIBUTE( "Name" ), FreeRandom_ );
entry___ registry::TaggedRandomLimit( "@Limit", TaggedRandom_ );

