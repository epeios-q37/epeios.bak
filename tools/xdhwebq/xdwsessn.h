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

	typedef xdhcuc::cSingle cUpstream_;

	class rUpstream_
	: public cUpstream_
	{
	private:
		qRMV(fdr::rRWDriver, D_, Driver_);
	protected:
		virtual bso::sBool XDHCUCProcess(
			const str::string_ &Script,
			tht::rBlocker *Blocker,
			str::dString *ReturnedValue ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset(P, Driver_);
		}
		E_CVDTOR( rUpstream_ );
		void Init(fdr::rRWDriver &Driver)
		{
			Driver_ = &Driver;
		}
	};

	class rSession
	{
	private:
		rUpstream_ Upstream_;
		xdhbrd::rXCallback XCallback_;
		xdhups::sSession Session_;
	public:
		void reset( bso::bool__ P = true )
		{
			tol::reset(P, Upstream_, XCallback_, Session_);
		}
		E_CDTOR( rSession );
		bso::sBool Init(
			xdhcdc::cSingle &Callback,
			fdr::rRWDriver &Driver,
			const char *Language,
			const str::dString &Token)	// If empty, FaaS session, else token used for the FaaS session.
		{
			Upstream_.Init(Driver);
			Session_.Init(Callback);
			return XCallback_.Init(Upstream_, Token) && Session_.Initialize(Upstream_, Language, Token);
		}
		bso::sBool Handle( const char *EventDigest )
		{
			return Session_.Handle(EventDigest);
		}
		bso::sBool Execute(
			const str::dString &Script,
			tht::rBlocker *Blocker,
			str::dString &ReturnedValue)
		{
			return Upstream_.Process(Script, Blocker,&ReturnedValue);
		}
		bso::sBool Execute(const str::dString &Script)
		{
			return Upstream_.Process(Script);
		}
	};
}

#endif
