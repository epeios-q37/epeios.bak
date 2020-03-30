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

    class sUpstream_
    : public cUpstream_
    {
    private:
        flw::rDressedRWFlow<> Proxy_;
        sId Id_;
    protected:
        virtual void XDHCMNProcess(
            const str::string_ &Script,
            str::dString *ReturnedValue ) override;
     public:
        void reset( bso::bool__ P = true )
        {
            Proxy_.reset(P);
            Id_ = UndefinedId;
        }
        E_CVDTOR( sUpstream_ );
        void Init(
            fdr::rRWDriver &ProxyDriver,
            sId Id )
        {
            Proxy_.Init(ProxyDriver);
            Id_ = Id;
        }
    };

    class sSession
    {
    private:
        sUpstream_ Upstream_;
        xdhups::sSession Session_;
    public:
        bso::sBool Handshaked;
        void reset(bso::sBool P = true)
        {
            tol::reset(P, Upstream_, Session_);
            Handshaked = false;
        }
        qCDTOR(sSession);
        void Init(
            sId Id,
            xdhcmn::cSession *Session,
            fdr::rRWDriver &ProxyDriver)
        {
            Session_.Init(Session);
            Upstream_.Init(ProxyDriver, Id);
            Session_.Initialize(Upstream_,"",str::wString(""));
            Handshaked = false;
        }
        void Launch(
            const str::dString &Id,
            const str::dString &Action );
    };

    qROW(Row);

    typedef lstbch::qBUNCHd(sSession,sRow) dSessions;
    qW(Sessions);

    typedef lstbch::qBUNCHd(sId,sRow) dIds;
    qW(Ids);

    sRow Search(
        sId Id,
        const dIds &Ids)    ;
}

#endif
