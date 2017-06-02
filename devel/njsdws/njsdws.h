/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

// Node.JS DoWnStream

#ifndef NJSDWS_INC_
# define NJSDWS_INC_

# define NJSDWS_NAME		"NJSDWS"

# if defined( E_DEBUG ) && !defined( NJSDWS_NODBG )
#  define NJSDWS_DBG
# endif

# include "njscmn.h"

# include "err.h"
# include "str.h"

namespace njsdws {
	inline void Get_(
		int Index,
		njscmn::cArguments &Callback )
	{}

	inline void Get_(
		int Index,
		njscmn::cArguments &Callback,
		str::dString &Value )
	{
		return Callback.GetValue( Index, njscmn::tString, &Value );
	}

	inline void Get_(
		int Index,
		njscmn::cArguments &Callback,
		str::wString &Value )
	{
		return Get_( Index, Callback, ( str::dString )Value );
	}

	template <typename item, typename ...items> inline void Get_(
		int Index,
		njscmn::cArguments &Callback,
		item &Item,
		items &...Items )
	{
		Get_( Index, Callback, Item );

		Get_( Index + 1, Callback, Items... );
	}

	class sArguments {
	private:
		qRMV( njscmn::cArguments, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( sArguments );
		void Init( njscmn::cArguments &Callback )
		{
			Callback_ = &Callback;
		}
		template <typename item> void Get(
			bso::sUInt Index,
			item &Item ) const
		{
			if ( Index == 0 )
				qRFwk();

			Get_( Index, C_(), Item );
		}
		template <typename ...items> inline void Get( items &...Items ) const
		{
			Get_( 1, C_(), Items... );
		}
		void SetReturnValue( const str::dString &Value )
		{
			C_().SetReturnValue( njscmn::tString, &Value );
		}
	};
}

#endif
