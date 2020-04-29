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

#include "lstbch.h"
#include "lstcrt.h"
#include "str.h"

using namespace broadcst;

namespace {
    typedef mtx::rHandler rMutex_;
    typedef mtx::rMutex hGuardian_;

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
        rMutex_ Mutex_;
        sTRow_ TRow_;
        eState_ State_;
    public:
        void reset(bso::sBool P = true)
        {
            if ( P )
                if ( Mutex_ != UndefinedMutex_ )
                    Mutex_ = mtx::Create();

            Driver_ =  NULL;
            TRow_ = qNIL;
            State_ = s_Undefined;
        }
        qCDTOR(rXDriver_);
        void Init(
            fdr::rWDriver &Driver,
            sTRow_ TRow)
        {
            if ( Mutex_ != UndefinedMutex_ )
                mtx::Delete(Mutex_);

            Driver_ = &Driver;
            Mutex_ = mtx::Create();
            TRow_ = TRow;
            State_ = sAlive;
        }
        sTRow_ Deactivate(hGuardian_ &Guardian)
        {
            Hire_(Guardian, Mutex_);

            State_ = sDead;

            return TRow_;
        }
        void Send(
            const bso::sChar *Script,
            hGuardian_ &Guardian)
        {
            Hire_(Guardian, Mutex_);

            if ( State_ == sAlive )
            {
            qRH
                flw::rDressedWFlow<> Flow;
                qCBUFFERh Buffer;
            qRB
                Flow.Init(D_());

                flw::PutString(Script, Flow);

                Flow.Commit();
            qRR
            qRT
            qRE
            }
        }
    };

    typedef lstbch::qLBUNCHw(rXDriver_ *, sDRow_) rXDrivers_;
    rXDrivers_ UnprotectedXDrivers_;
    rMutex_ XDriversMutex_ = UndefinedMutex_;

    rXDrivers_ &GetXDrivers_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XDriversMutex_);

        return UnprotectedXDrivers_;
    }

    sDRow_ Store_(rXDriver_ *XDriver)
    {
        sDRow_ DRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        DRow = GetXDrivers_(Guardian).Add(XDriver);
    qRR
    qRT
    qRE
        return DRow;
    }

    rXDriver_ &FetchXDriver_(sDRow_ DRow)
    {
        rXDriver_ *XDriver = NULL;
    qRH
        hGuardian_ Guardian;
    qRB
        XDriver = GetXDrivers_(Guardian)(DRow);
    qRR
    qRT
    qRE
        return *XDriver;
    }

    // Also returns its token row.
    sTRow_ Deactivate_(sDRow_ DRow)
    {
        sTRow_ TRow = qNIL;
    qRH
        hGuardian_ Guardian;
    qRB
        rXDriver_ &XDriver = *GetXDrivers_(Guardian)(DRow);

        Dismiss_(Guardian);

        TRow = XDriver.Deactivate(Guardian);
    qRR
    qRT
    qRE
        return TRow;
    }

    class rXDRows_
    {
    private:
        wDRows_ UnprotectedDRows_;
        rMutex_ Mutex_;
    public:
        str::wString Token;
        void reset(bso::sBool P = true)
        {
            if ( P )
                if ( Mutex_ != UndefinedMutex_)
                    mtx::Delete(Mutex_);

            tol::reset(P, UnprotectedDRows_, Token);

            Mutex_ = UndefinedMutex_;
        }
        qCDTOR(rXDRows_);
        void Init(const str::dString &Token)
        {
            UnprotectedDRows_.Init();

            if ( Mutex_ != UndefinedMutex_ )
                mtx::Delete(UndefinedMutex_);

            Mutex_ = mtx::Create();

            this->Token.Init(Token);
        }
        wDRows_ &GetDRows(hGuardian_ &Guardian)
        {
            Hire_(Guardian, Mutex_);

            return UnprotectedDRows_;
        }
    };

    typedef lstbch::qLBUNCHw(rXDRows_ *, sTRow_) rXDRowsSets_;
    rXDRowsSets_ UnprotectedXDRowsSets_;
    rMutex_ XDRowsSetMutex_ = UndefinedMutex_;

    rXDRowsSets_ &GetXDRowsSets_(hGuardian_ &Guardian)
    {
        Hire_(Guardian, XDRowsSetMutex_);

        return UnprotectedXDRowsSets_;
    }

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
        hGuardian_ Guardian;
    qRB
        TRow = Search_(Token, GetXDRowsSets_(Guardian));
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
            hGuardian_ Guardian;
        qRB
            XDRows = GetXDRowsSets_(Guardian)(TRow);
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
    hGuardian_ Guardian;
qRB
    TRow = Search_(Token);

    if ( TRow == qNIL )
        qRGnr();

    XDriver = new rXDriver_;

    if ( XDriver == NULL )
        qRGnr();

    XDriver->Init(Driver, TRow);

    rXDRows_ &XDRows = FetchXDRows_(TRow);

    XDRows.GetDRows(Guardian).Add(DRow = Store_(XDriver));
qRR
    if ( XDriver != NULL )
        delete XDriver;
qRT
qRE
    return DRow;
}

namespace {
    namespace {
        namespace {
            void GetScript_(
                const str::dString &Message,
                str::dString &Script)
        {
        qRH
            str::dString RawScript;
        qRB
            RawScript.Init();
            sclm::MGetValue(registry::definition::BroadcastScript);

            tagsbs::SubstituteLongTag(RawString, Message, Script);
        qRR
        qRT
        qRE
        }

        struct sData_

        void Routine

        void Broadcast_(
            const str::dString &Message,
            rXDriver_ &XDriver )
        {
        qRH
            str::dString RawScript, Script;
        qRB
            RawScript.Init();
            sclm::MGetValue(registry::definition::BroadcastScript);

            tagsbs::SubstituteLongTag(RawString, Message, Script);
        qRR
        qRT
        qRE
        }
        {
        }
    }

    void Broadcast_(
        const str::dString &Message,
        const dDRows_ &DRows)
    {
        sdr::sRow Row = DRows.First();

        while ( Row != qNIL ) {
            Broadcast_(Message, FetchXDriver_(DRows(Row)));
            Row = DRows.Next(Row);
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

    Broadcast_( Message, FetchXDRows_(TRow).GetDRows(Guardian) );
qRR
qRT
qRE

}

namespace {
    void Remove_(
        sDRow_ DRow,
        dDRows_ &DRows)
    {
        sdr::sRow Row = DRows.First();

        while ( Row != qNIL ) {
            if ( DRows(Row) == DRow )
                break;
            Row = DRows.Next(Row);
        }

        if ( Row == qNIL )
            qRGnr();

        DRows.Delete(Row);
    }
}

void broadcst::Remove(sRow DRow)
{
qRH
    sTRow_ TRow = qNIL;
    hGuardian_ Guardian;
qRB
    TRow = Deactivate_(DRow);

    Remove_( DRow, FetchXDRows_(TRow).GetDRows(Guardian) );
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
    UnprotectedXDrivers_.Init();
    XDriversMutex_ = mtx::Create();

    UnprotectedXDRowsSets_.Init();
    XDRowsSetMutex_ = mtx::Create();
}

qGDTOR(broadcst)
{
    if ( XDriversMutex_ != UndefinedMutex_ )
        mtx::Delete(XDriversMutex_);

    if ( XDRowsSetMutex_ != UndefinedMutex_ )
        mtx::Delete(XDRowsSetMutex_);
}
