/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq.

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

#ifndef MISC__INC
# define MISC__INC

# include "fdr.h"
# include "flw.h"
# include "flx.h"
# include "cio.h"

namespace misc {
	inline void Dump( fdr::rIDriver &Driver )
	{
	qRH
		flw::sDressedIFlow<> Flow;
	qRB
		Flow.Init( Driver );

		while ( !Flow.EndOfFlow() )
			cio::COut << Flow.Get() << txf::commit;

		Flow.Dismiss();
	qRR
	qRT
	qRE
	}

	inline void Dump(
		fdr::rIDriver &Driver,
		str::dString &Content )
	{
	qRH
		flx::rStringODriver ODriver;
	qRB
		ODriver.Init( Content, fdr::ts_Default );

		fdr::Copy( Driver, ODriver );
	qRR
	qRT
	qRE
	}
}

#endif
