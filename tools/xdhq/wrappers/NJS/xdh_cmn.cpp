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

#include "proxy.h"

using namespace xdh_cmn;

void *xdh_cmn::rProcessing::CSDSCBPreProcess(
	fdr::rRWDriver *IODriver,
	const ntvstr::char__ *Origin )
{
	rData *Data = NULL;
qRH;
	flw::sDressedRWFlow<> Flow;
qRB;
	Data = new rData;

	if ( Data == NULL )
		qRAlc();

	Data->Init();

	Flow.Init( *IODriver );
	proxy::Handshake( Flow, Data->Language );

	Data->Lock();

	S_().Upstream( Data );

	Data->Lock();
	Data->Unlock();

	prtcl::PutAnswer( prtcl::aOK_1, Flow );
	Flow.Commit();
qRR;
	if ( Data != NULL )
		delete Data;
qRT;
qRE;
	return Data;
}

csdscb::eAction xdh_cmn::rProcessing::CSDSCBProcess(
	fdr::rRWDriver *IODriver,
	void *UP )
{
qRH;
	flw::sDressedRWFlow<> Flow;
	rData &Data = *(rData *)UP;
qRB;
	if ( UP == NULL )
		qRGnr();

	Flow.Init( *IODriver );

	Data.Return.Init();

	if ( !proxy::Recv( Data.Request, Flow, Data.Return ) )
		proxy::GetAction( Flow, Data.Id, Data.Action );

	Data.Request = proxy::r_Undefined;

	Data.Lock();

	S_().Upstream( &Data );

	Data.Lock();
	Data.Unlock();

	if ( !proxy::Send( Data.Request, Flow, Data.Arguments ) )
		prtcl::PutAnswer( prtcl::aOK_1, Flow );
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

