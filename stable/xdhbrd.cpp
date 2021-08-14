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
    qCDEF(rMutex_, UndefinedMutex_, mtx::Undefined);

    void Hire_(
        hGuardian_ &Guardian,
        rMutex_ &Mutex )
    {
        Guardian.InitAndLock(Mutex);
    }

    typedef lstbch::qLBUNCHdl( sCRow ) dCRows_;
    qW(CRows_);

    typedef lstbch::qLBUNCHw( rXCallback *, sCRow ) rXCallbacks_;
    rXCallbacks_ UnprotectedXCallbacks_;
    rMutex_ XCallbacksMutex_ = UndefinedMutex_;

    rXCallbacks_ &GetXCallbacks_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XCallbacksMutex_);

        return UnprotectedXCallbacks_;
    }

    sCRow Store_(rXCallback &XCallback)
    {
        sCRow CRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        CRow = GetXCallbacks_(Guardian).Add(&XCallback);
    qRR
    qRT
    qRE
        return CRow;
    }

    rXCallback *FetchXCallback_(
      sCRow CRow,
      hGuardian_ &Guardian)
    {
      return GetXCallbacks_(Guardian)(CRow);
    }

    rXCallback &FetchXCallback_(sCRow CRow)
    {
        rXCallback *XCallback = NULL;
    qRH
        hGuardian_ Guardian;
    qRB
        XCallback = FetchXCallback_(CRow, Guardian);
    qRR
    qRT
    qRE
        return *XCallback;
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
            while( ( TRow != qNIL ) && ( XCRowsSets(TRow)->Token != Token ) )
                TRow = XCRowsSets.Next(TRow);

            if ( ( TRow == qNIL ) && !ReturnNotFound )
                qRGnr();
        } else if ( TRow != qNIL )
            qRGnr();

        return TRow;
    }

    sTRow_ Search_(
        const str::dString &Token,
        bso::sBool ReturnNotFound)
    {
        sTRow_ TRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        TRow = Search_(Token, GetXCRowsSets_(Guardian), ReturnNotFound);
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

namespace {
    void Remove_(
        sCRow CRow,
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

void xdhbrd::rXCallback::Remove_(void)
{
qRH
    hGuardian_ Guardian;
qRB
    Deactivate_(Guardian);

    ::Remove_(CRow_, FetchXCRows_(TRow_).GetCRows(Guardian));
qRR
qRT
qRE
}

void xdhbrd::rXCallback::reset(bso::sBool P)
{
    if ( P ) {
        if ( CRow_ != qNIL )
            Remove_();

        if ( Mutex_ != UndefinedMutex_ )
            mtx::Delete(Mutex_);
    }

    Mutex_ = UndefinedMutex_;
    Callback_ =  NULL;
    TRow_ = qNIL;
    CRow_ = qNIL;
    State_ = s_Undefined;
}

void xdhbrd::rXCallback::Deactivate_(hGuardian_ &Guardian)
{
    Hire_(Guardian, Mutex_);

    State_ = sIdle;
}

sCRow xdhbrd::rXCallback::AddTo_(
    const str::dString &Token,
    bso::sBool ReturnNotFound)
{
    bso::sBool Success = true;
qRH
    hGuardian_ Guardian;
qRB
    if ( ( TRow_ = Search_(Token, ReturnNotFound) ) == qNIL ) {
        if ( Token.Amount() == 0 ) {
            if ( GetXCRowsSets_(Guardian).Amount() != 0 )
                qRGnr();
        } else
            Success = false;
    }

    if ( Success )
      FetchXCRows_(TRow_).GetCRows(Guardian).Add(CRow_ = Store_(*this));
    else
      CRow_ = qNIL;
qRR
qRE
qRT
    return CRow_;
}

sCRow xdhbrd::rXCallback::Init(
    xdhcuc::cSingle &Callback,
    const str::dString &Token)
{
    if ( Mutex_ != UndefinedMutex_ )
        mtx::Delete(Mutex_);

    if ( CRow_ != qNIL )
        Remove_();

    Callback_ = &Callback;
    Mutex_ = mtx::Create();
    State_ = sAlive;

    return AddTo_(Token, true);
}

bso::sBool xdhbrd::rXCallback::Send_(
    const str::dString &Script,
    hGuardian_ &Guardian)
{
    Hire_(Guardian, Mutex_);

    if ( State_ == sAlive )
        return C_().Process(Script);
    else
        return false;
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

            Send_(XCallback, Script, Guardian);
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
   sTRow_ TRow)
{
qRH
    hGuardian_ Guardian;
qRB
    Broadcast_(Script, FetchXCRows_(TRow).GetCRows(Guardian));
qRR
qRT
qRE
}

void xdhbrd::Remove(sTRow_ TRow)
{
qRH
    rXCRows_ *XCRows = NULL;
    hGuardian_ Guardian;
qRB
    rXCRowsSets_ &Sets = GetXCRowsSets_(Guardian);

    XCRows = Sets(TRow);

    Sets.Remove(TRow);

    if ( XCRows == NULL )
        qRGnr();

    delete XCRows;
qRR
qRT
qRE
}

qGCTOR(xdhbrd)
{
    UnprotectedXCallbacks_.Init();
    XCallbacksMutex_ = mtx::Create();

    UnprotectedXCRowsSets_.Init();
    XCRowsSetMutex_ = mtx::Create();

    UntokenizedXCRows_.Init(str::Empty);
}

qGDTOR(xdhbrd)
{
    if ( XCallbacksMutex_ != UndefinedMutex_ )
        mtx::Delete(XCallbacksMutex_);

    if ( XCRowsSetMutex_ != UndefinedMutex_ )
        mtx::Delete(XCRowsSetMutex_);
}
