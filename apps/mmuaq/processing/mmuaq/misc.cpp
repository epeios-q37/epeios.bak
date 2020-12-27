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

#include "sclm.h"

#include "rgstry.h"

using namespace misc;

# define M( name ) extern qCDEF( char *, misc::message::name, #name )
M( NoCorrespondingMail );
M( UnableToConnect );
M( UnknownField );
/*
		M(  );
*/

bso::sBool misc::IsVerboseActivated( void )
{
	return sclm::OGetBoolean( registry::parameter::Verbose, false );
}

namespace {
	flx::sMarkers Markers_;// = { { "<- ", "--\n" }, { "-> ", "" }, false };
}

void misc::rVerboseIODriver::Init(
	const rgstry::rEntry &HostPortEntry,
	flx::eChannel Channel )
{
	qRH
		str::wString HostPort;
		qCBUFFERh Buffer;
	qRB
		::Markers_.In.Before = "<- " ;
		::Markers_.In.After = "--\n";
		::Markers_.Out.Before = "-> ";
		HostPort.Init();
		sclm::MGetValue( HostPortEntry, HostPort );

		if ( !Driver_.Init( HostPort.Convert( Buffer ), SCK_INFINITE, qRPU ) )
			sclm::ReportAndAbort( message::UnableToConnect, HostPort );

		rIODriver_::Init( Driver_, Channel, ::Markers_, cio::COut );
	qRR
	qRT
	qRE
}
