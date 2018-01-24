/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "proxy.h"

#include "prtcl.h"

#include "sclmisc.h"

using namespace proxy;

namespace {
	void Handshake_(
		flw::sRFlow &Flow,
		str::dString & Language )
	{
		csdcmn::sVersion Version = csdcmn::UndefinedVersion;

		if ( (Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow )) != prtcl::ProtocolVersion )
			qRGnr();

		prtcl::Get( Flow, Language );
		Flow.Dismiss();
	}

	void GetAction_(
		flw::sRWFlow &Flow,
		str::dString &Id,
		str::dString &Action )
	{
		if ( prtcl::GetRequest( Flow ) != prtcl::rLaunch_1 )
			qRGnr();

		prtcl::Get( Flow, Id );
		prtcl::Get( Flow, Action );
		Flow.Dismiss();
	}
}

void *proxy::rProcessing::CSDSCBPreProcess(
	fdr::rRWDriver *IODriver,
	const ntvstr::char__ *Origin )
{
	rData *Data = NULL;
qRH;
	flw::sDressedRWFlow<> Flow;
qRB;
	Data = PRXYNew();

	if ( Data == NULL )
		qRAlc();

	Flow.Init( *IODriver );
	Handshake_( Flow, Data->Language );

	Data->Lock();

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

csdscb::eAction proxy::rProcessing::CSDSCBProcess(
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

	if ( prxy_recv::Recv( Data.Request, Flow, Data.Return ) ) {
		Data.Request = prxy_cmn::r_Undefined;
		Data.Lock();
		PRXYOnPending( &Data );
	}  else {
		GetAction_( Flow, Data.Id, Data.Action );
		Data.Lock();
		PRXYOnAction( &Data );
	}

	Data.Lock();
	Data.Unlock();

	if ( !prxy_send::Send( Data.Request, Flow, Data.Arguments ) )
		prtcl::PutAnswer( prtcl::aOK_1, Flow );
qRR;
qRT;
qRE;
	return csdscb::aContinue;
}

bso::sBool proxy::rProcessing::CSDSCBPostProcess( void *UP )
{
	rData *Data = (rData *)UP;

	if ( Data == NULL )
		qRGnr();

	delete Data;

	return false;
}


