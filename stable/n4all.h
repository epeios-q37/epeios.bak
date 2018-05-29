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

// Native 4 (for) ALL
// The 'N4ALL' (Native For All) project makes it possible to easily provide native component
// to runtime systems for interpreted languages (Node.js (JavaScript), JRE (Java), Zend (PHP)...).
// This library provides the abstraction layer between the runtime and the component.

#ifndef N4ALL_INC_
# define N4ALL_INC_

# define N4ALL_NAME		"N4ALL"

# if defined( E_DEBUG ) && !defined( N4ALL_NODBG )
#  define N4ALL_DBG
# endif

# include "err.h"
# include "sclmisc.h"
# include "tol.h"

# define N4ALL_REGISTER_FUNCTION_NAME		N4ALLRegister

namespace n4all {
	typedef int sType;	// Generic type of type.

	class cCaller {
	protected:
		virtual void N4ALLGetArgument(
			bso::sU8,
			sType Type,
			void *Value ) = 0;
		virtual void N4ALLSetReturnValue(
			sType Type,
			const void *Value ) = 0;
	public:
		qCALLBACK( Caller );
		void GetArgument(
			int Index,
			sType Type,
			void *Value )
		{
			return N4ALLGetArgument( Index, Type, Value );
		}
		void SetReturnValue(
			sType Type,
			const void *Value )
		{
			return N4ALLSetReturnValue( Type, Value );
		}
	};

	class cRegistrar {
	protected:
		virtual void N4ALLRegister( void *Function ) = 0;
	public:
		qCALLBACK( Registrar );
		void Register( void *Function )
		{
			return N4ALLRegister( Function );
		}
	};

	typedef void *sEnv;

	// Destroyed by launching by 'delete', so must be created with 'new' !
	class cLauncher {
	protected:
		virtual void N4ALLCall(
			sEnv *Env,
			void *Function,
			cCaller &Caller ) = 0;
		virtual void N4ALLInfo( str::dString &Info ) = 0;
	public:
		qCALLBACK( Launcher );
		void Call(
			sEnv *Env,
			void *Function,
			cCaller &Caller )
		{
			return N4ALLCall( Env, Function, Caller );
		}
		void Info( str::dString &Info )
		{
			return N4ALLInfo( Info );
		}
	};

#define N4ALL_DATA_VERSION	"1"

#pragma pack( push, 1)
	// NOTA : If modified, increment 'N4ALL_DATA_VERSION' !
	class sData {
	public:
		const char *Version;	// Always first.
		bso::size__ ControlValue;
		sclmisc::sRack *SCLRack;
		const fnm::rName *Location;
		const fnm::rName *Arguments;
		void *UP;				// User pointer.
		void reset( bso::bool__ P = true )
		{
			Version = NULL;
			UP = NULL;
			SCLRack = NULL;
			Location = NULL;
			Arguments = NULL;
		}
		E_CDTOR( sData );
		sData(
			sclmisc::sRack &Rack,
			const fnm::rName &Location,
			const fnm::rName &Arguments,
			void *UP = NULL )
		{
			Init( Rack, Location, Arguments, UP );
		}
		void Init(
			sclmisc::sRack &SCLRack,
			const fnm::rName &Location,
			const fnm::rName &Arguments,
			void *UP = NULL )
		{
			Version = PLGNCORE_SHARED_DATA_VERSION;
			ControlValue = Control();
			this->SCLRack = &SCLRack;
			this->Location = &Location;
			this->Arguments = &Arguments;
			this->UP = UP;
		}
		static bso::size__ Control( void )
		{
			return sizeof( sData );
		}
	};
#pragma pack( pop )

	// Will be used as 'extern "C"', so no reference can be used (I suppose).
	typedef cLauncher *(fRegister)(
		cRegistrar *Registrar,
		sData *Data );
}

#endif
