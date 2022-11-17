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


#include "trial5.h"

#include "cio.h"
#include "lstcrt.h"


using namespace trial5;

namespace {
  qROW( Row_ );

#ifdef LIST
  using lstcrt::sHooks;
  typedef lstcrt::qLMCRATEd( str::dString, sRow_ ) dCrate_;
#else
  using crt::sHooks;
  typedef crt::qMCRATEd( str::dString, sRow_ ) dCrate_;
#endif
  qW(Crate_);


  class dBundle_ {
  public:
    struct s {
      dCrate_::s Crate;
      bso::sU8 Value;
    } &S_;
    dCrate_ Crate;
    dBundle_(s &S)
    : S_(S),
      Crate(S.Crate)
    {}
    void reset(bso::sBool P = true)
    {
      tol::reset(P, Crate);
      S_.Value = 50;
    }
    void plug(sHooks &Hooks)
    {
      Crate.plug(Hooks);
    }
    void plug(qASd *AS)
    {
      Crate.plug(AS);
    }
    dBundle_ &operator =(const dBundle_ &B)
    {
      Crate = B.Crate;

      S_.Value = B.S_.Value;

      return *this;
    }
    void Init(void)
    {
      tol::Init(Crate);
      S_.Value = 100;
    }
  };

  qW( Bundle_ );

#define FILE
// #define OFFSET

#ifndef FILE
# define NO_FILE
#endif

#ifdef FILE
# ifdef OFFSET
  uys::rFOH<sizeof(dBundle_::s)> FH_;
# else
  uys::rFH FH_;
# endif
#endif

#ifndef NO_FILE
  ags::aggregated_storage AS_;
#endif
  wBundle_ Bundle_;


  bso::sBool Initialize_(void)
  {
#ifdef FILE
    bso::sBool Exists = FH_.Init("T5", uys::mReadWrite).Boolean();
#else
    bso::sBool Exists = false;
#endif

#ifdef FILE
    AS_.plug(FH_);
    Bundle_.plug(&AS_);
#endif

    if ( !Exists ) {
#ifdef FILE
      AS_.Init();
#endif
      Bundle_.Init();
    }

    return Exists;
  }

  void Flush_(void)
  {
#ifdef FILE
    FH_.Flush();
#endif
  }

  void Display_(void)
  {
    cio::COut << (bso::sUInt)Bundle_.S_.Value << " : " << Bundle_.Crate(Bundle_.Crate.First()) << txf::nl << txf::commit;
  }

  void Put_(void)
  {
#ifdef OFFSET
    FH_.Put((const sdr::sByte *)&Bundle_.S_);
#elif !defined( NO_FILES )
    flf::rWFlow Flow;

    Flow.Init("T5s.q37");

    Flow.Write(&Bundle_.S_, sizeof( Bundle_.S_));

    Flow.reset();
#endif
  }

  void Get_(void)
  {
#ifdef OFFSET
    FH_.Get((sdr::sByte *)&Bundle_.S_);
#elif !defined( NO_FILES )
    flf::rRFlow Flow;

    Flow.Init("T5s.q37");

    Flow.Read(sizeof( Bundle_.S_), &Bundle_.S_);

    Flow.reset();
#endif
  }
}

void trial5::Launch(void)
{
  if ( !Initialize_() ) {
    sRow_ Row = Bundle_.Crate.New();
    Bundle_.Crate(Row).Init("Short");
    Bundle_.S_.Value = 30;
    Flush_();
    Display_();
    Put_();
  } else {
    Get_();
    Display_();
    Bundle_.Crate(Bundle_.Crate.First()) = "Longer";
    Bundle_.S_.Value = 45;
    Display_();
    Flush_();
    Put_();
  }

  Bundle_.reset();
}
