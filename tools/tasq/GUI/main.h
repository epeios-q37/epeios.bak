/*
  Copyright (C) 2022 Claude SIMON (http://zeusw.org/epeios/contact.html).

  This file is part of 'TASq' software.

  'TASq' is free software: you can redistribute it and/or modify it
  under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  'TASq' is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with 'TASq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAIN_INC_
# define MAIN_INC_

# include "sclx.h"

# include "bso.h"
# include "tol.h"

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
      sProxy::Init(Callback, Info, sclx::xfh_Default);
    }
  };

  extern sclx::action_handler<sSession> Core;
}


#endif // MAIN_INC_
