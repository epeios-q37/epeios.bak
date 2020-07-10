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

// Node.JS
// This library is the shared part between the 'njsq' tool, and the 'sclnjs' library.

#ifndef NJS_INC_
# define NJS_INC_

# define NJS_NAME		"NJS"

# if defined( E_DEBUG ) && !defined( NJS_NODBG )
#  define NJS_DBG
# endif

# include "sclm.h"

# include "err.h"
# include "tol.h"

# define NJS_REGISTER_FUNCTION_NAME		NJSRegister

namespace njs {
	qENUM( Type ) {
		tString,
		tamount,
		t_Undefined
	};

	class cCaller {
	protected:
		virtual void NJSGetArgument(
			int Index,
			eType Type,
			void *Value ) = 0;
		virtual void NJSSetReturnValue(
			eType Type,
			const void *Value ) = 0;
	public:
		qCALLBACK( Caller );
		void GetArgument(
			int Index,
			eType Type,
			void *Value )
		{
			return NJSGetArgument( Index, Type, Value );
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
			cCaller &Caller ) = 0;
		virtual void NJSInfo( str::dString &Info ) = 0;
	public:
		qCALLBACK( Launcher );
		void Launch(
			void *Function,
			cCaller &Caller )
		{
			return NJSLaunch( Function, Caller );
		}
		void Info( str::dString &Info )
		{
			return NJSInfo( Info );
		}
	};

#define NJS_DATA_VERSION	"1"

#pragma pack( push, 1)
	// NOTA : If modified, increment 'NJS_DATA_VERSION' !
	class sData {
	public:
		const char *Version;	// Always first.
		bso::size__ ControlValue;
		sclm::sRack *SCLRack;
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
			sclm::sRack &Rack,
			const fnm::rName &Location,
			const fnm::rName &Arguments,
			void *UP = NULL )
		{
			Init( Rack, Location, Arguments, UP );
		}
		void Init(
			sclm::sRack &SCLRack,
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
