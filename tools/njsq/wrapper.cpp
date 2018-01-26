/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

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
	template <typename cbase, typename dbase, typename wbase> class rBase
	: public cbase
	{
	private:
		wbase Base_;
	protected:
		virtual void N4NJSSet( const dbase &Base ) override
		{
			Base_ = Base;
		}
		virtual const dbase &N4NJSGet( void ) const override
		{
			return Base_;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Base_.reset( P );
		}
		qCVDTOR( rBase );
		void Init( void )
		{
			Base_.Init();
		}
		dbase &Expose( void )
		{
			return Base_;
		}
		const dbase &Expose( void ) const
		{
			return Base_;
		}
	};

	typedef rBase<n4njs::cUString, str::dString, str::wString> rString;

	inline n4njs::cUString *GetString_( const v8::Local<v8::Value> &Value )
	{
		rString *StringCallback = NULL;
	qRH
		v8q::sLString String;
	qRB
		StringCallback = new rString;

		if ( StringCallback == NULL )
			qRAlc();

		StringCallback->Init();

		String.Init( Value );

		String.Get( StringCallback->Expose() );
	qRR
		if ( StringCallback != NULL )
			delete StringCallback;
	qRT
	qRE
		return StringCallback;
	}

	inline n4njs::cUString *GetString_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return GetString_( Info[Index] );
	}

	typedef rBase<n4njs::cUStrings, str::dStrings, str::wStrings> rStrings;

	inline n4njs::cUStrings *GetStrings_( const v8::Local<v8::Array> &Array )
	{
		rStrings *StringsCallback = NULL;
	qRH;
		uint32_t Length = 0;
		v8q::sLString String;
	qRB;
		StringsCallback = new rStrings;

		if ( StringsCallback == NULL )
			qRAlc();

		StringsCallback->Init();

		Length = Array->Length();

		if ( Length != 0 ) {
			StringsCallback->Expose().New( Length );

			while ( Length-- ) {
				String.Init( v8q::ToLocal( Array->Get( v8q::GetContext(), Length ) ) );
				String.Get( StringsCallback->Expose()( Length ) ); 
			}
		}
	qRR;
		if ( StringsCallback != NULL )
			delete StringsCallback;
	qRT;
	qRE;
		return StringsCallback;
	}

	inline n4njs::cUStrings *GetStrings_( const v8::Local<v8::Value> &Value )
	{
		if ( !Value->IsArray() )
			qRGnr();

		return GetStrings_( v8::Local<v8::Array>::Cast( Value ) );
	}

	inline n4njs::cUStrings *GetStrings_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return GetStrings_( Info[Index] );
	}

	namespace {
		namespace {
			void Error_( const v8::FunctionCallbackInfo<v8::Value>& info )
			{
				v8q::sLFunction This;

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
			void Init(
				v8::Local<v8::Value> Value,
				bso::sBool UndefinedForbidden )
			{
				Core_.Init( Value, UndefinedForbidden );
			}
			v8::Local<v8::Value> Core( void ) const
			{
				return Core_.Core();
			}
		};

		template <typename host> inline host *Get_(
			const v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			bso::sBool *Success = NULL )
		{
			host *Host = NULL;
		qRH
		qRB
			Host = new host;

			if ( Host == NULL )
				qRAlc();

			Host->Init( Value, UndefinedForbidden );
		qRR
			if ( Host != NULL )
				delete Host;
		qRT
		qRE
			return Host;
		}

		template <typename host> inline host *Get_(
			int Index,
			const v8::FunctionCallbackInfo<v8::Value> &Info,
			bso::sBool UndefinedForbidden )
		{
			return Get_<host>( Info[Index], UndefinedForbidden );
		}

		typedef rCore_<n4njs::cUObject, nodeq::rPObject> rObject_;

		class rBuffer_
		: public rCore_<n4njs::cUBuffer, nodeq::rPBuffer> {
		protected:
			virtual void N4NJSToString( str::dString &String ) override
			{
				Core_.ToString( String );
			}
		};

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

		namespace {
			namespace {
				namespace {
					void Set_(
						v8::Local<v8::Value> &Argv,
						const int *Value )
					{
						nodeq::sLNumber Number;

						Number.Init( *Value );

						Argv = Number.Core();
					}

					void Set_(
						v8::Local<v8::Value> &Argv,
						const str::dString *Value )
					{
					qRH
						 qCBUFFERr Buffer;
					qRB
						Argv = v8q::ToString( Value->Convert( Buffer ) );
					qRR
					qRT
					qRE
					}

					void Set_(
						v8::Local<v8::Value> &Argv,
						const n4njs::cUObject *Object )
					{
						Argv = ( (rObject_ *)Object )->Core();
					}

					void Set_(
						v8::Local<v8::Value> &Argv,
						const str::dStrings *Value )
					{
					qRH;
						qCBUFFERr Buffer;
						sdr::sRow Row = qNIL;
					qRB;
						if ( Value->Amount() > bso::U32Max )
							qRLmt();

						v8::Local<v8::Array> Array = v8::Array::New( v8q::GetIsolate(), Value->Amount() );

						Row = Value->First();

						while ( Row != qNIL ) {
							if ( !Array->Set( v8q::GetContext(), (int32_t)*Row, v8q::ToString( Value->operator()( Row ).Convert( Buffer ) ) ).FromJust() )
								qRGnr();

							Row = Value->Next( Row );

						}

						Argv = Array;
					qRR;
					qRT;
					qRE;
					}
				}

				void Set_(
					v8::Local<v8::Value> &Argv,
					const n4njs::sArgument &Argument )
				{
					if ( Argument.Value== NULL )
						qRGnr();

					switch ( Argument.Type ) {
					case n4njs::tInt:
						Set_( Argv, (const int *)Argument.Value );
						break;
					case n4njs::tString:
						Set_( Argv, (const str::dString *)Argument.Value );
						break;
					case n4njs::tObject:
						Set_( Argv, (const n4njs::cUObject *)Argument.Value );
						break;
					case n4njs::tStrings:
						Set_( Argv, (const str::dStrings *)Argument.Value );
						break;
					default:
						qRGnr();
						break;
					}
				}
			}

			void Fill_(
				v8::Local<v8::Value> *&Argv,
				const n4njs::dArguments &Arguments )
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
			virtual n4njs::cUObject *N4NJSLaunch(
				n4njs::eType ReturnType,
				bso::sBool *IsEmpty,
				const n4njs::dArguments &Arguments ) override
			{
				n4njs::cUObject *CallbackReturn = NULL;
			qRH
				v8::Local<v8::Value> *Argv = NULL;
				int Argc = 0;
				v8::Local<v8::Value> Return;
			qRB
				Argc = Arguments.Amount();

				Argv = new v8::Local<v8::Value>[Argc];

				if ( Argv == NULL )
					qRAlc();

				Fill_( Argv, Arguments );

				if ( Core_.IsEmpty() )
					*IsEmpty = true;
				else {
					Return = Core_.Launch( Argv, Argc );

					switch ( ReturnType ) {
					case n4njs::tVoid:
						break;
					case n4njs::tObject:
						if ( Return.IsEmpty() )
							qRGnr();
						CallbackReturn = Get_<rObject_>( Return, true );
						break;
					default:
						qRVct();
						break;
					}
				}
			qRR
				if ( CallbackReturn != NULL )
					delete CallbackReturn;
			qRT
				if ( Argv != NULL )
					delete Argv;
			qRE
				return CallbackReturn;
			}
		};
	}

	inline n4njs::cUObject *GetObject_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rObject_>( Index, Info, true );
	}

	inline n4njs::cUBuffer *GetBuffer_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rBuffer_>( Index, Info, true );
	}

	inline n4njs::cURStream *GetStream_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return Get_<rRStream_>( Index, Info, true );
	}

	inline n4njs::cUCallback *GetCallback_(
		const v8::Local<v8::Value> &Value,
		bso::sBool UndefinedForbidden )
	{
		return Get_<rCallback_>( Value, UndefinedForbidden );
	}

	inline n4njs::cUCallback *GetCallback_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info,
		bso::sBool UndefinedForbidden )
	{
		return GetCallback_( Info[Index], UndefinedForbidden );
	}

	inline void Delete_( n4njs::cUCallback *&Callback )
	{
		if ( Callback != NULL ) {
			delete Callback;

			Callback = NULL;
		}
	}

	class rCallbacks
	: public rBase<n4njs::cUCallbacks, n4njs::dCallbacks, n4njs::wCallbacks>
	{
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				n4njs::Delete( Expose() );
			}
		}
		qCVDTOR( rCallbacks );
	};

	inline n4njs::cUCallbacks *GetCallbacks_( const v8::Local<v8::Array> &Array )
	{
		rCallbacks *Callbacks = NULL;
	qRH;
		uint32_t Length = 0;
	qRB;
		Callbacks = new rCallbacks;

		if ( Callbacks == NULL )
			qRAlc();

		Callbacks->Init();

		Length = Array->Length();

		if ( Length != 0 ) {
			Callbacks->Expose().New( Length );

			Callbacks->Expose().FillWith( NULL );

			while ( Length-- ) {
				Callbacks->Expose().Store( GetCallback_( Array->Get( Length ), true ), Length );
			}
		}
	qRR;
		if ( Callbacks != NULL ) {
			n4njs::Delete( Callbacks->Expose() );

			delete Callbacks;
		}
	qRT;
	qRE;
		return Callbacks;
	}

	inline n4njs::cUCallbacks *GetCallbacks_( const v8::Local<v8::Value> &Value )
	{
		if ( !Value->IsArray() )
			qRGnr();

		return GetCallbacks_( v8::Local<v8::Array>::Cast( Value ) );
	}

	inline n4njs::cUCallbacks *GetCallbacks_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> &Info )
	{
		return GetCallbacks_( Info[Index] );
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

void SetReturnValue_(
	const v8::FunctionCallbackInfo<v8::Value> &Info,
	const str::dStrings &Values )
{
	Info.GetReturnValue().Set( v8q::ToArray( Values ) );
}

namespace {
	class sCaller_
	: public cCaller_ {
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, I_, Info_ );
	protected:
		virtual void N4ALLGetArgument(
			bso::sU8 Index,
			n4all::sType Type,
			void *Value ) override
		{
			Index+=2;	// First argument is the launcher, second the function id.

			if ( Index >= I_().Length() )
				qRGnr();

			switch ( Type ) {
			case n4njs::tString:
				( *( n4njs::cUString ** )Value ) = GetString_( Index, I_() );
				break;
			case n4njs::tStrings:
				( *( n4njs::cUStrings ** )Value ) = GetStrings_( Index, I_() );
				break;
			case n4njs::tObject:
				( *(n4njs::cUObject **)Value ) = GetObject_( Index, I_() );
				break;
			case n4njs::tBuffer:
				(*(n4njs::cUBuffer **)Value ) = GetBuffer_( Index, I_() );
				break;
			case n4njs::tRStream:
				( *(n4njs::cURStream **)Value ) = GetStream_( Index, I_() );
				break;
			case n4njs::tCallback:
				( *(n4njs::cUCallback **)Value ) = GetCallback_( Index, I_(), false );
				break;
			case n4njs::tCallbacks:
				( *( n4njs::cUCallbacks **)Value ) = GetCallbacks_( Index, I_() );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void N4ALLSetReturnValue(
			n4all::sType Type,
			const void *Value ) override
		{
			switch ( Type ) {
			case n4njs::tString:
				SetReturnValue_( I_(), *( const str::dString * )Value );
				break;
			case n4njs::tStrings:
				SetReturnValue_( I_(), *( const str::dStrings * )Value );
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

void wrapper::rLauncher::Call( const v8::FunctionCallbackInfo<v8::Value>& Info )
{
qRH
	sCaller_ Caller;
qRB
	if ( Info.Length() < 2 )
		qRGnr();

	if ( !Info[1]->IsUint32() )
		qRGnr();

	v8::Local<v8::Uint32> Index = v8::Local<v8::Uint32>::Cast(Info[1] );

	Caller.Init( Info );

	rLauncher_::Call( Index->Uint32Value(), Caller );

//	n4allw::GetLauncher().Launch( n4allw::GetFunction( Index->Uint32Value() ), Caller );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRR
qRT
qRE
}

