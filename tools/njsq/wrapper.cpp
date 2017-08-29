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
		v8q::sLString String;
	qRB
		String.Init( Info[Index] );

		String.Get( Value );
	qRR
	qRT
	qRE
	}

	namespace {
		namespace {
			void Error_( const v8::FunctionCallbackInfo<v8::Value>& info )
			{
				v8q::sLObject This;

				This.Init( info[0] );

				This.EmitError( info[1] );
			}
		}

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
				nodeq::rPExternal<void> External;

				External.Init( Value );

				Core_.Set( Key, External );
			}
			virtual void *N4NJSGet( const char *Key ) override
			{
				nodeq::rPExternal<void> External;

				External.Init( Core_.Get( Key ) );

				return External.Value();
			}
			virtual void N4NJSEmitError( const str::dString &Message ) override
			{
				v8q::sLFunction Function;

				Function.Init( Error_ );

				v8q::process::NextTick( Function, *this, Message );
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
			v8::Local<v8::Value> Core( void ) const
			{
				return Core_.Core();
			}
		};

		template <typename host> inline host *Get_(
			int Index,
			const v8::FunctionCallbackInfo<v8::Value> &Info )
		{
			host *Host = NULL;
		qRH
		qRB
			Host = new host;

			if ( Host == NULL )
				qRAlc();

			Host->Init( Info[Index] );
		qRR
			if ( Host != NULL )
				delete Host;
		qRT
		qRE
			return Host;
		}

		class rRStream_
		: public rCore_<n4njs::cURStream, nodeq::rPRStream>
		{
		protected:
			virtual bso::sBool N4NJSRead( str::dString &Chunk ) override
			{
				return Core_.Read( Chunk );
			}
			virtual bso::sBool N4NJSPush(
				void *Buffer,
				bso::sSize Size ) override
			{
				nodeq::sLBuffer Buf;

				Buf.Init( (const char *)Buffer, Size );

				return Core_.Push( Buf );
			}
			virtual void N4NJSEnd( void ) override
			{
				return Core_.End();
			}
		};

		class rBuffer_
		: public rCore_<n4njs::cUBuffer, nodeq::rPBuffer>
		{
		protected:
			virtual void N4NJSToString( str::dString &String ) override
			{
				Core_.ToString( String );
			}
		};

		namespace {
			namespace {
				namespace {
					void Set_(
						v8::Local<v8::Value> &Argv,
						int *Value )
					{
						nodeq::sLNumber Number;

						Number.Init( *Value );

						Argv = Number.Core();

						delete Value;
					}
					void Set_(
						v8::Local<v8::Value> &Argv,
						str::wString *Value )
					{
					qRH
						 qCBUFFERr Buffer;
					qRB
						Argv = v8q::ToString( Value->Convert( Buffer ) );
					qRR
					qRT
//						delete Value;
					qRE
					}
				}
				void Set_(
					v8::Local<v8::Value> &Argv,
					const n4njs::sArgument_ &Argument )
				{
					if ( Argument.Value == NULL )
						qRGnr();

					switch ( Argument.Type ) {
					case n4njs::atInt:
						Set_( Argv, (int *)Argument.Value );
						break;
					case n4njs::atString:
						Set_( Argv, (str::wString *)Argument.Value );
						break;
					default:
						qRGnr();
						break;
					}
				}
			}

			void Fill_(
				v8::Local<v8::Value> *&Argv,
				const n4njs::dArguments_ &Arguments )
			{
				sdr::sRow Row = Arguments.First();
				int Index = 0;

				while ( Row != qNIL ) {
					Set_( Argv[Index++], Arguments( Row ) );

					Row = Arguments.Next( Row );
				}
			}

		}

		class rCallback_
		: public rCore_<n4njs::cUCallback, nodeq::rPFunction>
		{
		protected:
			virtual void *N4NJSLaunch(
				n4njs::eArgumentType_ ReturnType,
				n4njs::dArguments_ &Arguments ) override
			{
			qRH
				v8::Local<v8::Value> *Argv = NULL;
				int Argc = 0;
			qRB
				if ( ReturnType != n4njs::atVoid )
					qRVct();

				Argc = Arguments.Amount();

				Argv = new v8::Local<v8::Value>[Argc];

				if ( Argv == NULL )
					qRAlc();

				Fill_( Argv, Arguments );

				Core_.Launch( Argv, Argc );
			qRR
			qRT
				if ( Argv != NULL )
					delete Argv;
			qRE
				return NULL;
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
	v8q::sLString String;

	String.Init( Value );

	Info.GetReturnValue().Set( String.Core() );
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

