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

// XDH(TML) Common Upstream Callbacks

#ifndef XDHCUC_INC_
# define XDHCUC_INC_

# define XDHCUC_NAME		"XDHCUC"

# if defined( E_DEBUG ) && !defined( XDHCUC_NODBG )
#  define XDHCUC_DBG
# endif

# include "xdhcmn.h"

# include "err.h"
# include "str.h"

namespace xdhcuc {
	class cSingle
	{
	protected:
		// The value returned by the script has to be stored
		// in 'ReturnedValue', unless it is equal to 'NULL'.
		// Returns 'false' when error (mainly lost connection to client).
		virtual bso::sBool XDHCUCProcess(
			const str::dString &Script,
			str::dString *ReturnedValue ) = 0;

	public:
		qCALLBACK( Single );
		bso::sBool Process(
			const str::dString &Script,
			str::dString *ReturnValue = NULL)
		{
			return XDHCUCProcess(Script, ReturnValue);
		}

	};

	namespace faas_ {
		using namespace xdhcmn::faas;
	}

	class cGlobal
	{
	protected:
		virtual faas_::sRow XDHCUCCreate(const str::dString &Token) = 0;
		virtual void XDHCUCBroadcast(
			const str::dString &Script,
			faas_::sRow Row) = 0;
		virtual void XDHCUCRemove(faas_::sRow Row) = 0;
	public:
		qCALLBACK(Global);
		faas_::sRow Create(const str::dString &Token)
		{
			return XDHCUCCreate(Token);
		}
		void Broadcast(
			const str::dString &Script,
			faas_::sRow Row)
		{
			return XDHCUCBroadcast(Script, Row);
		}
		void Remove(faas_::sRow Row)
		{
			return XDHCUCRemove(Row);
		}
	};
}

#endif
