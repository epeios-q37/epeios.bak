/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of dmnzq.

    dmnzq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    dmnzq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with dmnzq.  If not, see <http://www.gnu.org/licenses/>
*/

#include "registry.h"

using namespace registry;

static entry___ Module_( "Module", sclrgstry::Parameters );

static entry___ ModuleLog_( "Log", Module_ );
entry___ registry::ModuleLogFilename( "Filename", ModuleLog_ );
entry___ registry::ModuleLogMode( "@Mode", ModuleLog_ );

entry___ registry::ModuleFilename( "Filename", Module_ );

entry___ registry::ModuleConnection( "Connection", Module_ );
entry___ registry::ModuleConnectionType( "@Type", ModuleConnection );


