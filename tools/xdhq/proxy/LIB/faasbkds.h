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

/*
	This is the new launcher which will replace the existing one.
	Both will exist together until switching from old to new one is achieved.
*/

// FAAS (FaaS) BacKenDS

#ifndef FAASBKDS_INC_
# define FAASBKDS_INC_

# include "faasbckd.h"

# include "idxbtq.h"

namespace faasbkds {
  qMIMICr(mtx::rHandle, hGuard);

  typedef lstbch::qLBUNCHd( faasbckd::rBackend *, faasbckd::sRow ) dBackends;

  dBackends &GetBackends(hGuard &Guard);

  namespace index {
    faasbckd::sRow Search(
      const str::dString &Token,
      const dBackends &Backends,
      bso::sSign &Sign);

    void Put(
      const str::dString &Token,
      const dBackends &Backends,
      faasbckd::sRow Row );

    void Delete(faasbckd::sRow Row);
  }
}

#endif
