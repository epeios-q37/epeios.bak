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

# include "faasbkds.h"

using namespace faasbkds;


namespace {
  idxbtq::qINDEXw( faasbckd::sRow ) Index_;
  faasbckd::sRow Root_ = qNIL;
  mtx::rMutex Access_ = mtx::Undefined;

  inline void TestAccess_(void) {
    if ( !mtx::IsLocked(Access_) )
      qRGnr();
  }
}

faasbckd::sRow index::Search(
  const str::dString &Token,
  bso::sSign &Sign)
  {
    faasbckd::sRow Row = Root_;
  qRH;
    faasbckd::sRow Candidate = qNIL;
    bso::sBool Continue = true;
    hGuard Guard;
  qRB;
    TestAccess_();

    dBackends &Backends = GetBackends(Guard);

    if ( Row == qNIL)
      return qNIL;

    while ( Continue ) {
      switch ( Sign = str::Compare(Token,Backends(Row)->Token) ) {
      case -1:
        Candidate = Index_.GetLesser(Row);

        if ( Candidate != qNIL )
          Row = Candidate;
        else
          Continue = false;

        break;
      case 0:
        Continue = false;
        break;
      case 1:
        Candidate = Index_.GetGreater(Row);

        if ( Candidate != qNIL )
          Row = Candidate;
        else
          Continue = false;

        break;
      default:
        qRUnx();
        break;
      }
    }
  qRR;
  qRT;
  qRE;
    return Row;
}

void index::Put(
  const str::dString &Token,
  faasbckd::sRow Row )
{
  bso::sSign Sign = 0;
  faasbckd::sRow IRow = Search(Token, Sign);  // Tests if access locked.

  if ( IRow == qNIL )
    Root_ = Row;
  else
    switch ( Sign ) {
    case -1:
      Index_.BecomeLesser(Row, IRow, Root_);
      break;
    case 0:
      qRGnr();
      break;
    case 1:
      Index_.BecomeGreater(Row, IRow, Root_);
      break;
    default:
      qRUnx();
      break;
  }
}

void index::Delete(faasbckd::sRow Row)
{
  TestAccess_();

  if ( Root_ == qNIL )
    qRGnr();

  Root_ = Index_.Delete(Row, Root_);

  if ( Root_ != qNIL )
    Root_ = Index_.Balance(Root_);

