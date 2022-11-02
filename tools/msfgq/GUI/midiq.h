/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'mscfdraftq' tool.

    'mscfdraftq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mscfdraftq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mscfdraftq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MIDIQ_INC_
# define MIDIQ_INC_

# include "mscmdd.h"
# include "mscmld.h"

# include "mtk.h"

namespace midiq {
  struct sShared {
    mscmld::dMelody *Melody;
    mscmld::sRow Row;
    mscmdd::rRFlow *RFlow;
    str::dString *MIDIDeviceIn;
    sShared( void )
    {
      Melody = NULL;
      Row = qNIL;
      RFlow = NULL;
    }
  };

  void HandleInput(
    void *UP,
    mtk::gBlocker &Blocker);

  void Play(
    const mscmld::sNote &Note,
    bso::sU32 Base,
    bso::sBool Tied,
    flw::rWFlow &Flow );
}

#endif
