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

// SKeTch

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
    qROW(CRow_); // Callback row.
    typedef xdhcmn::faas::sRow sTRow_; // Token row.

    qENUM(State_) {
        sAlive,
        sDead,
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
        sCRow_ CRow_;
        eState_ State_;
        // Returns 'false' if 'Token' does not exist and 'ReturnNotFound' at 'true'.
        bso::sBool Add_(
            const str::dString &Token,
            bso::sBool ReturnNotFound);
        void Deactivate_(hGuardian_ &Guardian);
        void Remove_(void);
    public:
        void reset(bso::sBool P = true);
        qCDTOR(rXCallback);
        // Returns 'false' if 'Token' does nor exist.
        bso::sBool Init(
            xdhcuc::cSingle &Callback,
            const str::dString &Token);
        bso::sBool Send(
            const str::dString &Script,
            hGuardian_ &Guardian);
    };

    sTRow_ Create(const str::dString &Token);

    void Remove(sTRow_ TRow);

    void Broadcast(
        const str::dString &Script,
        sTRow_ TRow);
}

#endif
