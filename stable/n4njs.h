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
# include "uvqdcl.h"

namespace n4njs {
	using namespace n4all;
	using uvq::cAsync;

	typedef void(* fAsync)(cAsync &);

	struct gShared {
	public:
		fAsync Async;
		void reset( bso::sBool P = true )
		{
			Async = NULL;
		}
		qCDTOR( gShared );
	};

	qENUM( Type )
	{
		tVoid,	// Only used to specify that the function does not return a value.
		tInt,
		tString,
		tStrings,
		tObject,	// Generic object; sort of superclass.
		tRStream,
		tBuffer,
		tCallback,
		tCallbacks,
		t_amount,
		t_Undefined
	};

	class sArgument {
	public:
		eType Type;
		const void *Value;
		void reset( bso::sBool P = true )
		{
			Type = t_Undefined;
			Value = NULL;
		}
		qCDTOR( sArgument );
		void Init(
			eType Type = t_Undefined,
			const void *Value = NULL )
		{
			this->Type = Type;
			this->Value = Value;
		}
	};

	typedef bch::qBUNCHdl( sArgument ) dArguments;
	qW( Arguments );

	void Delete( n4njs::dArguments &Arguments );

	template <typename t> class cUBase_
	{
	protected:
		virtual const t &N4NJSGet( void ) const = 0;
		virtual void N4NJSSet( const t &Value ) = 0;
	public:
		qCALLBACK( UBase_ );
		const t &Get( void ) const
		{
			return N4NJSGet();
		}
		void Set( const t &Value )
		{
			return N4NJSSet( Value );
		}
	};

	typedef cUBase_<int> cUInt;
	typedef cUBase_<str::dString> cUString;
	typedef cUBase_<str::dStrings> cUStrings;

	class cUObject {
	protected:
		virtual void N4NJSSet(
			const char *Key,
			void *Value ) = 0;
		virtual void *N4NJSGet( const char *Key ) = 0;
		virtual void N4NJSEmitError( const str::dString &Message ) = 0;
	public:
		qCALLBACK( UObject );
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
		void EmitError( const str::dString &Message )
		{
			return N4NJSEmitError( Message );
		}
	};

	class cUBuffer
	: public cUObject
	{
	protected:
		virtual void N4NJSToString( str::dString &String ) = 0;
	public:
		qCALLBACK( UBuffer );
		void ToString( str::dString &String )
		{
			return N4NJSToString( String );
		}
	};

	class cURStream
	: public cUObject
	{
	protected:
		virtual bso::sBool N4NJSRead( str::dString &Chunk ) = 0;
		virtual bso::sBool N4NJSPush(
			void *Buffer,
			bso::sSize Size ) = 0;
		virtual void N4NJSEnd( void ) = 0;
	public:
		qCALLBACK( URStream );
		bso::sBool Read( str::dString &Chunk )
		{
			return N4NJSRead( Chunk );
		}
		bso::sBool Push(
			void *Buffer,
			bso::sSize Size )
		{
			return N4NJSPush( Buffer, Size );
		}
		void End( void )
		{
			return N4NJSEnd();
		}
	};

	// Callback for an upstream callback, i.e. which will be launched downstream.
	class cUCallback
	: public cUObject
	{
	protected:
		virtual cUObject *N4NJSLaunch(
			eType ReturnType,
			bso::sBool *IsEmpty,	// Set to 'true' if the callback was not set (the corresponding JS parameter was absent).
			const dArguments &Arguments ) = 0;	// Type is the expected type of the returned value.
	public:
		qCALLBACK( UCallback );
		cUObject *Launch(
			eType ReturnType,
			bso::sBool *IsEmpty,	// Set to 'true' if the callback was not set (the corresponding JS parameter was absent).
			const dArguments &Arguments )
		{
			return N4NJSLaunch( ReturnType, IsEmpty, Arguments );
		}
	};

	typedef bch::qBUNCHdl(cUCallback *) dCallbacks;
	qW( Callbacks );

	void Delete( dCallbacks &Callbacks );

	typedef cUBase_<dCallbacks> cUCallbacks;
}

#endif
