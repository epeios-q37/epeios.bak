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
    public:
        eState_ State;
        sTRow_ Row;
        void reset(bso::sBool P = true)
        {
            Driver_ =  NULL;
            State = s_Undefined;
            Locker_.reset(P);
            Row = qNIL;
        }
        qCDTOR(rXDriver_);
        void Init(
            fdr::rWDriver &Driver,
            sTRow_ Row)
        {
            Driver_ = &Driver;
            State = sAlive;
            Locker_.Init();
            this->Row = Row;
        }
    };

    typedef lstbch::qLBUNCHw(rXDriver_ *, sDRow_) rXDrivers_;
    rXDrivers_ UnprotectedXDrivers_;
    lck::rTutor<rXDrivers_> XDrivers_;

    class rXDRows_
    {
    private:
        wDRows_ UnprotectedDRows_;
    public:
        str::wString Token;
        lck::rTutor<dDRows_> Tutor;
        void reset(bso::sBool P = true)
        {
            tol::reset(P, UnprotectedDRows_, Token, Tutor);
        }
        qCDTOR(rXDRows_);
        void Init(const str::dString &Token)
        {
            UnprotectedDRows_.Init();
            Tutor.Init(UnprotectedDRows_);
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

    rXDRows_ &FetchDRows_(sTRow_ TRow)
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

    rXDRows_ &FetchDRows_(const str::dString &Token)
    {
        return FetchDRows_(Token.Amount() == 0 ? qNIL : Search_(Token));
    }
}

sRow broadcst::Add(
        fdr::rWDriver &Driver,
        const str::dString &Token)
{
    sDRow_ DRow = qNIL;
qRH
    lck::rExclusiveAccess<wDRows_> DRows;
qRB
    XDriver = new rXDriver_;

    if ( XDriver == NULL )
        qRGnr();

    DRows.Init();

    TRow = FetchDRows_(Token, DRows);
    Locked = true;

    XDriver->Init(Driver, TRow);

    DLocker_.Lock();
    DLocked = true;
    DRow = XDrivers_.Add(XDriver);
    DLocker_.Unlock();



    DRows.Add(DRow);

    ReleaseDRows_(TRow, DRows);
    Locked = false;
qRR
    if ( XDriver != NULL )
        delete XDriver;
qRT
    if ( Locked )
        ReleaseDRows_(TRow);
qRE
    return DRow;
}

void broadcst::Remove(sRow Row)
{
qRH
    wDRows_ DRows;
    rXDriver_ *XDriver = NULL;
    sTRow_ TRow = qNIL;
    bso::sBool DLocked = false, TLocked = false;
qRB
    XDriver = XDrivers_(Row);

    XDriver->Lock();
    DLocked = true;

    XDriver->State = sDead;
    TRow = XDriver->Row;

    XDriver->Unlock();
    DLocked = false;

    DRows.Init();
    FetchDRows_(TRow, Rows);
    TLocked = true;

    Rows.Remove(Row);

    ReleaseDRows_(TRow, DRows);
    TLock = false;

    DRows_.Remove(Row);
qRR
    if ( XDriver != NULL )
        delete XDriver;
qRT
    if ( DLock )
        if ( XDriver != NULL )
            XDriver->Unlock();

    if ( TLock )
        ReleaseDRows_(TRow);
qRE
    return Row;
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
