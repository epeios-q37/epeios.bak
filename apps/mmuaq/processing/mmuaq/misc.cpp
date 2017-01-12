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

#include "misc.h"

#include "registry.h"

#include "sclmisc.h"

#include "rgstry.h"

using namespace misc;

bso::sBool misc::IsVerboseActivated( void )
{
	return sclmisc::BGetBoolean( registry::parameter::Verbose, false );
}

void misc::rVerboseIODriver::Init(
	const rgstry::rEntry &HostPortEntry,
	bso::sBool Activate )
{
	qRH
		str::wString HostPort;
		qCBUFFERr Buffer;
	qRB
		HostPort.Init();
		sclmisc::MGetValue( HostPortEntry, HostPort );

		if ( !Driver_.Init( HostPort.Convert( Buffer ), SCK_INFINITE, qRPU ) )
			sclmisc::ReportAndAbort("UnableToConnect", HostPort );

		Activated_ = Activate;
		Commited_ = true;
		rIODriver_::Init( fdr::ts_Default );
	qRR
	qRT
	qRE
}
