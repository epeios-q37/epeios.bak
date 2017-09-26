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

#ifndef RNC_RPN_
# define RNC_RPN_

# include "rnccmn.h"
# include "xtf.h"

namespace rncrpn {
	using rnccmn::dOperators;
	using rnccmn::wOperators;

	template <typename dnumber, typename wnumber, typename dnumbers, typename wnumbers> bso::sBool Evaluate(
		xtf::sIFlow &Flow,
		dnumber &Result )
	{
		bso::sBool Success = false;
	qRH
		wnumbers Numbers;
		wnumber Number;
		wOperators Operators;
		bso::sChar Operator = 0;
	qRB
		SkipSpaces_( Flow );

		Numbers.Init();
		Operators.Init();

		Number.Init();

		if ( !GetNumber_( Flow, Number ) )
			qRReturn;

		Numbers.Push( Number );

		SkipSpaces_( Flow );

		while ( !Flow.EndOfFlow() ) {
			if ( isdigit( Flow.View() ) ) {
				Number.Init();
				if ( !GetNumber_( Flow, Number ) )
					qRReturn;
				Numbers.Push( Number );

				SkipSpaces_( Flow );

			} else {
				Operator = Flow.Get();

				if ( !rnccmn::Handle_<wnumber, dnumbers>( Numbers, Operator ) )
					qRReturn;

				SkipSpaces_( Flow );
			}
		}

		if ( Numbers.Amount() != 1 )
			qRReturn;

		Numbers.Pop( Result );

		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}
}

#endif
