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

#include "xdh_ups.h"

#include "server.h"

using namespace xdh_ups;

namespace server_ {
	using namespace xdh_ups::server;
}

namespace {
	namespace send_ {
		namespace {
			void SetLayout_(
				flw::sWFlow &Flow,
				server_::rArguments &Arguments )
			{
				::server::layout::set::S( Arguments.Id, Arguments.XML, Arguments.XSLFilename, Arguments.Language, Flow );
			}
			void GetContents_(
				flw::sWFlow &Flow,
				server_::rArguments &Arguments )
			{
				::server::contents::get::S( Arguments.Ids, Flow );
			}
		}

		void Process(
			flw::sWFlow &Flow,
			server_::rServer &Server )
		{
			switch ( Server.Request ) {
			case server_::rSetLayout:
				SetLayout_( Flow, Server.Arguments );
				break;
			case server_::rGetContents:

			default:
				qRGnr();
				break;
			}
		}
	}

	namespace recv_ {
		namespace {
			void SetLayout_(
				flw::sRFlow &Flow,
				sclnjs::dArguments &Agruments )
			{
				::server::layout::set::R( Flow );
			}
			void getContents_(
				flw::sRFlow &Flow,
				sclnjs::dArguments &Arguments )
			{
			qRH;
				str::wStrings Contents;
			qRB;
				Contents.Init();
				::server::contents::get::R( Flow, Contents );

				Arguments.Add( Contents );
			qRR;
			qRT;
			qRE;
			}
		}

		bso::sBool Process(
			server_::eRequest Id,
			flw::sRFlow &Flow,
			sclnjs::dArguments &Arguments )
		{
			switch ( Id ) {
			case server_::r_Undefined:	// New action is launched.
				return false;
				break;
			case server_::rSetLayout:
				SetLayout_( Flow, Arguments );
				break;
			default:
				qRGnr();
				break;
			}

			return true;
		}
	}
}

void *xdh_ups::rProcessing::CSDSCBPreProcess(
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

csdscb::eAction xdh_ups::rProcessing::CSDSCBProcess(
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

	Data.JS.Arguments.Add( Data.XDH );

	if ( !recv_::Process( Data.Server.Request, Flow, Data.JS.Arguments ) )
		::server::GetAction( Flow, Data.JS.Id, Data.JS.Action );

	Data.Lock();

	S_().Upstream( &Data );

	Data.Lock();
	Data.Unlock();

	send_::Process( Flow, Data.Server );
qRR;
qRT;
qRE;
	return csdscb::aContinue;
}

bso::sBool xdh_ups::rProcessing::CSDSCBPostProcess( void *UP )
{
	rData *Data = (rData *)UP;

	if ( Data == NULL )
		qRGnr();

	delete Data;

	return false;
}


