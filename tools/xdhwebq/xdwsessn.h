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

# include "xdhbrd.h"

# include "xdhups.h"
# include "websck.h"

namespace xdwsessn {
	class rSession; // Predeclaration.

	typedef xdhups::cEngine cEngine_;

	class sEngine_
	: public cEngine_
	{
	private:
		qRMV(fdr::rRWDriver, D_, Driver_);
	protected:
		virtual bso::sBool XDHUPSProcess(
			const str::string_ &Script,
			tht::rBlocker *Blocker,
			bso::sBool *Success,
			str::dString *ReturnedValue ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset(P, Driver_);
		}
		E_CVDTOR( sEngine_ );
		void Init(fdr::rRWDriver &Driver)
		{
			Driver_ = &Driver;
		}
	};

	typedef xdhcuc::cSingle cUpstream_;

	class sUpstream_
	: public cUpstream_
	{
  private:
    qRMV(xdhups::rSession, S_, Session_);
	protected:
		virtual bso::sBool XDHCUCProcess(
			const str::string_ &Primitive,
      const str::dStrings &TagValues,
			str::dString *ReturnedValue ) override;
	public:
		void reset( bso::bool__ = true )
		{
		  Session_ = NULL;
		}
		E_CVDTOR( sUpstream_ );
		void Init(xdhups::rSession &Session)
		{
		  reset();

		  Session_ = &Session;
		}
	};

	class rSession
	{
	private:
		sUpstream_ Upstream_;
		sEngine_ Engine_;
		xdhbrd::rXCallback Broadcaster_;
		xdhups::rSession Session_;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset(P, Upstream_, Engine_, Broadcaster_, Session_ );
		}
		E_CDTOR( rSession );
		xdhbrd::sCRow Init(
			xdhcdc::cSingle &Callback,
			fdr::rRWDriver &Driver,
			const char *Language,
			const str::dString &Token)	// If empty, self-hosting ('SlfH') session, else token used for the FaaS session.
		{
			Engine_.Init(Driver);
			Session_.Init(Callback, Engine_);
			Upstream_.Init(Session_);

      if ( Session_.Initialize(Upstream_, Language, Token) )
        return Broadcaster_.Init(Upstream_, Token);
      else
        return qNIL;
		}
		bso::sBool Handle(
      const char *EventDigest,
      const str::dString &UserId)
		{
			return Session_.Handle(EventDigest, UserId);
		}
		bso::sBool Execute(
			const str::dString &Script,
			tht::rBlocker *Blocker,
			bso::sBool *Success,
			str::dString &ReturnedValue)
		{
			return Engine_.Process(Script, Blocker, Success, &ReturnedValue);
		}
		bso::sBool Execute(const str::dString &Script)
		{
			return Engine_.Process(Script);
		}
  };
}

#endif
