#include "main.h"
/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'ZNDq'.

    'ZNDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'ZNDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'ZNDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "zndq.h"

#include "sclmisc.h"

using namespace main;

namespace{
	void GetInfo_(
		const char *PHPVersion,
		str::dString &Info )
	{
	qRH
		flx::rStringOFlow BaseFlow;
		txf::sOFlow Flow;
	qRB
		BaseFlow.Init( Info );
		Flow.Init( BaseFlow );

		Flow << sclmisc::SCLMISCProductName << " v" << VERSION << " - PHP v" << PHPVersion << txf::nl
			<< txf::pad << "Build : " __DATE__ " " __TIME__ " (" <<  cpe::GetDescription() << ')';
	qRR
	qRT
	qRE
	}
}

const char *main::GetInfo( const char *PHPVersion )
{
	static char Buffer[1000];
qRH
	str::wString Info;
qRB
	Info.Init();
	GetInfo_( PHPVersion, Info );

	if ( Info.Amount() >= sizeof( Buffer ) )
		qRLmt();

	Info.Recall( Info.First(), Info.Amount(), Buffer );
	Buffer[Info.Amount()] = 0;
qRR
qRT
qRE
	return Buffer;
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
