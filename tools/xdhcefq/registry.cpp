/*
	'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xdhcefq' tool.
	Copyright (C) 2014 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

entry___ registry::ModuleFilename( "ModuleFilename", sclrgstry::Parameters );

entry___ registry::URL("URL", sclrgstry::Parameters );

static entry___ Tags_("Tags", xdhjsr::CustomItems );

static entry___ KeyTag_("Key", Tags_ );
entry___ registry::custom_item::tag::key::Name("Name", KeyTag_ );
static entry___ KeyTagAttributeNames_("AttributeNames", KeyTag_ );
entry___ registry::custom_item::tag::key::attribute_name::Key("Key", KeyTagAttributeNames_ );
entry___ registry::custom_item::tag::key::attribute_name::Modifiers("Modifiers", KeyTagAttributeNames_ );
entry___ registry::custom_item::tag::key::attribute_name::Action("Action", KeyTagAttributeNames_ );

static entry___ CEF_( "CEF", Parameters );
entry___ registry::cef::ProcessType( "ProcessType", CEF_ );
entry___ registry::cef::SingleProcess( "SingleProcess", CEF_ );
entry___ registry::cef::NoSandbox( "NoSandbox", CEF_ );
entry___ registry::cef::ResourcesDirPath( "ResourcesDirPath", CEF_ );
entry___ registry::cef::LocaleDirPath( "LocaleDirPath", CEF_ );
entry___ registry::cef::Locale( "Locale", CEF_ );
entry___ registry::cef::LogFile( "LogFile", CEF_ );
entry___ registry::cef::LogSeverity( "LogSeverity", CEF_ );
//entry___ registry::CEF( "", CEF_ );

