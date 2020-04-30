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

#include "registry.h"

#include "lstbch.h"
#include "lstcrt.h"
#include "sclm.h"
#include "str.h"

using namespace broadcst;

namespace {
    qCDEF(rMutex_, UndefinedMutex_, mtx::Undefined);

    void Hire_(
        hGuardian_ &Guardian,
        rMutex_ &Mutex )
    {
        Guardian.Init(Mutex);

        Guardian.Lock();
    }

    void Dismiss_(hGuardian_ &Guardian)
    {
        Guardian.Unlock();
    }

    typedef lstbch::qLBUNCHdl(sCRow_) dCRows_;
    qW(CRows_);
}

void broadcst::rXCallback::reset(bso::sBool P)
{
    if ( P )
        if ( Mutex_ != UndefinedMutex_ )
            Mutex_ = mtx::Create();

    Callback_ =  NULL;
    TRow_ = qNIL;
    State_ = s_Undefined;
}

void broadcst::rXCallback::Init(
    xdhcmn::cUpstream &Callback,
    sTRow_ TRow)
{
    if ( Mutex_ != UndefinedMutex_ )
        mtx::Delete(Mutex_);

    Callback_ = &Callback;
    Mutex_ = mtx::Create();
    TRow_ = TRow;
    State_ = sAlive;
}

sTRow_ broadcst::rXCallback::Deactivate(hGuardian_ &Guardian)
{
    Hire_(Guardian, Mutex_);

    State_ = sDead;

    return TRow_;
}

void broadcst::rXCallback::Send(
    const str::dString &Script,
    hGuardian_ &Guardian)
{
    Hire_(Guardian, Mutex_);

    if ( State_ == sAlive )
        C_().Process(Script);
}

namespace {
    typedef lstbch::qLBUNCHw(rXCallback *, sCRow_) rXCallbacks_;
    rXCallbacks_ UnprotectedXCallbacks_;
    rMutex_ XCallbacksMutex_ = UndefinedMutex_;

    rXCallbacks_ &GetXCallbacks_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XCallbacksMutex_);

        return UnprotectedXCallbacks_;
    }

    sCRow_ Store_(rXCallback &XCallback)
    {
        sCRow_ CRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        CRow = GetXCallbacks_(Guardian).Add(&XCallback);
    qRR
    qRT
    qRE
        return CRow;
    }

    rXCallback &FetchXCallback_( sCRow_ CRow)
    {
        rXCallback *XCallback = NULL;
    qRH
        hGuardian_ Guardian;
    qRB
        XCallback = GetXCallbacks_(Guardian)(CRow);
    qRR
    qRT
    qRE
        return *XCallback;
    }

    // Also returns its token row.
    sTRow_ Deactivate_(sCRow_ CRow)
    {
        sTRow_ TRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        rXCallback &XCallback = *GetXCallbacks_(Guardian)(CRow);

        Dismiss_(Guardian);

        TRow = XCallback.Deactivate(Guardian);
    qRR
    qRT
    qRE
        return TRow;
    }

    class rXCRows_
    {
    private:
        wCRows_ UnprotectedCRows_;
        rMutex_ Mutex_;
    public:
        str::wString Token;
        void reset(bso::sBool P = true)
        {
            if ( P )
                if ( Mutex_ != UndefinedMutex_)
                    mtx::Delete(Mutex_);

            tol::reset(P, UnprotectedCRows_, Token);

            Mutex_ = UndefinedMutex_;
        }
        qCDTOR(rXCRows_);
        void Init(const str::dString &Token)
        {
            UnprotectedCRows_.Init();

            if ( Mutex_ != UndefinedMutex_ )
                mtx::Delete(UndefinedMutex_);

            Mutex_ = mtx::Create();

            this->Token.Init(Token);
        }
        wCRows_ &GetCRows(hGuardian_ &Guardian)
        {
            Hire_(Guardian, Mutex_);

            return UnprotectedCRows_;
        }
    };

    typedef lstbch::qLBUNCHw(rXCRows_ *, sTRow_) rXCRowsSets_;
    rXCRowsSets_ UnprotectedXCRowsSets_;
    rMutex_ XCRowsSetMutex_ = UndefinedMutex_;

    rXCRowsSets_ &GetXCRowsSets_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XCRowsSetMutex_);

        return UnprotectedXCRowsSets_;
    }

    rXCRows_ UntokenizedXCRows_;

    sTRow_ Search_(
        const str::dString &Token,
        const rXCRowsSets_ &XCRowsSets)
    {
        sTRow_ TRow = XCRowsSets.First();

        while( XCRowsSets(TRow)->Token != Token )
            TRow = XCRowsSets.Next(TRow);

        if ( TRow == qNIL )
            qRGnr();

        return TRow;
    }

    sTRow_ Search_(const str::dString &Token)
    {
        sTRow_ TRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        TRow = Search_(Token, GetXCRowsSets_(Guardian));
    qRR
    qRT
    qRE
        return TRow;
    }

    rXCRows_ &FetchXCRows_(sTRow_ TRow)
    {
        if ( TRow == qNIL )
            return UntokenizedXCRows_;
        else
        {
            rXCRows_ *XCRows = NULL;
        qRH
            hGuardian_ Guardian;
        qRB
            XCRows = GetXCRowsSets_(Guardian)(TRow);
        qRR
        qRT
        qRE
            return *XCRows;
        }
    }
}

sRow broadcst::InitAndAdd(
        xdhcmn::cUpstream &Callback,
        rXCallback &XCallback,
        const str::dString &Token)
{
    sCRow_ CRow = qNIL;
qRH
    sTRow_ TRow = qNIL;
    hGuardian_ Guardian;
qRB
    TRow = Search_(Token);

    if ( TRow == qNIL )
        qRGnr();

    XCallback.Init(Callback, TRow);

    FetchXCRows_(TRow).GetCRows(Guardian).Add(CRow = Store_(XCallback));
qRR
qRT
qRE
    return CRow;
}

namespace {
    namespace {
        void GetScript_(
            const str::dString &Message,
            str::dString &Script)
        {
        qRH
            str::wString RawScript;
        qRB
            RawScript.Init();
            sclm::MGetValue(registry::definition::BroadcastScript, RawScript);

            tagsbs::SubstituteLongTag(RawScript, Message, Script);
        qRR
        qRT
        qRE
        }

        struct sData_
        {
        public:
            const str::wString Script;

        };

        void Broadcast_(
            const str::dString &Message,
            rXCallback &XCallback )
        {
        qRH

        qRB

        qRR
        qRT
        qRE
        }
    }

    void Broadcast_(
        const str::dString &Message,
        const dCRows_ &CRows)
    {
        sdr::sRow Row = CRows.First();

        while ( Row != qNIL ) {
            Broadcast_(Message, FetchXCallback_(CRows(Row)));
            Row = CRows.Next(Row);
        }
    }
}

void broadcst::Broadcast(
    const str::dString &Message,
    const str::dString &Token)
{
qRH
    sTRow_ TRow = qNIL;
    hGuardian_ Guardian;
qRB
    TRow = Search_(Token);

    Broadcast_(Message, FetchXCRows_(TRow).GetCRows(Guardian));
qRR
qRT
qRE

}

namespace {
    void Remove_(
        sCRow_ CRow,
        dCRows_ &CRows)
    {
        sdr::sRow Row = CRows.First();

        while ( Row != qNIL ) {
            if ( CRows(Row) == CRow )
                break;
            Row = CRows.Next(Row);
        }

        if ( Row == qNIL )
            qRGnr();

        CRows.Delete(Row);
    }
}

void broadcst::Remove(sRow CRow)
{
qRH
    sTRow_ TRow = qNIL;
    hGuardian_ Guardian;
qRB
    TRow = Deactivate_(CRow);

    Remove_(CRow, FetchXCRows_(TRow).GetCRows(Guardian));
qRR
qRT
qRE
}
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


qGCTOR(broadcst)
{
    UnprotectedXCallbacks_.Init();
    XCallbacksMutex_ = mtx::Create();

    UnprotectedXCRowsSets_.Init();
    XCRowsSetMutex_ = mtx::Create();
}

qGDTOR(broadcst)
{
    if ( XCallbacksMutex_ != UndefinedMutex_ )
        mtx::Delete(XCallbacksMutex_);

    if ( XCRowsSetMutex_ != UndefinedMutex_ )
        mtx::Delete(XCRowsSetMutex_);
}
