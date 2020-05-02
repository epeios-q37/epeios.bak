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
# include "xdhcmn.h"

namespace xdhbrd {
    qROW(CRow_);    // Callback row.
    qROW(TRow_); // Token row.

    typedef sCRow_ sRow;    // For user.

    qENUM(State_) {
        sAlive,
        sDead,
        s_amount,
        s_Undefined
    };

    typedef mtx::rHandler rMutex_;
    typedef mtx::rMutex hGuardian_;

    struct rXCallback
    {
    private:
        qRMV(xdhcmn::cUpstream, C_, Callback_);
        rMutex_ Mutex_;
        sTRow_ TRow_;
        eState_ State_;
    public:
        void reset(bso::sBool P = true);
        qCDTOR(rXCallback);
        void Init(
            xdhcmn::cUpstream &Callback,
            sTRow_ TRow);
        sTRow_ Deactivate(hGuardian_ &Guardian);
        void Send(
            const str::dString &Script,
            hGuardian_ &Guardian);
    };

    void Create(const str::dString &Token);

    sRow InitAndAdd(
        xdhcmn::cUpstream &Callback,
        rXCallback &XCallback,
        const str::dString &Token);

    void Broadcast(
        const str::dString &Script,
        const str::dString &Token);

    void Remove(sRow Row);
}

#endif
