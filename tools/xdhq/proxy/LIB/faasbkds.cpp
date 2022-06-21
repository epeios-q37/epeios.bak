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
  namespace index_ {
    mtx::rMutex Access = mtx::Undefined;
    idxbtq::qINDEXw( faasbckd::sRow ) Index;
    faasbckd::sRow Root = qNIL;
  }

  namespace backends_ {
    mtx::rMutex Access = mtx::Undefined;
    lstbch::qLBUNCHw(faasbckd::rBackend *, faasbckd::sRow) Backends;
  }
}

dBackends &faasbkds::GetBackends(hGuard &Guard)
{
  Guard().InitAndLock(backends_::Access);

  return backends_::Backends;
}

namespace {
  inline void TestAccess_(void) {
    if ( !mtx::IsLocked(index_::Access) )
      qRGnr();
  }

  faasbckd::sRow TUSearch_(
    const str::dString &Token,
    const dBackends &Backends,
    bso::sSign &Sign)
  {
    faasbckd::sRow
      Row = index_::Root,
      Candidate = qNIL;
    bso::sBool Continue = true;

    TestAccess_();

    if ( Row == qNIL )
      return qNIL;

    while ( Continue ) {
      switch ( Sign = str::Compare(Token,Backends(Row)->Token) ) {
      case -1:
        Candidate = index_::Index.GetLesser(Row);

        if ( Candidate != qNIL )
          Row = Candidate;
        else
          Continue = false;

        break;
      case 0:
        Continue = false;
        break;
      case 1:
        Candidate = index_::Index.GetGreater(Row);

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

    return Row;
  }
}

faasbckd::sRow index::Search(
  const str::dString &Token,
  const dBackends &Backends,
  bso::sSign &Sign)
{
  faasbckd::sRow Row = qNIL;
qRH;
  mtx::rHandle Guard;
qRB;
  Guard.InitAndLock(index_::Access);

  Row = TUSearch_(Token, Backends, Sign);
qRR;
qRT;
qRE;
  return Row;
}

void index::Put(
  const str::dString &Token,
  const dBackends &Backends,
  faasbckd::sRow Row )
{
qRH;
  bso::sSign Sign = 0;
  faasbckd::sRow IRow = qNIL;
  mtx::rHandle Guard;
qRB;
  Guard.InitAndLock(index_::Access);

  IRow = TUSearch_(Token, Backends, Sign);

  index_::Index.Allocate(Backends.Extent());

  if ( IRow == qNIL )
    index_::Root = Row;
  else
    switch ( Sign ) {
    case -1:
      index_::Index.BecomeLesser(Row, IRow, index_::Root);
      break;
    case 0:
      qRGnr();
      break;
    case 1:
      index_::Index.BecomeGreater(Row, IRow, index_::Root);
      break;
    default:
      qRUnx();
      break;
  }
qRR;
qRT;
qRE;
}

void index::Delete(faasbckd::sRow Row)
{
qRH;
  mtx::rHandle IndexGuard;
qRB;
  IndexGuard.InitAndLock(index_::Access);

  TestAccess_();

  if ( index_::Root == qNIL )
    qRGnr();

  index_::Root = index_::Index.Delete(Row, index_::Root);

  if ( index_::Root != qNIL )
    index_::Root = index_::Index.Balance(index_::Root);
qRR;
qRT;
qRE;
}

qGCTOR(faasbkds)
{
qRFH;
qRFB;
  backends_::Access = mtx::Create();
	backends_::Backends.Init();

	index_::Access = mtx::Create();
	index_::Index.Init();
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
}

qGDTOR(faasbkds)
{
	if ( index_::Access != mtx::Undefined )
		mtx::Delete( index_::Access, true );

	if ( backends_::Access != mtx::Undefined )
		mtx::Delete( backends_::Access, true );

	faasbckd::sRow Row = backends_::Backends.First();

	while ( Row != qNIL ) {
		delete backends_::Backends( Row );

		Row = backends_::Backends.Next( Row );
	}

  backends_::Backends.reset();
}


