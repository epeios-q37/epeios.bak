/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHq.

    XDHq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdh_cmn.h"

#include "server.h"

using namespace xdh_cmn;

void *xdh_cmn::rProcessing::CSDSCBPreProcess(
	fdr::rIODriver *IODriver,
	const ntvstr::char__ *Origin )
{
	rData *Data = NULL;
qRH;
	flw::sDressedRWFlow<> Flow;
	str::wString Id, Action;
qRB;
	Data = new rData;

	if ( Data == NULL )
		qRAlc();

	Data->Init();

	Flow.Init( *IODriver );

	S_().Upstream( Data );
qRR;
	if ( Data != NULL )
		delete Data;
qRT;
qRE;
	return Data;
}

csdscb::eAction xdh_cmn::rProcessing::CSDSCBProcess(
	fdr::rIODriver *IODriver,
	void *UP )
{
qRH;
	flw::sDressedRWFlow<> Flow;
	rData &Data = *(rData *)UP;
qRB;
	if ( UP == NULL )
		qRGnr();

	Flow.Init( *IODriver );

	Data.JS.Arguments.Init();

	if ( Data.XDH.HasAssignation() )	// If not, this is the first call (new connection).
		Data.JS.Arguments.Add( Data.XDH );
	else
		server::Handshake( Flow, Data.Language );

	if ( !xdh_ups::Recv( Data.Server.Request, Flow, Data.JS.Arguments ) )
		server::GetAction( Flow, Data.JS.Id, Data.JS.Action );

	Data.Lock();

	S_().Upstream( &Data );

	Data.Lock();
	Data.Unlock();

	xdh_ups::Send( Flow, Data.Server );
qRR;
qRT;
qRE;
	return csdscb::aContinue;
}

bso::sBool xdh_cmn::rProcessing::CSDSCBPostProcess( void *UP )
{
	rData *Data = (rData *)UP;

	if ( Data == NULL )
		qRGnr();

	delete Data;

	return false;
}

