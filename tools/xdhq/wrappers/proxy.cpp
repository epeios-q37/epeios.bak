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

void proxy::Handshake(
	flw::sRFlow &Flow,
	str::dString & Language )
{
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;

	if ( ( Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow ) ) != prtcl::ProtocolVersion )
		qRGnr();

	prtcl::Get( Flow, Language );
	Flow.Dismiss();
}

void proxy::GetAction(
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

void *proxy::rProcessing_::CSDSCBPreProcess(
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

	Data->Init();

	Flow.Init( *IODriver );
	proxy::Handshake( Flow, Data->Language );

	Data->Lock();

	S_().Write( Data );

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

csdscb::eAction proxy::rProcessing_::CSDSCBProcess(
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

	//Data.Request = proxy::r_Undefined;

	Data.Lock();

	S_().Write( &Data );

	Data.Lock();
	Data.Unlock();

	if ( !proxy::Send( Data.Request, Flow, Data.Arguments ) )
		prtcl::PutAnswer( prtcl::aOK_1, Flow );
qRR;
qRT;
qRE;
	return csdscb::aContinue;
}

bso::sBool proxy::rProcessing_::CSDSCBPostProcess( void *UP )
{
	rData *Data = (rData *)UP;

	if ( Data == NULL )
		qRGnr();

	PRXYDelete( Data );

	return false;
}


