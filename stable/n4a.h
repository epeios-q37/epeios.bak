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

// Native 4 (for) All
// The 'N4A' (Native For All) project makes it possible to easily provide native component
// to runtime systems for interpreted languages (Node.js (JavaScript), JRE (Java), Zend (PHP)...).
// This library provides the abstraction layer between the runtime and the component.

#ifndef N4A_INC_
# define N4A_INC_

# define N4A_NAME		"N4A"

# if defined( E_DEBUG ) && !defined( N4A_NODBG )
#  define N4A_DBG
# endif

# define N4A_REGISTER_FUNCTION_NAME		N4ARegister

# include "err.h"
# include "sclmisc.h"
# include "tol.h"

namespace n4a {
	qENUM( Type ) {
		tString,
		tamount,
		t_Undefined
	};

	class cCaller {
	protected:
		virtual void N4AGetArgument(
			int Index,
			eType Type,
			void *Value ) = 0;
		virtual void N4ASetReturnValue(
			eType Type,
			const void *Value ) = 0;
	public:
		qCALLBACK( Caller );
		void GetArgument(
			int Index,
			eType Type,
			void *Value )
		{
			return N4AGetArgument( Index, Type, Value );
		}
		void SetReturnValue(
			eType Type,
			const void *Value )
		{
			return N4ASetReturnValue( Type, Value );
		}
	};

	class cRegistrar {
	protected:
		virtual void N4ARegister( void *Function ) = 0;
	public:
		qCALLBACK( Registrar );
		void Register( void *Function )
		{
			return N4ARegister( Function );
		}
	};

	// Destroyed by launching by 'delete', so must be created with 'new' !
	class cLauncher {
	protected:
		virtual void N4ALaunch(
			void *Function,
			cCaller &Caller ) = 0;
		virtual void N4AInfo( str::dString &Info ) = 0;
	public:
		qCALLBACK( Launcher );
		void Launch(
			void *Function,
			cCaller &Caller )
		{
			return N4ALaunch( Function, Caller );
		}
		void Info( str::dString &Info )
		{
			return N4AInfo( Info );
		}
	};

#define N4A_DATA_VERSION	"1"

#pragma pack( push, 1)
	// NOTA : If modified, increment 'N4A_DATA_VERSION' !
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
