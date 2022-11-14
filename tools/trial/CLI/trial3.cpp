/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'Trial' tool.

    'Trial' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'Trial' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'Trial'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "trial3.h"

#include "cio.h"
#include "lstcrt.h"


using namespace trial3;

namespace {
  qROW( CRow_ );

  typedef lstcrt::qLMCRATEd( str::dString, sCRow_ ) dCrate_;
  qW( Crate_ );

  qROW( BRow_ );

  typedef lstbch::qLBUNCHd( bso::sU8, sBRow_ ) dBunch_;
  qW(Bunch_);

  qHOOKS2(lstcrt::sHooks, Crate, lstbch::sHooks, Bunch);

  class dBundle_ {
  public:
    struct s {
      dCrate_::s Crate;
      dBunch_::s Bunch;
      bso::sU8 Value;
    } &S_;
    dCrate_ Crate;
    dBunch_ Bunch;
    dBundle_(s &S)
    : S_(S),
      Crate(S.Crate),
      Bunch(S.Bunch)
    {}
    void reset(bso::sBool P = true)
    {
      tol::reset(P, Crate, Bunch);
      S_.Value = 50;
    }
    void plug(sHooks &Hooks)
    {
      Crate.plug(Hooks.Crate_);
      Bunch.plug(Hooks.Bunch_);
    }
    void plug(qASd *AS)
    {
      Crate.plug(AS);
      Bunch.plug(AS);
    }
    dBundle_ &operator =(const dBundle_ &B)
    {
      Crate = B.Crate;
      Bunch = B.Bunch;

      S_.Value = B.S_.Value;

      return *this;
    }
    void Init(void)
    {
      tol::Init(Crate, Bunch);
      S_.Value = 100;
    }
  };

  qW( Bundle_ );

  uys::rFOH<sizeof(dBundle_::s)> FH_;
  ags::aggregated_storage AS_;
  wBundle_ Bundle_;

  bso::sBool Initialize_(void)
  {
    bso::sBool Exists = FH_.Init("T3", uys::mReadWrite).Boolean();

    AS_.plug(FH_);
    Bundle_.plug(&AS_);

    if ( !Exists ) {
      AS_.Init();
      Bundle_.Init();
    }

    return Exists;
  }

  void Flush_(void)
  {
    Bundle_.Crate.Flush();
    FH_.Flush();
  }
}

void trial3::Launch(void)
{
  if ( !Initialize_() ) {
    sCRow_ Row = Bundle_.Crate.New();
    Bundle_.Crate(Row) = "Short content.";
    Bundle_.Bunch.Add(25);
    Flush_();
    FH_.Put((const sdr::sByte *)&Bundle_.S_);
  } else {
    FH_.Get((sdr::sByte *)&Bundle_.S_);
    cio::COut << Bundle_.Crate(Bundle_.Crate.First()) << " : " << Bundle_.Bunch(Bundle_.Bunch.First()) << txf::nl << txf::commit;
    Bundle_.Crate(Bundle_.Crate.First()) = "Longer content.";
    cio::COut << Bundle_.Crate(Bundle_.Crate.First()) << " : " << Bundle_.Bunch(Bundle_.Bunch.First()) << txf::nl << txf::commit;
  }
}
