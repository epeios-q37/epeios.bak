/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'RNCq.

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

#ifndef RNCRTN_INC_
# define RNCRTN_INC_

# include "bso.h"
# include "mthrtn.h"
# include "stkcrt.h"
# include "xtf.h"

namespace rncrtn {
	inline bso::sBool GetNumber_(
		xtf::sIFlow &Flow,
		mthrtn::dRational &Number )
	{
		Number.Init( Flow.UndelyingFlow() );

		return true;
	}

	inline void Add_(
		const mthrtn::dRational &Op1,
		const mthrtn::dRational &Op2,
		mthrtn::dRational &Result )
	{
		mthrtn::Add( Op1, Op2, Result );
	}

	inline void Sub_(
		const mthrtn::dRational &Op1,
		const mthrtn::dRational &Op2,
		mthrtn::dRational &Result )
	{
		mthrtn::Sub( Op1, Op2, Result );
	}

	inline void Mul_(
		const mthrtn::dRational &Op1,
		const mthrtn::dRational &Op2,
		mthrtn::dRational &Result )
	{
		mthrtn::Mul( Op1, Op2, Result );
	}

	inline void Div_(
		const mthrtn::dRational &Op1,
		const mthrtn::dRational &Op2,
		mthrtn::dRational &Result )
	{
		mthrtn::Div( Op1, Op2, Result );
	}

	typedef stkcrt::qCSTACKdl( mthrtn::dRational ) dRationals;
	qW( Rationals );
}

#endif
