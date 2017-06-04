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

// Node.JS
// This library is the shared part between the 'njsq' tool, and the 'sclnjs' library.

#ifndef NJS_INC_
# define NJS_INC_

# define NJS_NAME		"NJS"

# if defined( E_DEBUG ) && !defined( NJS_NODBG )
#  define NJS_DBG
# endif

# include "err.h"
# include "tol.h"

# define NJS_REGISTER_FUNCTION_NAME		NJSRegister

namespace njs {
	qENUM( Type )
	{
		tString,
			tamount,
			t_Undefined
	};

	class cArguments {
	protected:
		virtual void NJSGetValue(
			int Index,
			eType Type,
			void *Value ) = 0;
		virtual void NJSSetReturnValue(
			eType Type,
			const void *Value ) = 0;
	public:
		qCALLBACK( Arguments );
		void GetValue(
			int Index,
			eType Type,
			void *Value )
		{
			return NJSGetValue( Index, Type, Value );
		}
		void SetReturnValue(
			eType Type,
			const void *Value )
		{
			return NJSSetReturnValue( Type, Value );
		}
	};

	class cRegistrar {
	protected:
		virtual void NJSRegister( void *Function ) = 0;
	public:
		qCALLBACK( Registrar );
		void Register( void *Function )
		{
			return NJSRegister( Function );
		}
	};

	// Destroyed by launching by 'delete', so must be created with 'new' !
	class cLauncher {
	protected:
		virtual void NJSLaunch(
			void *Function,
			cArguments &Arguments ) = 0;
	public:
		qCALLBACK( Launcher );
		void Launch(
			void *Function,
			cArguments &Arguments )
		{
			return NJSLaunch( Function, Arguments );
		}
	};

	class sSharedData {
	public:
		void Init( void )
		{}
	};

	// Will be used as 'extern "C"', so no reference can be used (I suppose).
	typedef cLauncher *(fRegister)(
		cRegistrar *Registrar,
		sSharedData *Data );
}

#endif
