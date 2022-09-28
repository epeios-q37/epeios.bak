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


#ifndef MELODY_INC_
# define MELODY_INC_

#include "mscmld.h"

namespace melody {
  using mscmld::sRow;
  using mscmld::dMelody;
  using mscmld::wMelody;
  using mscmld::sSignatureTime;
  using mscmld::sSignatureKey;
  using mscmld::sSignature;
  using mscmld::sNote;
  using mscmld::sAltPitch;
  using mscmld::sDuration;
  using mscmld::aFlat;
  using mscmld::aSharp;
  using mscmld::aNatural;
  using mscmld::pnRest;

  struct sTempo {
    bso::sU8 Value;
    mscmld::sDuration Unit;
    sTempo( void )
    {
      // By deafault, quarter note = 120.
      Value = 120;
      Unit = mscmld::sDuration(4, 0);
    }
  };

  sSignatureKey GetSignatureKey( void );

  sSignatureTime GetSignatureTime( void );

  sSignature GetSignature(void);

  struct rXMelody {
  public:
    melody::wMelody Melody;
    mscmld::sRow Row;
    bso::sU8 BaseOctave;
    mscmld::eAccidental Accidental; // Accidental to use for alterate notes when in C key.
    bso::sBool Overwrite;
    void reset(bso::sBool P = true)
    {
      Melody.reset(P);
      Row = qNIL;
      BaseOctave = 0;
      Accidental = mscmld::a_Undefined;
      Overwrite = false;
    }
    qCDTOR(rXMelody);
    void Init(void)
    {
      Melody.Init();
      Row = qNIL;
      BaseOctave = 2;
      Accidental = mscmld::aSharp;
      Overwrite = false;
    }
  };

  typedef mtx::rHandle hGuard;

  rXMelody &Get(hGuard &Guard);

  bso::sS8 Handle(
    sNote Note,
    rXMelody &XMelody);

  bso::sS8 Handle(const sNote &Note);

  void Play(
    const dMelody &Melody,
    const sTempo &Tempo,
    flw::oflow__ &Flow);
}

#endif
