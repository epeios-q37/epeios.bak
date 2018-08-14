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
	namespace {
		template <typename items> void Send_(
			const items &Items,
			flw::sWFlow &Flow )
		{
			//			prtcl::Put( Items.Amount(), Flow );

			sdr::sRow Row = Items.First();

			if ( Row != qNIL ) {
				while ( Row != qNIL ) {
					prtcl::Put( Items( Row ), Flow );

					Row = Items.Next( Row );
				}
			}
		}
	}
}

void proxy::Send(
	flw::sWFlow &Flow,
	const rArguments &Arguments )
{
qRH;
	qCBUFFERr Buffer;
qRB;
	flw::PutString( Arguments.Command.Convert( Buffer ), Flow );
	Send_( Arguments.Strings, Flow );
	Send_( Arguments.XStrings, Flow );
qRR;
qRT;
qRE;
}

void proxy::Recv(
	eType ReturnType,
	flw::sRFlow &Flow,
	rReturn &Return )
{
	switch ( ReturnType ) {
	case tVoid:
		break;
	case tString:
		prtcl::Get( Flow, Return.StringToSet() );
		break;
	case tStrings:
		prtcl::Get( Flow, Return.StringsToSet() );
		break;
	default:
		qRGnr();
		break;
	}
}

void proxy::Handshake_(
	flw::sRFlow &Flow,
	str::dString & Language )
{
	csdcmn::sVersion Version = csdcmn::UndefinedVersion;

	if ( (Version = csdcmn::GetProtocolVersion( prtcl::ProtocolId, Flow )) != prtcl::ProtocolVersion )
		qRGnr();

	prtcl::Get( Flow, Language );
	Flow.Dismiss();
}

void proxy::GetAction_(
	flw::sRWFlow &Flow,
	str::dString &Id,
	str::dString &Action )
{
	prtcl::Get( Flow, Id );
	prtcl::Get( Flow, Action );
	Flow.Dismiss();
}

