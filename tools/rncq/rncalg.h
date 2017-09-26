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

#ifndef RNCALG_INC_
# define RNCALG_INC_

# include "rnccmn.h"

# include "bso.h"
# include "xtf.h"

namespace rncalg {
	using rnccmn::dOperators;
	using rnccmn::wOperators;

	template <typename dnumber, typename wnumber, typename dnumbers> bso::sBool Handle_(
		dnumbers &Numbers,
		dOperators &Operators )
	{
		return rnccmn::Handle<wnumber, dnumbers>( Numbers, Operators.Pop() );
	}

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
		rnccmn::SkipSpaces( Flow );

		Numbers.Init();
		Operators.Init();

		Number.Init();
		if ( !rnc::GetNumber( Flow, Number ) )
			qRReturn;
		Numbers.Push( Number );

		rnccmn::SkipSpaces( Flow );

		while ( !Flow.EndOfFlow() ) {
			Operator = Flow.Get();
			Operators.Push( Operator );

			rnccmn::SkipSpaces( Flow );

			Number.Init();
			if ( !rnc::GetNumber( Flow, Number ) )
				qRReturn;
			Numbers.Push( Number );

			rnccmn::SkipSpaces( Flow );

			if ( !Handle_<dnumber, wnumber, dnumbers>( Numbers, Operators ) )
				qRReturn;
		}

		if ( Numbers.Amount() != 1 )
			qRFwk();

		Numbers.Pop( Result );

		Success = true;
	qRR
	qRT
	qRE
		return Success;
	}

}

#endif
