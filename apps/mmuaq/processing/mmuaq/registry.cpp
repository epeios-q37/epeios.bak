/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq'.

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

rEntry registry::parameter::HostPort("HostPort", sclrgstry::Parameters );
rEntry registry::parameter::Username("Username", sclrgstry::Parameters );
rEntry registry::parameter::Password("Password", sclrgstry::Parameters );
rEntry registry::parameter::Input("Input", sclrgstry::Parameters );
rEntry registry::parameter::Output("Output", sclrgstry::Parameters );
rEntry registry::parameter::Message("Message", sclrgstry::Parameters );
rEntry registry::parameter::Lines("Lines", sclrgstry::Parameters );
rEntry registry::parameter::KeepAnswer("KeepAnswer", sclrgstry::Parameters );


