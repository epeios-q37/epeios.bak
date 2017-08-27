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

// From NODE.js

#ifndef NODEQ_INC_
# define NODEQ_INC_

# define NODEQ_NAME		"NODEQ"

# if defined( E_DEBUG ) && !defined( NODEQ_NODBG )
#  define NODEQ_DBG
# endif

# include "err.h"
# include "v8q.h"

namespace nodeq {
	using namespace v8q;

	class sBuffer
	: public rPersistentObject
	{
	public:
		qCDTOR( sBuffer );
		sBuffer( 
			const char *Data,
			size_t Length,
			v8::Isolate *Isolate = NULL )
		{
			Init( Data, Length, Isolate );
		}
		sBuffer( 
			const char *Data,
			v8::Isolate *Isolate = NULL )
		{
			Init( Data, Isolate );
		}
		using rPersistentObject::Init;
		void Init(
			const char *Data,
			size_t Length,
			v8::Isolate *Isolate = NULL );
		void Init(
			const char *Data,
			v8::Isolate *Isolate = NULL )
		{
			Init(Data, strlen( Data ), Isolate );
		}
		template <typename name> void ToString( xString_<name> &String ) const
		{
			String.Init( Launch( "toString" ) );
		}
		void ToString( str::dString &String ) const;
	};

	class sRStream
	: public sLocalObject
	{
	private:
		bso::sBool Push_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			v8q::sLocalBoolean Boolean;
			Boolean.Init( Launch( "push", Isolate, Value ) );

			return *Boolean;
		}
	public:
		qCDTOR( sRStream );
		/*
			Both below event handler seems not to work properly. The 'onend' event semms not be always called. Use 'OnReadable' instead.
		*/
		template <typename object> void OnDataFail(
			const xFunction_<object> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "data", Callback, Isolate );
		}
		template <typename object> void OnEndFail(
			const xFunction_<object> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "end", Callback, Isolate );
		}
		// Use below event handler rather as the two above one !
		template <typename object> void OnReadable(
			const xFunction_<object> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "readable", Callback, Isolate );
		}
		// This function is ONLY to implement a readable stream. Is called when data is required.
		template <typename object> void OnRead(
			const xFunction_<object> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			Set( "_read", Callback.Core(), Isolate );
		}
		// 'true' : chunk available ; 'false' EOF ('Chunk' is NULL).
		bso::sBool Read(
			sBuffer &Chunk,
			v8::Isolate *Isolate = NULL )
		{
			v8q::sLocalValue Value;
			
			Value.Init( Launch( "read", Isolate ) );

			if ( Value.IsNull() )
				return false;
			else {
				Chunk.Init( Value.Core() );
				return true;
			}
		}
		bso::sBool Read(
			str::dString &Chunk,
			v8::Isolate *Isolate = NULL )
		{
			sBuffer RawChunk;

			RawChunk.Init();

			if ( Read( RawChunk, Isolate ) ) {
				RawChunk.ToString( Chunk );
				return true;
			} else
				return false;
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
	const nodeq::sBuffer &Buffer );

#endif
