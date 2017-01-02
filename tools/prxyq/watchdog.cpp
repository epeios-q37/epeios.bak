/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'prxyq'.

    'prxyq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'prxyq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'prxyq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "watchdog.h"

#include "registry.h"

#include "prxybase.h"

#include "sclmisc.h"

#include "flw.h"
#include "csdcmn.h"
#include "sck.h"
#include "csdbnc.h"

using namespace watchdog;

namespace {
	void Init_( csdbnc::rIOFlow &Flow )
	{
	qRH
		str::wString AddressService;
		qCBUFFERr Buffer;
	qRB
		AddressService.Init();
		sclmisc::MGetValue( registry::parameter::watchdog::Target, AddressService );

		if ( !Flow.Init( AddressService.Convert( Buffer ), sclmisc::MGetUInt( registry::parameter::watchdog::Timeout ), err::hUserDefined ) )
			sclmisc::ReportAndAbort( "UnableToConnectToProxy", AddressService );
	qRR
	qRT
	qRE
	}

	void HandleAnswer_(
		flw::sIFlow &Flow,
		prxybase::eAnswer ExpectedAnswer)
	{
		prxybase::eAnswer Answer = prxybase::a_Undefined;

		if ( Flow.EndOfFlow() || ( ( Answer = prxybase::GetAnswer(Flow) ) != ExpectedAnswer) ) {
			if ( Answer == prxybase::aForbidden )
				sclmisc::ReportAndAbort( "ForbiddenRequest" );
			else
				sclmisc::ReportAndAbort( "BadAnswerFromProxy" );
		}

		Flow.Dismiss();
	}
}

namespace {
	void Ping_(	flw::sIOFlow &Flow )
	{
		csdcmn::SendProtocol( prxybase::ProtocolId, prxybase::ProtocolVersion, Flow );

		prxybase::PutRequest( prxybase::rPing_1, Flow );

		Flow.Commit();

		HandleAnswer_( Flow, prxybase::aOK );
	}
}

void watchdog::Ping( void )
{
qRH
	csdbnc::rIOFlow Flow;
qRB
	Init_( Flow );
	Ping_( Flow );
qRR
qRT
qRE
}

namespace {
	void Freeze_(	flw::sIOFlow &Flow )
	{
		csdcmn::SendProtocol( prxybase::ProtocolId, prxybase::ProtocolVersion, Flow );

		prxybase::PutRequest( prxybase::rFreeze_1, Flow );

		Flow.Commit();

		HandleAnswer_( Flow, prxybase::aOK );
	}
}

void watchdog::Freeze( void )
{
qRH
	csdbnc::rIOFlow Flow;
qRB
	Init_( Flow );
	Freeze_( Flow );
qRR
qRT
qRE
}


namespace {
	void Crash_(	flw::sIOFlow &Flow )
	{
		csdcmn::SendProtocol( prxybase::ProtocolId, prxybase::ProtocolVersion, Flow );

		prxybase::PutRequest( prxybase::rCrash_1, Flow );

		Flow.Commit();

		HandleAnswer_( Flow, prxybase::a_Undefined );
	}
}

void watchdog::Crash( void )
{
qRH
	csdbnc::rIOFlow Flow;
qRB
	Init_( Flow );
	Crash_( Flow );
qRR
qRT
qRE
}

