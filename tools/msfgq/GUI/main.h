/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'MSFGq' software.

  'MSFGq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'MSFGq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'MSFGq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAIN_INC_
# define MAIN_INC_

# include "sclx.h"

# include "melody.h"

# include "bso.h"
# include "tol.h"


	inline void reset(
		bso::sBool,
		mscmld::sRow &Row )
	{
		Row = qNIL;
	}

namespace main {
  class sSession
  : public sclx::sProxy
  {
  public:
    void reset(bso::sBool P = true)
    {
      sProxy::reset(P);
    }
    qCDTOR( sSession );
    void Init(
			xdhcuc::cSingle &Callback,
			const scli::sInfo &Info)
    {
      reset();

      sProxy::Init(Callback, Info, sclx::xfh_Default);
    }
  };

  extern sclx::action_handler<sSession> Core;


  struct rXMelody {
  public:
    melody::wMelody Melody;
    mscmld::sRow Row;
    bso::sU8 BaseOctave;
    bso::sBool Overwrite;
    void reset(bso::sBool P = true)
    {
      Melody.reset(P);
      Row = qNIL;
      BaseOctave = 0;
      Overwrite = false;
    }
    qCDTOR(rXMelody);
    void Init(void)
    {
      Melody.Init();
      Row = qNIL;
      BaseOctave = 2;
      Overwrite = false;
    }
  };

  typedef mtx::rHandle hGuard;

  rXMelody &Get(hGuard &Guard);
}

#endif // MAIN_INC_
