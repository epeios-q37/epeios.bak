/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XDHWebQ.

    XDHWebQ is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XDHWebQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XDHWebQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XDWSESSN_INC_
# define XDWSESSN_INC_

# include "xdwrgstry.h"

# include "xdhups.h"
# include "websck.h"

namespace xdwsessn {
    class rSession; // Predeclaration.

    typedef xdhcmn::cUpstream cUpstream_;

	class rUpstream_
	: public cUpstream_
	{
	private:
		str::wString Token_;
		websck::rFlow Flow_;
	protected:
		virtual void XDHCMNProcess(
			const str::string_ &Script,
			str::dString *ReturnedValue ) override;
//        virtual void XDHCMNBroadcast(const str::dString &Id) override;
	public:
		void reset( bso::bool__ P = true )
		{
		    tol::reset(Token_, Flow_);
			Flow_.reset(P);
		}
		E_CVDTOR( rUpstream_ );
		void Init_(
             fdr::rRWDriver &Driver,
             const str::dString &Token)
		{
		    Token_.Init(Token);
			Flow_.Init(Driver, websck::mWithTerminator);
		}
	};

	class rSession
	{
	private:
        rUpstream_ Upstream_;
		xdhups::sSession Session_;
	public:
		void reset( bso::bool__ P = true )
		{
            tol::reset(P, Upstream_, Session_);
		}
		E_CDTOR( rSession );
		bso::sBool Init(
            xdhcmn::cSession *Callback,
            fdr::rRWDriver &Driver,
            const char *Language,
			const str::dString &Token )	// If empty, FaaS session, else token used for the FaaS session.
 		{
			Upstream_.Init_(Driver, Token);
			Session_.Init(Callback);
			return Session_.Initialize(Upstream_, Language, Token);
		}
		bso::sBool Launch(
            const char *Id,
            const char *Action )
        {
            return Session_.Launch(Id, Action );
        }
        void Execute(
            const str::dString &Script,
            str::dString &ReturnedValue)
        {
            Upstream_.Process(Script, &ReturnedValue);
        }
        void Execute(const str::dString &Script)
        {
            Upstream_.Process(Script);
        }
	};
}

#endif
