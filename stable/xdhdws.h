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

# include "xdhbrd.h"

# include "err.h"
# include "tol.h"

# define XDHDWS_DEFAULT_SUFFIX  "xdh"

namespace xdhdws {
	class sProxy
	{
	private:
		Q37_MRMDF( xdhcmn::cUpstream, C_, Callback_ );
        xdhbrd::rXCallback XCallback_;
		xdhbrd::sRow CallbackRow_;
    public:
		void reset( bso::bool__ P = true )
		{
		    if ( P )
                if ( CallbackRow_ != qNIL )
                    xdhbrd::Remove(CallbackRow_);

            XCallback_.reset(P);
            Callback_ = NULL;
            CallbackRow_ = qNIL;
		}
		E_CVDTOR( sProxy );
		void Init(
            xdhcmn::cUpstream &Callback,
            const str::dString &Token)
		{
			reset();

			Callback_ = &Callback;
			CallbackRow_ = xdhbrd::InitAndAdd(Callback, XCallback_, Token);
		}
		void Process(
            const char *ScriptName,
            const str::dStrings &Values,
            str::dString *ReturnValue = NULL);
		void Process(
            const str::dString &ScriptName,
            const str::dStrings &Values,
            str::dString *ReturnValue = NULL);
		template <typename string> void Process(
            const string &ScriptName,
            const str::dStrings &Values,
            str::dString &ReturnValue)
        {
            return Process(ScriptName,Values,&ReturnValue);
        }
        void BroadcastAction(
            const str::dString &Action,
            const str::dString &Id,
            const str::dString &Token);
	};
}

#endif
