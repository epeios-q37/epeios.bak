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

// Native 4 (for) Node.JS
// Extension of the 'N4ALL' library for Node.js.
// Common part between wrapper and component ('scln4a').

#ifndef N4NJS_INC_
# define N4NJS_INC_

# define N4NJS_NAME		"N4NJS"

# if defined( E_DEBUG ) && !defined( N4NJS_NODBG )
#  define N4NJS_DBG
# endif

# include "err.h"
# include "n4all.h"
# include "tol.h"

namespace n4njs {
	using namespace n4all;

	qENUM( Type )
	{
		t_First = n4all::t_amount,	// First types are those defined in 'n4all::eType'.
			tStream = t_First,
			tBuffer,
			tCallback
	};

	qENUM( Behavior )
	{
		bRelaunch,	// Relaunches 'N4NJSWork'.
		bExitAndDelete,	// Exits and calls 'delete' on the object, which have to be instantiated with 'new'.
		bExitOnly,	// Exits only.
		b_amount,
		b_Undefined
	};

	class cAsync {
	protected:
		// Note to 'v8' user : you can NOT access any of the 'v8' data from this method.
		virtual void N4NJSWork( void ) = 0;
		// Note to 'v8' user : you CAN access any of the 'v8' data from this method.
		virtual eBehavior N4NJSAfter( void ) = 0;
	public:
		qCALLBACK( Async );
		void Work( void )
		{
			return N4NJSWork();
		}
		eBehavior After( void )
		{
			return N4NJSAfter();
		}
	};

	class cUAsyncLauncher {
	protected:
		virtual void N4NJSLaunch( cAsync &Async ) = 0;
	public:
		qCALLBACK( UAsyncLauncher );
		void Launch( cAsync &Async )
		{
			return N4NJSLaunch( Async );
		}
	};

	qENUM( CallbackType_ )
	{
		ctVoid,	// Only to specify no return value.
		ct_amount,
		ct_Undefined
	};

	// Callback for an upstream callback, i.e. which will be launched downstream.
	class cUCallback {
	protected:
		virtual void N4NJSAdd(
			eCallbackType_ Type,
			void *Value ) = 0;
		virtual void *N4NJSLaunch( eCallbackType_ Type ) = 0;	// Type is the expected type of the returned value.
	public:
		qCALLBACK( UCallback );
		void Add(
			eCallbackType_ Type,
			void *Value )
		{
			return N4NJSAdd( Type, Value );
		}
		void *Launch( eCallbackType_ Type )
		{
			return N4NJSLaunch( Type );
		}
	};

	// Base of the callbacks which will be defined upstream.
	class cUCore_ {
	protected:
		virtual void N4NJSSet(
			const char *Key,
			void *Value ) = 0;
		virtual void *N4NJSGet( const char *Key ) = 0;
	public:
		qCALLBACK( UCore_ );
		void Set(
			const char *Key,
			void *Value )
		{
			return N4NJSSet( Key, Value );
		}
		void *Get( const char *Key )
		{
			return N4NJSGet( Key );
		}
	};

	class cUBuffer
	: public cUCore_
	{
	protected:
		virtual void N4NJSToString( str::dString &String ) = 0;
		virtual bso::sBool N4NJSIsNull( void ) = 0;
	public:
		qCALLBACK( UBuffer );
		void ToString( str::dString &String )
		{
			return N4NJSToString( String );
		}
		bso::sBool IsNull( void )
		{
			return N4NJSIsNull();
		}
	};

	class cURStream
	: public cUCore_
	{
	protected:
		virtual bso::sBool NANJSRead( str::dString &Chunk ) = 0;
	public:
		qCALLBACK( URStream );
		bso::sBool Read( str::dString &Chunk )
		{
			return NANJSRead( Chunk );
		}
	};
}

#endif
