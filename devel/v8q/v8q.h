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
# include "str.h"
# include "txf.h"

// Note to developer : you have to add '<path_to_node>/deps/v8/include' as 'Additional Include Directory' in the 'devel' related project.
# include <v8.h>
// Put after above line due to redefinition of 'System(...)'.
# include "tol.h"

namespace v8q {
	void SetGlobalIsolate( v8::Isolate *Isolate );

	v8::Isolate *GetGlobalIsolate( void );

	inline v8::Isolate *GetIsolate( v8::Isolate *Isolate = NULL )
	{
		if ( Isolate == NULL )
			return GetGlobalIsolate();
		else
			return Isolate;
	}

	inline v8::Local<v8::String> ToString(
		const char *String,
		v8::Isolate *Isolate = NULL )
	{
		return v8::String::NewFromUtf8( GetIsolate( Isolate ), String );
	}

	inline v8::Local<v8::String> ToString(
		const str::dString &String,
		v8::Isolate *Isolate = NULL )
	{
		v8::Local<v8::String> Result;
	qRH
		qCBUFFERr Buffer;
	qRB
		Result = ToString(String.Convert( Buffer ) );
	qRR
	qRT
	qRE
		return Result;
	}

	template <typename arg> inline void Set_(
		v8::Isolate *,
		v8::Local<v8::Value> *argv,
		int Position,
		const arg &Arg )
	{
		argv[Position] = Arg.Core();
	}

	template <typename t> inline void Set_(
		v8::Isolate *,
		v8::Local<v8::Value> *argv,
		int Position,
		const v8::Local<t> &Arg )
	{
		argv[Position] = Arg;
	}

	inline void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const char *Arg )
	{
		argv[Position] = ToString( Arg, Isolate  );
	}

	inline void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const str::dString &String )
	{
	qRH
		TOL_CBUFFER___ Buffer;
	qRB
		Set_(Isolate, argv, Position, String.Convert( Buffer ) );
	qRR
	qRT
	qRE
	}

	inline void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const str::wString &String )
	{
		return Set_( Isolate, argv, Position, *String );
	}

	template <typename arg, typename ...args> void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const arg &Arg,
		const args &...Args )
	{
		Set_( Isolate, argv, Position, Arg );

		Set_( Isolate, argv, Position+1, Args... );
	}

	template <typename t> inline v8::Local<t> ToLocal( v8::MaybeLocal<t> V )
	{
		if ( V.IsEmpty()  )
			qRFwk();

		return V.ToLocalChecked();
	}

	template <typename t> t Expose( v8::Maybe<t> V )
	{
		if ( !V.IsJust() )
			qRFwk();

		return V.FromJust();
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
			qRFwk();

		return v8::Local<v8::Function>::Cast( Value );
	}

	inline v8::Local<v8::Function> ToFunction( v8::MaybeLocal<v8::Value> Value )
	{
		return ToFunction(ToLocal( Value ) );
	}


	inline v8::Local<v8::Function> GetFunction(
		v8::Local<v8::Context> Context,
		v8::Local<v8::Object> Object,
		const char *Key,
		v8::Isolate *Isolate = NULL )
	{
		return ToFunction( Object->Get( Context, ToString( Key, Isolate ) ) );
	}

	inline v8::Local<v8::Context> GetContext( v8::Isolate *Isolate = NULL )
	{
		return GetIsolate( Isolate )->GetCurrentContext();
	}

	inline v8::Local<v8::Function> GetFunction(
		v8::Local<v8::Object> Object,
		const char *Key,
		v8::Isolate *Isolate = NULL )
	{
		return GetFunction( GetContext( Isolate ), Object, Key, Isolate );
	}


	inline v8::Local<v8::Value> Execute(
		const char *Script,
		v8::Isolate *Isolate = NULL )
	{
		v8::Local<v8::Context> Context = GetContext( Isolate );
		return ToLocal( ToLocal( v8::Script::Compile( Context, ToString( Script, Isolate ) ) )->Run( Context ) );
	}

	template <typename t> class sCore_
	{
	private:
		v8::Local<t> Core_;
	protected:
		void Init( v8::Local<v8::Value> Value )
		{
			Core_ = v8::Local<t>::Cast( Value );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.Clear();
		}
		qCDTOR( sCore_ );
		void Init( void )
		{
			Core_.Clear();
		}
		v8::Local<t> Core( void ) const
		{
			return Core_;
		}
	};

	typedef sCore_<v8::Value> sValue_;

	class sValue
	: public sValue_
	{
	public:
		qCDTOR( sValue );
		sValue( v8::Local<v8::Value> Value )
		{
			sValue_::Init( Value );
		}
		using sCore_<v8::Value>::Init;
		bso::sBool IsNull( void ) const
		{
			return Core()->IsNull();
		}
	};

	typedef sCore_<v8::Object> sObject_;

	class sObject
	: public sObject_
	{
	public:
		qCDTOR( sObject );
		sObject( sValue Value )
		{
			Init( Value.Core() );
		}
		using sObject_::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsObject() )
				qRFwk();

			sObject_::Init( Value );
		}
		v8::Local<v8::Value> Get(
			const char *Key,
			v8::Isolate *Isolate = NULL )
		{
			return ToLocal( Core()->Get( GetContext(), v8q::ToString( Key, Isolate ) ) );
		}
		bso::sBool Set(
			const char *Key,
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL,
			qRPD )
		{
			if ( !Expose( Core()->Set( GetContext(), v8q::ToString( Key, Isolate ), Value)) ) {
				if ( qRPU )
					qRFwk();
				else
					return false;
			}

			return true;
		}
		template <typename t> bso::sBool Set(
			const char *Key,
			sCore_<t> Value,
			v8::Isolate *Isolate = NULL,
			qRPD )
		{
			return Set( Key, Value.Core(), Isolate, qRP );
		}
		template <typename t> bso::sBool Set(
			const char *Key,
			t &Value,
			qRPN )
		{
			return Set( Key, Value, NULL, qRP );
		}
		template <typename arg, typename ...args> v8::Local<v8::Value> Launch(
			const char *Method,
			v8::Isolate *Isolate,
			const arg &Arg,
			const args &...Args ) const
		{
			v8::Local<v8::Value> Argv[1+sizeof...( Args )];

			Set_( GetIsolate( Isolate ), Argv, 0, Arg, Args... );

			v8::Local<v8::Function> Function = GetFunction( Core(), Method, Isolate );

			return ToLocal( Function->Call( GetContext(), Core(), 1 + sizeof...( Args ), Argv ) );
		}
		template <typename ...args> v8::Local<v8::Value> Launch(
			const char *Method,
			const args &...Args ) const
		{
			return Launch( Method, NULL, Args... );
		}
		v8::Local<v8::Value> Launch(
			const char *Method,
			v8::Isolate *Isolate = NULL ) const
		{
			v8::Local<v8::Function> Function = GetFunction( Core(), Method, Isolate );

			return Function->Call( Core(), 0, NULL );
		}
		void On(
			const char *Event,
			const class sFunction &Callback,
			v8::Isolate *Isolate = NULL ) const
		{
			Launch( "on", Isolate, Event, Callback );
		}
	};

	typedef sCore_<v8::Function> sFunction_;

	class sFunction
	: public sFunction_
	{
	public:
		qCDTOR( sFunction );
		sFunction( v8::FunctionCallback Callback )
		{
			Init( Callback );
		}
		sFunction( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		using sFunction_::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsFunction() )
				qRFwk();

			sFunction_::Init( Value );
		}
		void Init(
			v8::FunctionCallback Function,
			v8::Isolate *Isolate = NULL )
		{
			Init( v8::FunctionTemplate::New( GetIsolate( Isolate ), Function)->GetFunction() );
		}
		template <typename arg, typename ...args> v8::Local<v8::Value> Launch(
			v8::Isolate *Isolate,
			const arg &Arg,
			const args &...Args )
		{
			v8::Local<v8::Value> Argv[1+sizeof...( Args )];

			Set_( GetIsolate( Isolate ), Argv, 0, Arg, Args... );

			return Core()->Call( Core(), 1 + sizeof...( Args ), Argv );
		}
		template <typename ...args> v8::Local<v8::Value> Launch( args &...Args )
		{
			return Launch( NULL, Args... );
		}
		v8::Local<v8::Value> Launch( void )
		{
			return Core()->Call( Core(), 0, NULL );
		}
	};

	typedef sCore_<v8::String> sString_;

	class sString
	: public sString_
	{
	public:
		qCDTOR( sString );
		sString(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		sString(
			const str::dString &String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		sString( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		using sString_::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsString() )
				qRFwk();

			sString_::Init( Value );
		}
		void Init(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			sString_::Init( ToString( String , Isolate ) );
		}
		void Init(
			const str::dString &String,
			v8::Isolate *Isolate = NULL )
		{
		qRH
			TOL_CBUFFER___ Buffer;
		qRB
			Init( String.Convert( Buffer ), Isolate );
		qRR
		qRT
		qRE
		}
		// NOT the number of char, but the size of the string in bytes, WITHOUT NULL terminating char.
		int Size( void ) const
		{
			return Core()->Utf8Length();
		}
		int Get( char *Buffer ) const
		{
			return Core()->WriteUtf8( Buffer );
		}
	};

	typedef v8q::sCore_<v8::External> sExternal_;

	template <typename t> class sExternal
	: public v8q::sCore_<v8::External>
	{
	public:
		qCDTOR( sExternal );
		sExternal( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sExternal( sValue &Value )
		{
			Init( Value.Core() );
		}
		sExternal( const t* External )
		{
			Init( External );
		}
		using sExternal_::Init;
		void Init(
			const t *External,
			v8::Isolate *Isolate = NULL )
		{
			sExternal_::Init( v8::External::New( GetIsolate( Isolate ), (void *)External ) );
		}
		t *Value( void ) const
		{
			return (t *)Core()->Value();
		}
	};

	typedef sCore_<v8::Boolean> sBoolean_;

	class sBoolean
	: public sBoolean_
	{
	public:
		qCDTOR( sBoolean );
		sBoolean( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sBoolean( sValue &Value )
		{
			Init( Value.Core() );
		}
		using sBoolean_::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsBoolean() )
				qRFwk();

			sBoolean_::Init( Value );
		}
		void Init(
			bool Boolean,
			v8::Isolate *Isolate = NULL )
		{
			Init( v8::Boolean::New( GetIsolate( Isolate ), Boolean ) );
		}
		bso::sBool operator *(void)
		{
			return Core()->Value();
		}
	};

	typedef sCore_<v8::Number> sNumber_;

	class sNumber
	: public sNumber_
	{
	public:
		qCDTOR( sNumber );
		sNumber( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sNumber( sValue &Value )
		{
			Init( Value.Core() );
		}
		using sNumber_::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsNumber() )
				qRFwk();

			sNumber_::Init( Value );
		}
		void Init(
			double Number,
			v8::Isolate *Isolate = NULL )
		{
			Init( v8::Number::New( GetIsolate( Isolate ), Number ) );
		}
		double operator *(void)
		{
			return Core()->Value();
		}
	};


	template <typename item> inline void Get(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item )
	{
		if ( Index >= Infos.Length()  )
			qRFwk();

		Item.Init( Infos[Index] );
	}

	template <typename item> inline void Get_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item )
	{
		Get( Index, Infos, Item );
	}

	template <typename item, typename ...items> inline void Get_(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item,
		items &...Items )
	{
		Get( Index, Infos, Item );

		Get_( Index+1, Infos, Items... );
	}

	template <typename ...items> inline void Get(
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		items &...Items )
	{
		Get_( 0, Infos, Items... );
	}

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
		template <typename item> void Get(
			bso::sUInt Index,
			item &Item ) const
		{
			if ( Index == 0 )
				qRFwk();

			v8q::Get( Index, A_(), Item );
		}
		template <typename ...items> inline void Get( items &...Items ) const
		{
			Get_( 1, A_(), Items... );
		}
		void This( sObject &This )
		{
			This.Init( A_().This() );
		}
	};

	typedef v8::FunctionCallbackInfo<v8::Value> sFunctionInfos;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const v8q::sString &String );


#endif
