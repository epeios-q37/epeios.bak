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
		xtf::sRFlow &Flow,
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

		if ( rnc::GetNumber( Flow, Number ) ) {
      Numbers.Push( Number );

      rnccmn::SkipSpaces( Flow );

      while ( !Flow.EndOfFlow() ) {
        if ( isdigit( Flow.View() ) ) {
          Number.Init();
          if ( !rnc::GetNumber( Flow, Number ) )
            break;
          Numbers.Push( Number );

          rnccmn::SkipSpaces( Flow );

        } else {
          Operator = Flow.Get();

          if ( !rnccmn::Handle<wnumber, dnumbers>( Numbers, Operator ) )
            break;

          rnccmn::SkipSpaces( Flow );
        }
      }

      if ( Flow.EndOfFlow() && ( Numbers.Amount() == 1 ) ) {
        Numbers.Pop( Result );

        Success = true;
      }
		}
	qRR
	qRT
	qRE
		return Success;
	}
}

#endif
