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
	typedef njs::cArguments cArguments_;

	inline void Get_(
		int Index,
		cArguments_ &Arguments )
	{}

	inline void Get_(
		int Index,
		cArguments_ &Arguments,
		str::dString *Value )
	{
		return Arguments.GetValue( Index, njs::tString, Value );
	}

	inline void Get_(
		int Index,
		cArguments_ &Arguments,
		str::dString &Value )
	{
		return Get_( Index, Arguments, &Value );
	}

	inline void Get_(
		int Index,
		cArguments_ &Arguments,
		str::wString &Value )
	{
		return Get_( Index, Arguments, &Value );
	}

	template <typename item, typename ...items> inline void Get_(
		int Index,
		cArguments_ &Arguments,
		item &Item,
		items &...Items )
	{
		Get_( Index, Arguments, Item );

		Get_( Index + 1, Arguments, Items... );
	}

	class sArguments {
	private:
		qRMV( cArguments_, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( sArguments );
		void Init( cArguments_ &Callback )
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
			C_().SetReturnValue( njs::tString, &Value );
		}
	};

	typedef void (fFunction)( sArguments &Arguments );

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
			R_().Register( Function );
		}
	};

	void SCLNJSRegister( sRegistrar &Registrar );	// To define by user.
	void SCLNJSInfo( txf::sOFlow &Flow );	// To define by user.

	extern const char *SCLNJSProductVersion;	// To define by user.
}


#endif
