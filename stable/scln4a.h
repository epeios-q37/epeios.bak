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

// Native 4 (for) All  SoCLe

#ifndef SCLN4A_INC_
# define SCLN4A_INC_

# define SCLN4A_NAME		"SCLN4A"

# if defined( E_DEBUG ) && !defined( SCLN4A_NODBG )
#  define SCLN4A_DBG
# endif

# include "err.h"
# include "n4all.h"

// The content of this namespace will be completed by other libraries.
namespace scln4 {
	typedef n4all::cCaller cCaller_;

	// Will declare all needed version of this function, which can be defined in other libraries.
	template <typename item> void Get(
		int Index,
		cCaller_ &Caller,
		item &Item );

	inline void Get(
		int Index,
		cCaller_ &Caller )
	{}

	inline void Get(
		int Index,
		cCaller_ &Caller,
		str::dString *Value )
	{
		return Caller.GetArgument( Index, n4all::tString, Value );
	}

	inline void Get(
		int Index,
		cCaller_ &Caller,
		str::dString &Value )
	{
		return Get( Index, Caller, &Value );
	}

	inline void Get(
		int Index,
		cCaller_ &Caller,
		str::wString &Value )
	{
		return Get( Index, Caller, &Value );
	}
}

namespace scln4a {
	typedef n4all::cCaller cCaller_;

	inline void GetArgument_(
		int Index,
		cCaller_ &Caller )
	{
	}

	template <typename item, typename ...items> inline void GetArgument_(
		int Index,
		cCaller_ &Caller,
		item &Item,
		items &...Items )
	{
		scln4::Get( Index, Caller, Item );

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

			GetArgument_( Index, C_(), Item );
		}
		template <typename ...items> inline void GetArgument( items &...Items ) const
		{
			GetArgument_( 0, C_(), Items... );
		}
		void SetReturnValue( const str::dString &Value )
		{
			C_().SetReturnValue( n4all::tString, &Value );
		}
	};

	typedef void ( fFunction )( sCaller &Caller );

	class sRegistrar {
	private:
		qRMV( n4all::cRegistrar, R_, Registrar_ );
	public:
		void reset( bso::sBool = true )
		{
			Registrar_ = NULL;
		}
		qCDTOR( sRegistrar );
		void Init( n4all::cRegistrar &Registrar )
		{
			Registrar_ = &Registrar;
		}
		void Register( fFunction Function )
		{
			R_().Register( (void *)Function );
		}
		template <typename function, typename ...functions> void Register(
			function Function,
			functions ...Functions )
		{
			Register( Function );
			Register( Functions... );
		}
	};

	void SCLN4ARegister(
		sRegistrar &Registrar,
		void *UP );	// To define by user.
	void SCLN4AInfo( txf::sOFlow &Flow );	// To define by user.

	extern const char *SCLN4AProductVersion;	// To define by user.
}

#endif
