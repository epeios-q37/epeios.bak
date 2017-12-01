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

// Note to developer : you have to add '<path_to_node>/src' as 'Additional Include Directory' in the 'devel' related project.
#include <node_buffer.h>

namespace nodeq {
	using namespace v8q;

	template <typename object> class xBuffer_
	: public object
	{
	public:
		using object::reset;
		using object::Core;
		using object::Launch;
		qCDTOR( xBuffer_ );
		xBuffer_( 
			const char *Data,
			size_t Length,
			v8::Isolate *Isolate = NULL )
		{
			Init( Data, Length, Isolate );
		}
		xBuffer_( 
			const char *Data,
			v8::Isolate *Isolate = NULL )
		{
			Init( Data, Isolate );
		}
		using object::Init;
		void Init(
			const char *Data,
			size_t Length,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = v8q::GetIsolate( Isolate );
			//			object::Init( v8q::ToLocal( node::Buffer::New( Isolate, (char *)Data, Length, ConstFreeCallback_, NULL ) ) );
			object::Init( v8q::ToLocal( node::Buffer::New( Isolate, (char *)Data, Length, NULL, NULL ) ) );
		}
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
		void ToString( str::dString &Target ) const
		{
		qRH
			sLString String;
			char *Buffer = NULL;
			int Size = 0;
		qRB
			String.Init();
			ToString( String );

			Buffer = (char *)malloc ( Size = String.Size() + 1 );

			if ( Buffer == NULL )
				qRAlc();

			if ( Size != String.Get( Buffer ) )
				qRFwk();

			Target.Append( Buffer );
		qRR
		qRT
			if ( Buffer != NULL )
				delete( Buffer );
		qRE
		}
	};

	typedef xBuffer_<v8q::rPObject> rPObject;
	typedef xBuffer_<v8q::sLObject> sLObject;

	typedef xBuffer_<v8q::rPObject> rPBuffer;
	typedef xBuffer_<v8q::sLObject> sLBuffer;

	template <typename object> class xRStream_
	: public object
	{
	private:
		bso::sBool Push_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			v8q::sLBoolean Boolean;
			Boolean.Init( Launch( "push", Isolate, Value ) );

			return *Boolean;
		}
	public:
		using object::reset;
		using object::Core;
		using object::Launch;
		qCDTOR( xRStream_ );
		/*
			Both below event handler seems not to work properly. The 'onend' event semms not be always called. Use 'OnReadable' instead.
		*/
		template <typename obj> void OnDataFail(
			const xFunction_<obj> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "data", Callback, Isolate );
		}
		template <typename obj> void OnEndFail(
			const xFunction_<obj> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "end", Callback, Isolate );
		}
		// Use below event handler rather as the two above one !
		template <typename obj> void OnReadable(
			const xFunction_<obj> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			On( "readable", Callback, Isolate );
		}
		// This function is ONLY to implement a readable stream. Is called when data is required.
		template <typename obj> void OnRead(
			const xFunction_<obj> &Callback,
			v8::Isolate *Isolate = NULL )
		{
			Set( "_read", Callback.Core(), Isolate );
		}
		// 'true' : chunk available ; 'false' EOF ('Chunk' is NULL).
		template <typename buffer> bso::sBool Read(
			buffer &Chunk,
			v8::Isolate *Isolate = NULL )
		{
			v8q::sLValue Value;
			
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
			sLBuffer RawChunk;

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
		template <typename buffer> bso::sBool Push(
			const buffer &Buffer,
			v8::Isolate *Isolate = NULL )
		{
			return Push_( Buffer.Core(), Isolate );
		}
		void End( v8::Isolate *Isolate = NULL )
		{
			Push_( v8::Null( v8q::GetIsolate( Isolate ) ), Isolate );
		}
	};

	typedef xRStream_<v8q::rPObject> rPRStream;
	typedef xRStream_<v8q::sLObject> sLRStream;

}

template <typename object> inline txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const nodeq::xBuffer_<object> &Buffer )
{
qRH
	v8q::sLString String;
qRB
	String.Init();
	Buffer.ToString( String );

	Flow << String;
qRR
qRT
qRE
	return Flow;
}


#endif
