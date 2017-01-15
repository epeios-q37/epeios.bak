/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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

namespace {
	rEntry POP3_("POP3", sclrgstry::Parameters );
}
rEntry registry::parameter::pop3::HostPort("@HostPort", POP3_ );
rEntry registry::parameter::pop3::Username("Username", POP3_ );
rEntry registry::parameter::pop3::Password("Password", POP3_ );

namespace {
	rEntry IMAP_("IMAP", sclrgstry::Parameters );
}

rEntry registry::parameter::imap::HostPort("@HostPort", IMAP_ );
rEntry registry::parameter::imap::Username("Username", IMAP_ );
rEntry registry::parameter::imap::Password("Password", IMAP_ );
rEntry registry::parameter::imap::Reference("Reference", IMAP_ );
rEntry registry::parameter::imap::Mailbox("Mailbox", IMAP_ );
rEntry registry::parameter::imap::Sequence("Sequence", IMAP_ );
rEntry registry::parameter::imap::Items("Items", IMAP_ );

rEntry registry::parameter::Input("Input", sclrgstry::Parameters );
rEntry registry::parameter::Output("Output", sclrgstry::Parameters );
rEntry registry::parameter::Message("Message", sclrgstry::Parameters );
rEntry registry::parameter::Lines("Lines", sclrgstry::Parameters );
rEntry registry::parameter::Verbose("Verbose", sclrgstry::Parameters );
rEntry registry::parameter::FieldName("FieldName", sclrgstry::Parameters );
rEntry registry::parameter::Tag("Tag", sclrgstry::Parameters );


