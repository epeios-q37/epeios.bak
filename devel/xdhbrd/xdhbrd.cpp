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

#define XDHBRD_COMPILATION_

#include "xdhbrd.h"

#include "err.h"
#include "mtk.h"
#include "rgstry.h"

using namespace xdhbrd;

namespace {
    typedef sRow sCRow_;

    qCDEF(rMutex_, UndefinedMutex_, mtx::Undefined);

    void Hire_(
        hGuardian_ &Guardian,
        rMutex_ &Mutex )
    {
        Guardian.InitAndLock(Mutex);
    }

    void Dismiss_(hGuardian_ &Guardian)
    {
        Guardian.Unlock();
    }

    typedef lstbch::qLBUNCHdl(sCRow_) dCRows_;
    qW(CRows_);
}

void xdhbrd::rXCallback::reset(bso::sBool P)
{
    if ( P )
        if ( Mutex_ != UndefinedMutex_ )
            Mutex_ = mtx::Create();

    Mutex_ = UndefinedMutex_;
    Callback_ =  NULL;
    TRow_ = qNIL;
    State_ = s_Undefined;
}

void xdhbrd::rXCallback::Init(
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

sTRow_ xdhbrd::rXCallback::Deactivate(hGuardian_ &Guardian)
{
    Hire_(Guardian, Mutex_);

    State_ = sDead;

    return TRow_;
}

void xdhbrd::rXCallback::Send(
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
    rXCRows_ UntokenizedXCRows_;

    rXCRowsSets_ &GetXCRowsSets_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XCRowsSetMutex_);

        return UnprotectedXCRowsSets_;
    }

    sTRow_ Search_(
        const str::dString &Token,
        const rXCRowsSets_ &XCRowsSets,
        bso::sBool ReturnNotFound)
    {
        sTRow_ TRow = XCRowsSets.First();

        if ( Token.Amount() ) {
            while( ( TRow != qNIL ) && XCRowsSets(TRow)->Token != Token )
                TRow = XCRowsSets.Next(TRow);

            if ( ( TRow == qNIL ) && !ReturnNotFound )
                qRGnr();
        } else if ( TRow != qNIL )
            qRGnr();

        return TRow;
    }

    sTRow_ Search_(const str::dString &Token)
    {
        sTRow_ TRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        TRow = Search_(Token, GetXCRowsSets_(Guardian), false);
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

sTRow_ xdhbrd::Create(const str::dString &Token)
{
    sTRow_ TRow = qNIL;
qRH
    rXCRows_ *XCRows = NULL;
    hGuardian_ Guardian;
qRB
    XCRows = new rXCRows_;

    if ( XCRows == NULL )
        qRAlc();

    XCRows->Init(Token);

    rXCRowsSets_ &Sets = GetXCRowsSets_(Guardian);

    if ( Search_(Token, Sets, true) != qNIL )
        qRGnr();

    TRow = Sets.Add(XCRows);
qRR
    if ( XCRows != NULL )
        delete XCRows;
qRT
qRE
    return TRow;
}

sRow xdhbrd::InitAndAdd(
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

    XCallback.Init(Callback, TRow);

    FetchXCRows_(TRow).GetCRows(Guardian).Add(CRow = Store_(XCallback));
qRR
qRT
qRE
    return CRow;
}

namespace {
    namespace {
        struct sData_ {
        public:
            rXCallback *XCallback;
            const str::dString *Script;
        };

        void Routine_(
            void *UP,
            mtk::gBlocker &Blocker)
        {
            sData_ &Data = *(sData_ *)UP;
            rXCallback &XCallback = *Data.XCallback;
        qRH
            str::wString Script;
            hGuardian_ Guardian;
        qRB
            Script.Init(*Data.Script);

            Blocker.Release();

            XCallback.Send(Script, Guardian);
        qRR
        qRT
        qRE
        }

        void Broadcast_(
            const str::dString &Script,
            rXCallback &XCallback )
        {
            sData_ Data;

            Data.XCallback = &XCallback;
            Data.Script = &Script;

            mtk::Launch(Routine_, &Data);
        }
    }

    void Broadcast_(
        const str::dString &Script,
        const dCRows_ &CRows)
    {
        sdr::sRow Row = CRows.First();

        while ( Row != qNIL ) {
            Broadcast_(Script, FetchXCallback_(CRows(Row)));
            Row = CRows.Next(Row);
        }
    }
}

void xdhbrd::Broadcast(
    const str::dString &Script,
    const str::dString &Token)
{
qRH
    sTRow_ TRow = qNIL;
    hGuardian_ Guardian;
qRB
    TRow = Search_(Token);

    Broadcast_(Script, FetchXCRows_(TRow).GetCRows(Guardian));
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

void xdhbrd::Remove(sRow CRow)
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

qGCTOR(broadcst)
{
    UnprotectedXCallbacks_.Init();
    XCallbacksMutex_ = mtx::Create();

    UnprotectedXCRowsSets_.Init();
    XCRowsSetMutex_ = mtx::Create();

    UntokenizedXCRows_.Init(str::Empty);
}

qGDTOR(broadcst)
{
    if ( XCallbacksMutex_ != UndefinedMutex_ )
        mtx::Delete(XCallbacksMutex_);

    if ( XCRowsSetMutex_ != UndefinedMutex_ )
        mtx::Delete(XCRowsSetMutex_);
}
