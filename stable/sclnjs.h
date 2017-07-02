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

// SoCLe Node.JS

#ifndef SCLNJS_INC_
# define SCLNJS_INC_

# define SCLNJS_NAME		"SCLNJS"

# if defined( E_DEBUG ) && !defined( SCLNJS_NODBG )
#  define SCLNJS_DBG
# endif

# include "njs.h"

# include "err.h"
# include "str.h"

namespace sclnjs {
	typedef njs::cCaller cCaller_;

	inline void GetArgument_(
		int Index,
		cCaller_ &Caller )
	{}

	inline void GetArgument_(
		int Index,
		cCaller_ &Caller,
		str::dString *Value )
	{
		return Caller.GetArgument( Index, njs::tString, Value );
	}

	inline void GetArgument_(
		int Index,
		cCaller_ &Caller,
		str::dString &Value )
	{
		return GetArgument_( Index, Caller, &Value );
	}

	inline void GetArgument_(
		int Index,
		cCaller_ &Caller,
		str::wString &Value )
	{
		return GetArgument_( Index, Caller, &Value );
	}

	template <typename item, typename ...items> inline void GetArgument_(
		int Index,
		cCaller_ &Caller,
		item &Item,
		items &...Items )
	{
		GetArgument_( Index, Caller, Item );

		GetArgument_( Index + 1, Caller, Items... );
	}

	class sCaller {
	private:
		qRMV( cCaller_, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( sCaller );
		void Init( cCaller_ &Callback )
		{
			Callback_ = &Callback;
		}
		template <typename item> void GetArgument(
			bso::sUInt Index,
			item &Item ) const
		{
			if ( Index == 0 )
				qRFwk();

			Get_( Index, C_(), Item );
		}
		template <typename ...items> inline void GetArgument( items &...Items ) const
		{
			GetArgument_( 1, C_(), Items... );
		}
		void SetReturnValue( const str::dString &Value )
		{
			C_().SetReturnValue( njs::tString, &Value );
		}
	};

	typedef void (fFunction)( sCaller &Caller );

	class sRegistrar
	{
	private:
		qRMV( njs::cRegistrar, R_, Registrar_ );
	public:
		void reset( bso::sBool = true )
		{
			Registrar_ = NULL;
		}
		qCDTOR( sRegistrar );
		void Init( njs::cRegistrar &Registrar )
		{
			Registrar_ = &Registrar;
		}
		void Register( fFunction Function )
		{
			R_().Register( (void *)Function );
		}
	};

	void SCLNJSRegister( sRegistrar &Registrar );	// To define by user.
	void SCLNJSInfo( txf::sOFlow &Flow );	// To define by user.

	extern const char *SCLNJSProductVersion;	// To define by user.
}


#endif
