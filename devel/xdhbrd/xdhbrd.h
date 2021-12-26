/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// XDH(TML) BRoaDcasting

#ifndef XDHBRD_INC_
# define XDHBRD_INC_

# define XDHBRD_NAME		"XDHBRD"

# if defined( E_DEBUG ) && !defined( XDHBRD_NODBG )
#  define XDHBRD_DBG
# endif

# include "fdr.h"
# include "sdr.h"
# include "xdhcuc.h"

namespace xdhbrd {
    qROW(CRow); // Callback row.
    typedef xdhcmn::faas::sRow sTRow_; // Token row.

    qENUM(State_) {
        sAlive,
        sIdle,
        s_amount,
        s_Undefined
    };

    typedef mtx::rMutex rMutex_;
    typedef mtx::rHandle hGuardian_;

    struct rXCallback
    {
    private:
        qRMV(xdhcuc::cSingle, C_, Callback_);
        rMutex_ Mutex_;
        sTRow_ TRow_;
        sCRow CRow_;
        eState_ State_;
        // Returns 'false' if 'Token' does not exist and 'ReturnNotFound' at 'true'.
        sCRow AddTo_(
            const str::dString &Token,
            bso::sBool ReturnNotFound);
        void Deactivate_(hGuardian_ &Guardian);
        void Remove_(void);
        bso::sBool Send_(
          const str::dString &Primitive,
          const str::dStrings &TagValues,
          hGuardian_ &Guardian);
    public:
        void reset(bso::sBool P = true);
        qCDTOR(rXCallback);
        // Returns 'false' if 'Token' does nor exist.
        sCRow Init(
            xdhcuc::cSingle &Callback,
            const str::dString &Token);
        friend bso::sBool Send_(
          rXCallback &Callback,
          const str::dString &Primitive,
          const str::dStrings &TagValues,
          hGuardian_ &Guardian);
    };

    inline bso::sBool Send_(
      rXCallback &Callback,
      const str::dString &Primitive,
      const str::dStrings &TagValues,
      hGuardian_ &Guardian)
      {
        return Callback.Send_(Primitive, TagValues, Guardian);
      }

    xdhcmn::faas::sRow Create(const str::dString &Token);

    void Broadcast(
      const str::dString &Primitive,
      const str::dStrings &TagValues,
      xdhcmn::faas::sRow TRow);

    void Remove(xdhcmn::faas::sRow TRow);
}

#endif
