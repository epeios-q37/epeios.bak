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

// Node.JS related extension to the 'v8q' module

#ifndef V8QNJS_INC_
# define V8QNJS_INC_

# define V8QNJS_NAME		"V8QNJS"

# if defined( E_DEBUG ) && !defined( V8QNJS_NODBG )
#  define V8QNJS_DBG
# endif

# include "v8q.h"

# include "err.h"

namespace v8qnjs {
	using namespace v8q;

	// A class used to retrieve some 'node.js' specific objects.
	class sHelper
	: public sObject
	{
	public:
		qCDTOR( sHelper );
		using sObject::Init;
		using sObject::Get;
		void Init(
			sHelper &Helper,
			v8::Isolate *Isolate = NULL )
		{
			sObject::Init(Helper.Core(), Isolate );
		}
	};

	class sBuffer
	: public sValue
	{
	public:
		qCDTOR( sBuffer );
		using sValue::Init;
		void Init(
			const char *String,
			sFunction &Function,
			v8::Isolate *Isolate = NULL )
		{
			sValue::Init( Function.Launch( String ), Isolate );
		}
		void Init(
			const char *String,
			sHelper &Helper,
			v8::Isolate *Isolate = NULL )
		{
			sFunction Function;

			Function.Init( Helper.Get( "from" ), Isolate );
			Init( String, Function, Isolate);
		}
		void ToString(
			sString &String,
			v8::Isolate *Isolate = NULL ) const
		{
			String.Init( v8q::sObject( *this ).Launch( "toString" ), Isolate );
		}
	};

	class sRStream
	: public sObject
	{
	private:
		bso::sBool Push_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			v8q::sBoolean Boolean;
			Boolean.Init( Launch( "push", Isolate, Value ) );

			return *Boolean;
		}
	public:
		qCDTOR( sRStream );
		/*
			Both below event handler seems not to work properly. The 'onend' event semms not be always called. Use 'OnReadable' instead.
		*/
		void OnDataFail(
			const sFunction &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "data", Callback, Isolate );
		}
		void OnEndFail(
			const sFunction &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "end", Callback, Isolate );
		}
		// Use below event handler rather as the two above one !
		void OnReadable(
			const sFunction &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "readable", Callback, Isolate );
		}
		// This function is ONLY to implement a readable stream. Is called when data is required.
		void OnRead(
			const sFunction &Callback,
			v8::Isolate *Isolate = NULL )
		{
			Set( "_read", Callback.Core(), Isolate );
		}
		// 'true' : chunk available ; 'false' EOF ('Chunk' is NULL).
		bso::sBool Read(
			sBuffer &Chunk,
			v8::Isolate *Isolate = NULL )
		{
			Chunk.Init( Launch( "read", Isolate ), Isolate );

			return !Chunk.IsNull();
		}
		bso::sBool Push(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate  = NULL )
		{
			return Push_( Value, Isolate );
		}
		bso::sBool Push(
			const sBuffer &Buffer,
			v8::Isolate *Isolate = NULL )
		{
			return Push_( Buffer.Core(), Isolate );
		}
		void End( v8::Isolate *Isolate = NULL )
		{
			Push_( v8::Null( v8q::GetIsolate( Isolate ) ), Isolate );
		}
	};
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const v8qnjs::sBuffer &Buffer );

#endif
