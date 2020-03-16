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

#ifndef XDWMAIN_INC_
# define XDWMAIN_INC_

# include "xdwrgstry.h"

# include "xdhups.h"
# include "websck.h"

namespace xdwmain {
    class rSession; // Predeclaration.

    using xdhups::rAgent;

    typedef xdhcmn::cUpstream cUpstream_;

	class sUpstream
	: public cUpstream_
	{
	private:
		websck::rFlow Flow_;
	protected:
		virtual void XDHCMNProcess(
			const str::string_ &Script,
			str::dString *ReturnedValue ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			Flow_.reset(P);
		}
		E_CVDTOR( sUpstream );
		void Init( fdr::rRWDriver &Driver )
		{
			Flow_.Init(Driver, websck::mWithTerminator);
		}
	};


	class rSession
	{
	private:
        qRMV(rAgent, A_, Agent_);
        sUpstream Upstream_;
        xdhcmn::cSession *SessionCallback_;
		xdhups::sSession Session_;
	public:
		void reset( bso::bool__ P = true )
		{
            if ( P ) {
                if ( Agent_ != NULL)
                    if ( SessionCallback_ != NULL )
                        A_().ReleaseSession( SessionCallback_ );
            }

            Agent_ = NULL;
            SessionCallback_ = NULL;
            tol::reset(P, Session_, Upstream_ );
		}
		E_CDTOR( rSession );
		bso::sBool Init(
            rAgent &Agent,
            fdr::rRWDriver &Driver,
            const char *Language,
			const str::dString &Token )	// If empty, FaaS session, else token used for the FaaS session.
 		{
            reset();

            Agent_ = &Agent;

			Upstream_.Init(Driver);
			SessionCallback_ = A_().RetrieveSession();
			Session_.Init(SessionCallback_);
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
            Upstream_.Process(Script, ReturnedValue);
        }
        void Execute(const str::dString &Script)
        {
            Upstream_.Process(Script);
        }
	};
}

#endif
