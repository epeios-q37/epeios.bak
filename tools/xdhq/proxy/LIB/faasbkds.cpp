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
  typedef lstbch::qLBUNCHd( faasbckd::rBackend *, faasbckd::sRow ) dBackends_;
}

namespace backends_ {
  namespace {
    mtx::rMutex Access_ = mtx::Undefined;
    lstbch::qLBUNCHw(faasbckd::rBackend *, faasbckd::sRow) Backends_;
  }

  void CTor(void)
  {
    Access_ = mtx::Create();
    Backends_.Init();
  }

  void DTor(void)
  {
    if ( Access_ != mtx::Undefined )
      mtx::Delete( Access_, true );

    Access_ = mtx::Undefined;

    faasbckd::sRow Row = Backends_.First();

    while ( Row != qNIL ) {
      delete Backends_( Row );

      Row = Backends_.Next( Row );
    }

    Backends_.reset();
  }

  qMIMICr(mtx::rHandle, hGuard);

  inline dBackends_ &Get(hGuard &Guard)
  {
    Guard().InitAndLock(Access_);

    return Backends_;
  }

  inline void TestLocking(void)
  {
    if ( !mtx::IsLocked(Access_) )
      qRGnr();
  }
}

// Index thread safety is ensured with the locking of the 'backends_::Backends_'.
namespace index_ {
  namespace {
    idxbtq::qINDEXw( faasbckd::sRow ) Index_;
    faasbckd::sRow Root_ = qNIL;

    inline void TestLocking_(void) {
      backends_::TestLocking();
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
    TestLocking_();

    return TUSearch_(Token, Backends, Sign);
  }

  void Put(
    const str::dString &Token,
    const dBackends_ &Backends,
    faasbckd::sRow Row )
  {
    bso::sSign Sign = 0;
    faasbckd::sRow IRow = qNIL;

    TestLocking_();

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
  }

  void Delete(faasbckd::sRow Row)
  {
    TestLocking_();

    if ( Root_ == qNIL )
      qRGnr();

    Root_ = Index_.Delete(Row, Root_);

    if ( Root_ != qNIL )
      Root_ = Index_.Balance(Root_);
  }

  void CTor(void)
  {
    Root_ = qNIL;
    Index_.Init();
  }

  void DTor(void)
  {
    // Nothing to do.
  }
}

namespace backend_{
  faasbckd::sRow GetRow(
    const str::dString &Token,
    const dBackends_ &Backends)
  {
    bso::sSign Sign = 0;
    faasbckd::sRow Row = index_::Search(Token, Backends, Sign);

    if ( Sign != 0 )
      Row = qNIL;

    return Row;
  }

  namespace {
    faasbckd::rBackend *Get_(
      const str::dString &Token,
      const dBackends_ &Backends)
    {
      faasbckd::sRow Row = GetRow(Token, Backends);

      if ( Row != qNIL )
        if ( Backends(Row)->Driver != NULL )
          return Backends(Row);
        else
          return NULL;
      else
        return NULL;
    }
  }

  faasbckd::rBackend *TUGet(const str::dString &Token)
  {
    faasbckd::rBackend *Backend = NULL;
  qRH;
    backends_::hGuard Guard;
  qRB;
    Backend = Get_(Token, backends_::Get(Guard));
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
  faasbckd::rBackend *Backend = backend_::TUGet(Token);

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
  backends_::hGuard Guard;
qRB;
  dBackends_ &Backends = backends_::Get(Guard);

  Row = backend_::GetRow(Token, Backends);

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

  Backend = NULL;
qRT;
qRE;
  return Backend;
}

void faasbkds::Remove(faasbckd::sRow Row)
{
qRH;
  backends_::hGuard Guard;
qRB;
  dBackends_ &Backends = backends_::Get(Guard);

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
  backends_::CTor();
	index_::CTor();
qRFR;
qRFT;
qRFE(sclm::ErrorDefaultHandling());
}

qGDTOR(faasbkds)
{
  index_::DTor();
  backends_::DTor();
}


