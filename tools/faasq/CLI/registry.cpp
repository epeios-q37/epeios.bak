/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

    'FaaSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FaaSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "registry.h"

#include "sclmisc.h"

using namespace registry;

namespace {
    rEntry Proxy_("Proxy", sclrgstry::Parameters);
}

rEntry registry::parameter::proxy::Host( "Host", Proxy_);
rEntry registry::parameter::proxy::Service( "Service", Proxy_ );

rEntry registry::parameter::ModuleFilename( "ModuleFilename", sclrgstry::Parameters );

const str::dString &registry::GetWebHost(str::dString &Host)
{
    return sclmisc::MGetValue(parameter::proxy::Host, Host);
}

