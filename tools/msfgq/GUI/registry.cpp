/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'MSFGq' software.

  'MSFGq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'MSFGq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'MSFGq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

rEntry registry::definition::Body( "Body", sclr::Definitions);

rEntry registry::definition::Script( "Script", sclr::Definitions );

namespace {
	rEntry Signature_( "Signature", sclr::Parameters );
}

rEntry registry::parameter::signature::Key( "Key", Signature_ );
rEntry registry::parameter::signature::Time( "Time", Signature_ );

namespace {
	rEntry Devices_( "Devices", sclr::Parameters );
	rEntry In_( "In", Devices_ );
	rEntry Out_( "Out", Devices_ );
}

rEntry registry::parameter::devices::in::Value = In_;
rEntry registry::parameter::devices::in::Policy( "@Policy", In_ );


