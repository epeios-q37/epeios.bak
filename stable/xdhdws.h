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

#ifndef XDHDWS_INC_
# define XDHDWS_INC_

# define XDHDWS_NAME		"XDHDWS"

# if defined( E_DEBUG ) && !defined( XDHDWS_NODBG )
#  define XDHDWS_DBG
# endif

// X(SL) DH(TML) DowNStream

# include "xdhcmn.h"
# include "xdhcuc.h"

# include "err.h"
# include "tol.h"

# define XDHDWS_DEFAULT_SUFFIX  "xdh"

namespace xdhdws {
	namespace faas {
		using namespace xdhcmn::faas;
	}

	typedef bso::sU8 sScriptsVersion;

	sScriptsVersion GetScriptsVersion_(void);

	class sProxy
	{
	private:
		Q37_MRMDF( xdhcuc::cSingle, C_, Callback_ );
	public:
		void reset( bso::bool__ P = true )
		{
			Callback_ = NULL;
		}
		E_CVDTOR( sProxy );
		bso::sBool Init(xdhcuc::cSingle &Callback)
		{
			reset();

			Callback_ = &Callback;

			return true;
		}
		bso::sBool Process(
			const char *ScriptName,
			const str::dStrings &TagValues,
			str::dString *ReturnValue = NULL);
		bso::sBool Process(
			const str::dString &ScriptName,
			const str::dStrings &TagValues,
			str::dString *ReturnValue = NULL);
		template <typename string> bso::sBool Process(
			const string &ScriptName,
			const str::dStrings &TagValues,
			str::dString &ReturnValue)
		{
			return Process(ScriptName, TagValues, &ReturnValue);
		}
		bso::sBool Process(
			const char *TaggedScript,
			const char *TagList,
			const str::dStrings &TagValues,
			str::dString *ReturnValue = NULL);
		bso::sBool Process(
			const str::dString &TaggedScript,
			const char *TagList,
			const str::dStrings &TagValues,
			str::dString *ReturnValue = NULL);
		template <typename string> bso::sBool Process(
			const string &TaggedScript,
			const char *TagList,
			const str::dStrings &TagValues,
			str::dString &ReturnValue)
		{
			return Process(TaggedScript, TagList, TagValues, &ReturnValue);
		}
	};

	void BroadcastAction(
		xdhcuc::cGlobal &Callback,
		const char *Action,
		const char *Id,
		faas::sRow FaasRow);

	void BroadcastAction(
		xdhcuc::cGlobal &Callback,
		const str::dString &Action,
		const str::dString &Id,
		faas::sRow FaasRow);
}

#endif
