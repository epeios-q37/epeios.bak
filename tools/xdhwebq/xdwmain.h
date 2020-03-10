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

# include "xdhujp.h"
# include "xdhups.h"
# include "websck.h"

namespace xdwmain {
    class rSession; // Predeclaration.

    using xdhups::rAgent;

    typedef xdhujp::cJS cJS_;

	class sJS
	: public cJS_
	{
	private:
		Q37_MRMDF( rSession, S_, Session_ );
		websck::rFlow Flow_;
	protected:
		virtual void XDHUJPExecute(
			const str::string_ &Script,
			str::dString &ReturnValue ) override;
		virtual void XDHUJPGetWidgetAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Widget, Buffer);
		}
		virtual void XDHUJPGetResultAttributeName( TOL_CBUFFER___ &Buffer ) override
		{
			sclmisc::MGetValue( xdwrgstry::custom_item::attribute_name::Result, Buffer );
		}
		/*
		virtual void XDHJSPHandleExtensions( const xdhcbk::nstring___ &Id ) override;
		virtual void XDHJSPHandleCastings( const xdhcbk::nstring___ &Id ) override;
		*/
	public:
		void reset( bso::bool__ P = true )
		{
			Session_ = NULL;
			Flow_.reset(P);
		}
		E_CVDTOR( sJS );
		void Init(
            rSession &Session,
            fdr::rRWDriver &Driver )
		{
			Session_ = &Session;
			Flow_.Init(Driver, websck::mWithTerminator);
		}
	};


	class rSession
	{
	private:
        qRMV(rAgent, A_, Agent_);
		sJS JS_;
        xdhujp::sUpstream Upstream_;
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
            tol::reset(P, JS_, Session_, Upstream_ );
		}
		E_CDTOR( rSession );
		bso::sBool Init(
            rAgent &Agent,
            fdr::rRWDriver &Driver,
            const char *Language,
			const str::dString &Token )	// If empty, FaaS session, else token used for the DEMO session.
 		{
            reset();

            Agent_ = &Agent;

			JS_.Init(*this, Driver);
			Upstream_.Init(JS_);
			SessionCallback_ = A_().RetrieveSession(Language, Token, &Upstream_);
			Session_.Init(SessionCallback_);
			return Session_.Initialize(Upstream_, Language, Token);
		}
		bso::sBool Launch(
            const char *Id,
            const char *Action )
        {
            return Session_.Launch(Id, Action );
        }
        void SetMainPage(const str::dString &HTML)
        {
            xdhujp::SetMainPage(JS_, HTML);
        }
	};
}

#endif
