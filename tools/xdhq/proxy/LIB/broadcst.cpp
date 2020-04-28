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

#include "broadcst.h"

#include "lck.h"
#include "lstbch.h"
#include "lstcrt.h"
#include "str.h"

using namespace broadcst;

namespace {
    typedef sRow sDRow_; // Driver row.

    typedef lstbch::qLBUNCHdl(sDRow_) dDRows_;
    qW(DRows_);

    qROW(TRow_);    // Token row.

    qENUM(State_) {
        sAlive,
        sDead,
        s_amount,
        s_Undefined
    };

    struct rXDriver_
    {
    private:
        qRMV(fdr::rWDriver, D_, Driver_);
        tht::rLocker Locker_;
        void Lock_(void)
        {
            Locker_.Lock();
        }
        void Unlock_(void)
        {
            Locker_.Unlock();
        }
        sTRow_ TRow_;
    public:
        eState_ State;
        void reset(bso::sBool P = true)
        {
            Driver_ =  NULL;
            State = s_Undefined;
            Locker_.reset(P);
            TRow_ = qNIL;
        }
        qCDTOR(rXDriver_);
        void Init(
            fdr::rWDriver &Driver,
            sTRow_ TRow)
        {
            Driver_ = &Driver;
            State = sAlive;
            Locker_.Init();
            TRow_ = TRow;
        }
        sTRow_ Deactivate(void)
        {
        qRH
        qRB
            Lock_();

            State = sDead;
        qRR
        qRT
            Unlock_();
        qRE
            return TRow_;
        }
    };

    typedef lstbch::qLBUNCHw(rXDriver_ *, sDRow_) rXDrivers_;
    rXDrivers_ UnprotectedXDrivers_;
    lck::rTutor<rXDrivers_> XDrivers_;

    sDRow_ Store_(rXDriver_ *XDriver)
    {
        sDRow_ DRow = qNIL;
    qRH
        lck::rExclusiveAccess<rXDrivers_> XDrivers;
    qRB
        XDrivers.Init(XDrivers_);

        DRow = XDrivers().Add(XDriver);
    qRR
    qRT
    qRE
        return DRow;
    }

    // Also returns its token row.
    sTRow_ Deactivate_(sDRow_ DRow)
    {
        sTRow_ TRow = qNIL;
    qRH
        lck::rExclusiveAccess<rXDrivers_> XDrivers;
    qRB
        XDrivers.Init(XDrivers_);

        rXDriver_ &XDriver = *XDrivers()(DRow);

        XDrivers.Release();

        TRow = XDriver.Deactivate();
    qRR
    qRT
    qRE
        return TRow;
    }

    class rXDRows_
    {
    private:
        wDRows_ UnprotectedDRows_;
    public:
        str::wString Token;
        lck::rTutor<dDRows_> DRows;
        void reset(bso::sBool P = true)
        {
            tol::reset(P, UnprotectedDRows_, Token, DRows);
        }
        qCDTOR(rXDRows_);
        void Init(const str::dString &Token)
        {
            UnprotectedDRows_.Init();
            DRows.Init(UnprotectedDRows_);
            this->Token.Init(Token);
        }
    };

    typedef lstbch::qLBUNCHw(rXDRows_ *, sTRow_) rXDRowsSets_;
    rXDRowsSets_ UnprotectedXDRowsSets_;
    lck::rTutor<rXDRowsSets_> XDRowsSets_;

    rXDRows_ UntokenizedXDRows_;

    sTRow_ Search_(
        const str::dString &Token,
        const rXDRowsSets_ &XDRowsSets)
    {
        sTRow_ TRow = XDRowsSets.First();

        while( XDRowsSets(TRow)->Token != Token )
            TRow = XDRowsSets.Next(TRow);

        if ( TRow == qNIL )
            qRGnr();

        return TRow;
    }

    sTRow_ Search_(const str::dString &Token)
    {
        sTRow_ TRow = qNIL;
    qRH
        lck::rExclusiveAccess<rXDRowsSets_> XDRowsSets;
    qRB
        XDRowsSets.Init(XDRowsSets_);

        TRow = Search_(Token, XDRowsSets());
    qRR
    qRT
    qRE
        return TRow;
    }

    rXDRows_ &FetchXDRows_(sTRow_ TRow)
    {
        if ( TRow == qNIL )
            return UntokenizedXDRows_;
        else
        {
            rXDRows_ *XDRows = NULL;
        qRH
            lck::rExclusiveAccess<rXDRowsSets_> XDRowsSets;
        qRB
            XDRowsSets.Init(XDRowsSets_);

            XDRows = XDRowsSets()(TRow);
        qRR
        qRT
        qRE
            return *XDRows;
        }
    }
}

sRow broadcst::Add(
        fdr::rWDriver &Driver,
        const str::dString &Token)
{
    sDRow_ DRow = qNIL;
qRH
    sTRow_ TRow = qNIL;
    rXDriver_ *XDriver = NULL;
    lck::rExclusiveAccess<dDRows_> DRows;
qRB
    TRow = Search_(Token);

    if ( TRow == qNIL )
        qRGnr();

    XDriver = new rXDriver_;

    if ( XDriver == NULL )
        qRGnr();

    XDriver->Init(Driver, TRow);

    rXDRows_ &XDRows = FetchXDRows_(TRow);

    DRows.Init(XDRows.DRows);

    DRows().Add(DRow = Store_(XDriver));
qRR
    if ( XDriver != NULL )
        delete XDriver;
qRT
qRE
    return DRow;
}

void broadcst::Remove(sRow DRow)
{
qRH
    lck::rExclusiveAccess<dDRows_> DRows;
    sTRow_ TRow = qNIL;
qRB
    TRow = Deactivate_(DRow);

    rXDRows_ &XDRows = FetchXDRows_(TRow);

    DRows.Init(XDRows.DRows);




qRR
qRT
qRE
}
#endif
/*
void xdwmain::rUpstream::XDHCMNBroadcast(const str::dString &Id)
{
qRH
qRB
    qRVct();
qRR
qRT
qRE
}
*/


qGCTOR(broddcast)
{

}
