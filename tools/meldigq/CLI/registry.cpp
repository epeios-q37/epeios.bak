/*
	Copyright (C) 2018 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MelDigQ'.

    'MelDigQ' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MelDigQ' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MelDigQ'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "registry.h"

using namespace registry;

// rEntry registry::parameter::Test( "Test", sclrgstry::Parameters );

rEntry registry::parameter::TargetFileName( "TargetFilename", sclr::Parameters );
rEntry registry::parameter::Script( "Script", sclr::Parameters );

namespace {
	rEntry Devices_( "Devices", sclr::Parameters );
	rEntry In_( "In", Devices_ );
	rEntry Out_( "Out", Devices_ );
}

rEntry registry::parameter::devices::in::Value = In_;
rEntry registry::parameter::devices::in::Policy( "@Policy", In_ );

rEntry registry::parameter::devices::out::Value = Out_;
rEntry registry::parameter::devices::out::Policy( "@Policy", Out_ );

namespace {
	rEntry Signature_( "Signature", sclr::Parameters );
}

rEntry registry::parameter::signature::Key( "Key", Signature_ );

namespace {
	rEntry Time_( "Time", Signature_ );
}

rEntry registry::parameter::signature::time::Numerator( "Numerator", Time_ );
rEntry registry::parameter::signature::time::Denominator( "Denominator", Time_ );

namespace {
	rEntry Tempo_( "Tempo", sclr::Parameters );
}

rEntry registry::parameter::tempo::Value( "Value", Tempo_ );
rEntry registry::parameter::tempo::Unit( "Unit", Tempo_ );


