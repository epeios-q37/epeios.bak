/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'NJSq'.

    'NJSq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'NJSq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'NJSq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrapper.h"

#include "n4allw.h"
#include "n4njs.h"
#include "nodeq.h"
#include "sclerror.h"
#include "v8q.h"

using namespace wrapper;

typedef n4all::cCaller cCaller_;

namespace {
	inline void GetString_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info,
		str::dString &Value )
	{
		qRH
			v8q::sString String;
		qRB
			String.Init( Info[Index] );

			String.Get( Value );
		qRR
		qRT
		qRE
	}

	namespace {
		template <typename callback, typename host> class rCore_
		: public callback
		{
		protected:
			host Core_;
		protected:
			virtual void N4NJSSet(
				const char *Key,
				void *Value ) override
			{
				v8::Isolate *Isolate = v8q::GetIsolate();

				if ( !v8q::Expose( Core_.Core()->Set( v8q::GetContext(), v8q::ToString( Key, Isolate ), *( v8::Local<v8::Value> * )Value ) ) )
					qRFwk();
			}
			virtual void *N4NJSGet( const char *Key ) override
			{
				v8::Isolate *Isolate = v8q::GetIsolate();

				return &v8q::ToLocal( Core_.Core()->Get( v8q::GetContext( Isolate ), v8q::ToString( Key, Isolate ) ), Isolate );
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Core_ );
			}
			qCVDTOR( rCore_ );
			void Init( v8::Local<v8::Value> Value )
			{
				Core_.Init( Value );
			}
		};

		template <typename host> inline host *Get_(
			int Index,
			const v8::FunctionCallbackInfo<v8::Value> &Info )
		{
			host *Stream = NULL;
		qRH
		qRB
			Stream = new host;

			if ( Stream == NULL )
				qRAlc();

			Stream->Init( Info[Index] );
		qRR
			if ( Stream != NULL )
				delete Stream;
		qRT
		qRE
			return Stream;
		}

		class rRStream_
		: public rCore_<n4njs::cURStream, nodeq::sRStream>
		{
		protected:
			virtual bso::sBool NANJSRead( str::dString &Chunk ) override
			{
				return Core_.Read( Chunk );
			}
		};

		class rBuffer_
		: public rCore_<n4njs::cUBuffer, nodeq::sBuffer> {
		protected:
			virtual void N4NJSToString( str::dString &String ) override;
			virtual bso::sBool N4NJSIsNull( void ) override;
		};

		class rCallback_
		: public n4njs::cUCallback
		{
		private:
			nodeq::sFunction Core_;
		protected:
			virtual void N4NJSAdd(
				n4njs::eCallbackType_ Type,
				void *Value ) override;
			virtual void *N4NJSLaunch( n4njs::eCallbackType_ Type ) override;	// Type is the expected type of the returned value.
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Core_ );
			}
			qCVDTOR( rCallback_ );
			void Init( v8::Local<v8::Value> Value )
			{
				Core_.Init( Value );
			}
		};

	}

	inline n4njs::cURStream *GetStream_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rRStream_>( Index, Info );
	}

	inline n4njs::cUBuffer *GetBuffer_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rBuffer_>( Index, Info );
	}

	inline n4njs::cUCallback *GetCallback_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rCallback_>( Index, Info );
	}
}

void SetReturnValue_(
	const v8::FunctionCallbackInfo<v8::Value> &Info,
	const str::dString &Value )
{
	Info.GetReturnValue().Set( v8q::sString( Value ).Core() );
}

namespace {
	class sCaller_
	: public cCaller_ {
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, I_, Info_ );
	protected:
		virtual void N4ALLGetArgument(
			bso::sU8 Index,
			n4all::sEnum Type,
			void *Value ) override
		{
			Index++;	// The first one was the function id.

			if ( Index >= I_().Length() )
				qRGnr();

			switch ( Type ) {
			case n4all::tString:
				GetString_( Index, I_(), *( str::dString * )Value );
				break;
			case n4njs::tStream:
				(*(n4njs::cURStream **)Value) = GetStream_( Index, I_() );
				break;
			case n4njs::tBuffer:
				(*( n4njs::cUBuffer ** )Value) = GetBuffer_( Index, I_() );
				break;
			case n4njs::tCallback:
				( *( n4njs::cUCallback ** )Value ) = GetCallback_( Index, I_() );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void N4ALLSetReturnValue(
			n4all::sEnum Type,
			const void *Value ) override
		{
			switch ( Type ) {
			case n4all::tString:
				SetReturnValue_( I_(), *( const str::dString * )Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
	public:
		void reset( bso::sBool = true )
		{
			Info_ = NULL;
		}
		qCDTOR( sCaller_ );
		void Init( const v8::FunctionCallbackInfo<v8::Value> &Info )
		{
			Info_ = &Info;
		}
	};
}

void wrapper::Launch( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
qRH
	sCaller_ Caller;
qRB
	if ( Info.Length() < 1 )
		qRGnr();

	if ( !Info[0]->IsUint32() )
		qRGnr();

	v8::Local<v8::Uint32> Index = v8::Local<v8::Uint32>::Cast(Info[0] );

	Caller.Init( Info );

	n4allw::GetLauncher().Launch( n4allw::GetFunction( Index->Uint32Value()), Caller );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRR
qRT
qRE
}

