/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xdwsessn.h"

#include "websck.h"

#include "lstcrt.h"

using namespace xdwsessn;

bso::sBool xdwsessn::rUpstream_::XDHCUCProcess(
	const str::string_ &Script,
	str::dString *ReturnedValue )
{
	bso::sBool Success = true;
qRFH
	qCBUFFERh Buffer;
	websck::rFlow Flow;
qRFB
	Flow.Init(D_(), websck::mWithTerminator);

	Flow.Write( Script.Convert(Buffer), Script.Amount());
	Flow.Commit();

	if ( ReturnedValue != NULL)
		websck::GetMessage(Flow, *ReturnedValue);

	Flow.Dismiss();
qRFR
	Success = false;
	ERRRst();
qRFT
qRFE(sclm::ErrorDefaultHandling())
	return Success;
}
