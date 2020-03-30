/*
	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'FaaSq' tool.

    'FaaSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'FaaSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'FaaSq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SESSION_INC_
# define SESSION_INC_

# include "xdhcmn.h"
# include "xdhups.h"

namespace session {
    typedef bso::sU8 sId;
    qCDEF(sId, UndefinedId, 255);

    typedef xdhcmn::cUpstream cUpstream_;

    class rBlockers_
    {
    private:
        qRMV(tht::rBlocker, G_, Global_);
        tht::rBlocker Self_;
    public:
        void reset(bso::sBool P = true)
        {
            tol::reset(P,Global_, Self_);
        }
        qCDTOR(rBlockers_);
        void Init(tht::rBlocker &Global)
        {
            Global_ = &Global;
            Self_.Init();
        }
        void WaitSelf(void)
        {
            Self_.Wait();
        }
        void UnblockSelf(void)
        {
            Self_.Unblock();
        }
        void UnblockGlobal(void)
        {
            G_().Unblock();
        }
    };

    class sUpstream_
    : public cUpstream_
    {
    private:
        qRMV(rBlockers_, B_, Blockers_);
        qRMV(flw::rRWFlow,P_, Proxy_);
        sId Id_;
    protected:
        virtual void XDHCMNProcess(
            const str::string_ &Script,
            str::dString *ReturnedValue ) override;
     public:
        void reset( bso::bool__ P = true )
        {
            Blockers_ = NULL;
            Proxy_ = NULL;
            Id_ = UndefinedId;
        }
        E_CVDTOR( sUpstream_ );
        void Init(
            flw::rRWFlow &Proxy,
            sId Id,
            rBlockers_ &Blockers)
        {
            Blockers_ = &Blockers;
            Proxy_ = &Proxy;
            Id_ = Id;
        }
    };

    class rSession
    {
    private:
        flw::rDressedRWFlow<> Proxy_;
        sUpstream_ Upstream_;
        xdhups::sSession Session_;
        rBlockers_ Blockers_;
        sId Id_;
    public:
        bso::sBool Handshaked;
        void reset(bso::sBool P = true)
        {
            tol::reset(P, Proxy_, Upstream_, Session_, Blockers_);
            Handshaked = false;
            Id_ = UndefinedId;
        }
        qCDTOR(rSession);
        void Init(
            sId Id,
            xdhcmn::cSession *Session,
            fdr::rRWDriver &ProxyDriver,
            tht::rBlocker &Global)
        {
            Proxy_.Init(ProxyDriver);
            Session_.Init(Session);
            Upstream_.Init(Proxy_, Id, Blockers_);
            Session_.Initialize(Upstream_,"",str::wString(""));
            Handshaked = false;
            Blockers_.Init(Global);
            Id_ = Id;
        }
        void Launch( void );
        void Unblock()
        {
            Blockers_.UnblockSelf();
        }
    };

    qROW(Row);

    typedef lstbch::qBUNCHd(rSession *,sRow) dSessions;
    qW(Sessions);

    typedef lstbch::qBUNCHd(sId,sRow) dIds;
    qW(Ids);

    sRow Search(
        sId Id,
        const dIds &Ids);
}

#endif
