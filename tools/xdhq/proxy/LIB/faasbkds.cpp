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
  namespace backends_ {
    mtx::rMutex Access = mtx::Undefined;
    lstbch::qLBUNCHw(faasbckd::rBackend *, faasbckd::sRow) Backends;
  }
}

namespace {
  qMIMICr(mtx::rHandle, hGuard_);

  typedef lstbch::qLBUNCHd( faasbckd::rBackend *, faasbckd::sRow ) dBackends_;

  dBackends_ &GetBackends_(hGuard_ &Guard)
  {
    Guard().InitAndLock(backends_::Access);

    return backends_::Backends;
  }
}

namespace index_ {
  namespace {
    mtx::rMutex Access_ = mtx::Undefined;
    idxbtq::qINDEXw( faasbckd::sRow ) Index_;
    faasbckd::sRow Root_ = qNIL;

    inline void TestAccess_(void) {
      if ( !mtx::IsLocked(Access_) )
        qRGnr();
    }

    faasbckd::sRow TUSearch_(
      const str::dString &Token,
      const dBackends_ &Backends,
      bso::sSign &Sign)
    {
      faasbckd::sRow
        Row = Root_,
        Candidate = qNIL;
      bso::sBool Continue = true;

      TestAccess_();

      if ( Row == qNIL )
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

      return Row;
    }
  }

  faasbckd::sRow Search(
    const str::dString &Token,
    const dBackends_ &Backends,
    bso::sSign &Sign)
  {
    faasbckd::sRow Row = qNIL;
  qRH;
    mtx::rHandle Guard;
  qRB;
    Guard.InitAndLock(Access_);

    Row = TUSearch_(Token, Backends, Sign);
  qRR;
  qRT;
  qRE;
    return Row;
  }

  void Put(
    const str::dString &Token,
    const dBackends_ &Backends,
    faasbckd::sRow Row )
  {
  qRH;
    bso::sSign Sign = 0;
    faasbckd::sRow IRow = qNIL;
    mtx::rHandle Guard;
  qRB;
    Guard.InitAndLock(Access_);

    IRow = TUSearch_(Token, Backends, Sign);

    Index_.Allocate(Backends.Extent());

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
  qRR;
  qRT;
  qRE;
  }

  void Delete(faasbckd::sRow Row)
  {
  qRH;
    mtx::rHandle IndexGuard;
  qRB;
    IndexGuard.InitAndLock(Access_);

    TestAccess_();

    if ( Root_ == qNIL )
      qRGnr();

    Root_ = Index_.Delete(Row, Root_);

    if ( Root_ != qNIL )
      Root_ = Index_.Balance(Root_);
  qRR;
  qRT;
  qRE;
  }

  void CTor(void)
  {
    Access_ = mtx::Create();
    Root_ = qNIL;
    Index_.Init();
  }

  void DTor(void)
  {
    if (Access_ != mtx::Undefined )
      mtx::Delete(Access_, true);

    Access_ = mtx::Undefined;
  }
}

namespace {
  namespace {
    faasbckd::sRow GetBackendRow_(
      const str::dString &Token,
      const dBackends_ &Backends)
    {
      bso::sSign Sign = 0;
      faasbckd::sRow Row = index_::Search(Token, Backends, Sign);

      if ( Sign != 0 )
        Row = qNIL;

      return Row;
    }

    faasbckd::rBackend *GetBackend_(
      const str::dString &Token,
      const dBackends_ &Backends)
    {
      faasbckd::sRow Row = GetBackendRow_(Token, Backends);

      if ( Row != qNIL )
        if ( Backends(Row)->Driver != NULL )
          return Backends(Row);
        else
          return NULL;
      else
        return NULL;
    }
  }

  faasbckd::rBackend *GetBackend_(
    const str::dString &Token,
    faasbckd::hGuard &Guard)
  {
    faasbckd::rBackend *Backend = NULL;
  qRH;
    hGuard_ BackendsGuard;
  qRB;
    Backend = GetBackend_(Token, GetBackends_(BackendsGuard));
  qRR;
  qRT;
  qRE;
    return Backend;
  }
}

faasbckd::rBackend *faasbkds::GetBackend(
  const str::dString &Token,
  faasbckd::hGuard &Guard)
{
  faasbckd::rBackend *Backend = GetBackend_(Token, Guard);

  if ( Backend != NULL )
    Backend->Lock(Guard);

  return Backend;
}

faasbckd::rBackend *faasbkds::Create(
  const str::dString &Token,
  faasbckd::sRow &Row)
{
  faasbckd::rBackend *Backend = NULL;
qRH;
  hGuard_ Guard;
qRB;
  dBackends_ &Backends = GetBackends_(Guard);

  Row = GetBackendRow_(Token, Backends);

  if ( Row == qNIL ) {
    Row = Backends.New();

    if ( (Backend = new faasbckd::rBackend) == NULL )
      qRAlc();

    Backends.Store(Backend, Row);

    index_::Put(Token, Backends, Row);
  }
qRR;
  if ( Backend != NULL )
    delete Backend;
qRT;
qRE;
  return Backend;
}

void faasbkds::Remove(faasbckd::sRow Row)
{
qRH;
  hGuard_ Guard;
qRB;
  dBackends_ &Backends = GetBackends_(Guard);

  if ( !Backends.Exists( Row ) )
    qRGnr();

  Backends.Remove( Row );

  index_::Delete(Row);
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

	index_::CTor();
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
}

qGDTOR(faasbkds)
{
  index_::DTor();

	if ( backends_::Access != mtx::Undefined )
		mtx::Delete( backends_::Access, true );

	faasbckd::sRow Row = backends_::Backends.First();

	while ( Row != qNIL ) {
		delete backends_::Backends( Row );

		Row = backends_::Backends.Next( Row );
	}

  backends_::Backends.reset();
}


