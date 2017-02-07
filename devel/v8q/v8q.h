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

// V8 (from the JAvaScript engine of same name)

#ifndef V8Q__INC
# define V8Q__INC

# define V8Q_NAME		"V8Q"

# if defined( E_DEBUG ) && !defined( V8Q_NODBG )
#  define V8Q_DBG
# endif

# include "err.h"

# include <v8.h>

# include "tol.h"

namespace v8q {
	template <typename arg> void Set_(
		v8::Local<v8::Value> *argv,
		int Position,
		arg Arg )
	{
		argv[Position] = Arg;
	}

	template <typename arg, typename ...args> void Set_(
		v8::Local<v8::Value> *argv,
		int Position,
		arg Arg,
		args... Args )
	{
		argv[Position] = Arg;

		Set_( argv, Position+1, Args... );
	}

	template <typename t> v8::Local<t> ToLocal( v8::MaybeLocal<t> V )
	{
		if ( V.IsEmpty()  )
			qRGnr();

		return V.ToLocalChecked();
	}

	inline bso::sBool IsFunction( v8::Local<v8::Value> Value )
	{
		return Value->IsFunction();
	}

	inline bso::sBool IsFunction( v8::MaybeLocal<v8::Value> Value )
	{
		return IsFunction( Value.ToLocalChecked() );
	}

	inline v8::Local<v8::Function> ToFunction( v8::Local<v8::Value> Value )
	{
		if ( !IsFunction( Value ) )
			qRGnr();

		return v8::Local<v8::Function>::Cast( Value );
	}

	inline v8::Local<v8::Function> ToFunction( v8::MaybeLocal<v8::Value> Value )
	{
		return ToFunction(ToLocal( Value ) );
	}


	inline v8::Local<v8::Function> GetFunction(
		v8::Isolate *Isolate,
		v8::Local<v8::Context> Context,
		v8::Local<v8::Object> Object,
		const char *Key )
	{
		return ToFunction( Object->Get( Context, v8::String::NewFromUtf8( Isolate, Key, v8::String::NewStringType::kNormalString ) ) );
	}

	inline v8::Local<v8::Function> GetFunction(
		v8::Isolate *Isolate,
		v8::Local<v8::Object> Object,
		const char *Key )
	{
		return GetFunction(Isolate, Isolate->GetCurrentContext(), Object, Key );
	}


	class sObject {
	private:
		v8::Local<v8::Object> Object_;
		qRMV( v8::Isolate, I_, Isolate_ );
	public:
		void reset( bso::sBool P = true )
		{
			Object_.Clear();
			Isolate_ = NULL;
		}
		qCDTOR( sObject );
		void Init( void )
		{
			Object_.Clear();
			Isolate_ = NULL;
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			if ( !Value->IsObject() )
				qRGnr();

			Object_ = v8::Local<v8::Object>::Cast( Value );
			Isolate_ = Isolate;
		}
		template <typename ...args> inline v8::Local<v8::Value> Launch(
			const char *Method,
			args &...Args )
		{
			v8::Local<v8::Value> argv[sizeof...( Args )];

			Set_( argv, 0, Args... );

			v8::Local<v8::Function> Function = GetFunction( &I_(), Object_, Method );

			return Function->Call( Object_, sizeof...( Args ), argv );
		}
	};

	class sFunction {
	private:
		v8::Local<v8::Function> Function_;
		qRMV( v8::Isolate, I_, Isolate_ );
	public:
		void reset( bso::sBool P = true )
		{
			Function_.Clear();
			Isolate_ = NULL;
		}
		qCDTOR( sFunction );
		void Init( void )
		{
			Function_.Clear();
			Isolate_ = NULL;
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			if ( !Value->IsFunction() )
				qRGnr();

			Function_ = v8::Local<v8::Function>::Cast( Value );
			Isolate_ = Isolate;
		}
		v8::Local<v8::Function> Core( void ) const
		{
			return Function_;
		}
	};

	class sArguments {
	private:
		qRMV( const v8::FunctionCallbackInfo<v8::Value>, A_, Arguments_ );
	public:
		void reset( bso::sBool P = true )
		{
			Arguments_ = NULL;
		}
		qCDTOR( sArguments );
		void Init( const v8::FunctionCallbackInfo<v8::Value> &Arguments )
		{
			Arguments_ = &Arguments;
		}
		void Get(
			int Index,
			sObject &Object ) const
		{
			if ( Index == 0 )
				qRGnr();

			if ( Index >= A_().Length()  )
				qRGnr();

			Object.Init( A_()[Index], A_().GetIsolate() );
		}
		void Get(
			int Index,
			sFunction &Function ) const
		{
			if ( Index == 0 )
				qRGnr();

			if ( Index >= A_().Length()  )
				qRGnr();

			Function.Init( A_()[Index], A_().GetIsolate() );
		}
		v8::Isolate *GetIsolate( void ) const
		{
			return A_().GetIsolate();
		}
	};
}

#endif
