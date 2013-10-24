/*
	'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'xxx_app' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

entry___ registry::Data( "Data" );
entry___ registry::Output( "Output" );
entry___ registry::XSL( "XSL" );
entry___ registry::Context( "Context" );
entry___ registry::Command( "Command" );
entry___ registry::SessionMaxDuration( "SessionMaxDuration" );

static entry___ FreeRandom_( "Random", sclrgstry::Parameters );
entry___ registry::RandomName( "@Name", FreeRandom_ );
static entry___ TaggedRandom_( RGSTRY_TAGGING_ATTRIBUTE( "Name" ), FreeRandom_ );
entry___ registry::TaggedRandomLimit( "@Limit", TaggedRandom_ );

