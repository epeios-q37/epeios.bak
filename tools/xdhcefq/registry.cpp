/*
	Copyright (C) 2014-2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

    This file is part of 'xdhcefq'.

    'xdhcefq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhcefq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhcefq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

entry___ registry::ModuleFilename( "ModuleFilename", sclrgstry::Parameters );

entry___ registry::URL("URL", sclrgstry::Parameters );

static entry___ CEF_( "CEF", Parameters );
entry___ registry::cef::ProcessType( "ProcessType", CEF_ );
entry___ registry::cef::SingleProcess( "SingleProcess", CEF_ );
entry___ registry::cef::NoSandbox( "NoSandbox", CEF_ );
entry___ registry::cef::ResourcesDirPath( "ResourcesDirPath", CEF_ );
entry___ registry::cef::LocaleDirPath( "LocaleDirPath", CEF_ );
entry___ registry::cef::Locale( "Locale", CEF_ );
entry___ registry::cef::LogFile( "LogFile", CEF_ );
entry___ registry::cef::LogSeverity( "LogSeverity", CEF_ );
entry___ registry::cef::CachePath( "CachePath", CEF_ );
//entry___ registry::cef::( "", CEF_ );

