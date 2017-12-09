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

	// 'String(s)' variants declared here, as it will be needed by all the 'sln4...' libraries.
	// Definition will be made in this libraries.
	void Get(
		int Index,
		cCaller_ &Caller,
		str::dString *Value );

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

	void Get(
		int Index,
		cCaller_ &Caller,
		str::dStrings *Value );

	inline void Get(
		int Index,
		cCaller_ &Caller,
		str::dStrings &Value )
	{
		return Get( Index, Caller, &Value );
	}

	inline void Get(
		int Index,
		cCaller_ &Caller,
		str::wStrings &Value )
	{
		return Get( Index, Caller, &Value );
	}
}

namespace scln4a {
	template <typename function> class sRegistrar {
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
		// Termination function.
		void Register()
		{}
		template <typename ...functions> void Register(
			function Function,
			functions ...Functions )
		{
			R_().Register( (void *)Function );
			Register( Functions... );
		}
	};

	class sCaller {
	protected:
		qRMV( n4all::cCaller, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( sCaller );
		void Init( n4all::cCaller &Callback )
		{
			Callback_ = &Callback;
		}
	};


	n4all::cLauncher *SCLN4ARegister(
		n4all::cRegistrar &Registrar,
		void *UP );	// To define by user.

	extern const char *SCLN4AProductVersion;	// To define by user.
}

#endif
