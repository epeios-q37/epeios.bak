#include "rnctol.h"
/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'RNCq'.

    'RNCq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'RNCq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'RNCq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "rnctol.h"

# include "rncalg.h"
# include "rncrpn.h"

using namespace rnctol;

namespace {
	bso::sBool Evaluate_(
		xtf::sIFlow &XFlow,
		bso::sBool RPN,
		txf::sWFlow &OFlow )
	{
		bso::sBool Success = false;
	qRH
		mthrtn::wRational Number;
	qRB
		Number.Init();

	if ( RPN )
		Success = rncrpn::Evaluate<mthrtn::dRational, mthrtn::wRational, rncrtn::dRationals, rncrtn::wRationals>( XFlow, Number );
	else
		Success = rncalg::Evaluate<mthrtn::dRational, mthrtn::wRational, rncrtn::dRationals, rncrtn::wRationals>( XFlow, Number );

		if ( Success ) {
			Number.Simplify();

			OFlow << Number.N << " / " << Number.D;
		}
	qRR
	qRT
	qRE
		return Success;
	}

	bso::sBool Evaluate_(
		xtf::sIFlow &XFlow,
		bso::sBool RPN,
		str::dString &Result )
	{
		bso::sBool Success = false;
	qRH
		flx::rStringTWFlow OFlow;
	qRB
		OFlow.Init( Result );

		Success = Evaluate_( XFlow, RPN, OFlow );
	qRR
	qRT
	qRE
		return Success;
	}
}

bso::sBool rnctol::Evaluate( 
	str::dString &Expression,
	bso::sBool RPN,
	str::dString &Result )
{
	bso::sBool Success = false;
qRH
	flx::sStringIFlow IFlow;
	xtf::sIFlow XFlow;
qRB
	IFlow.Init( Expression );
	XFlow.Init( IFlow, utf::f_Guess );

	Success = Evaluate_( XFlow, RPN, Result );
qRR
qRT
qRE
	return Success;
}

