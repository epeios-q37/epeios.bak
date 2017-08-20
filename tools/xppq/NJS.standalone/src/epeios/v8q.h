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

// Note to developer : include 'h:\hg\NJSIncludeDirectories.props' in the '.vcxproj'.
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

	template <typename t> inline v8::Local<t> ToLocal(
		v8::MaybeLocal<t> V,
		v8::Isolate *Isolate = NULL )
	{
		if ( V.IsEmpty()  )
			return v8::Null( GetIsolate( Isolate ) );
		else
			return V.ToLocalChecked();
	}
	
	inline v8::Local<v8::Script> ToLocal(
		v8::MaybeLocal<v8::Script> Script,
		v8::Isolate *Isolate = NULL )
	{
		if ( Script.IsEmpty() )
			qRFwk();

		return Script.ToLocalChecked();
	}
	
	inline v8::Local<v8::Object> ToLocal(
		v8::MaybeLocal<v8::Object> Object,
		v8::Isolate *Isolate = NULL )
	{
		if ( Object.IsEmpty() )
			qRFwk();

		return Object.ToLocalChecked();
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

	inline v8::Local<v8::Function> ToFunction(
		v8::MaybeLocal<v8::Value> Value,
		v8::Isolate *Isolate = NULL )
	{
		return ToFunction( ToLocal( Value, Isolate ) );
	}


	inline v8::Local<v8::Function> GetFunction(
		v8::Local<v8::Context> Context,
		v8::Local<v8::Object> Object,
		const char *Key,
		v8::Isolate *Isolate = NULL )
	{
		Isolate = GetIsolate( Isolate );
		return ToFunction( Object->Get( Context, ToString( Key, Isolate ) ), Isolate );
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
		Isolate = GetIsolate( Isolate );

		v8::Local<v8::Context> Context = GetContext( Isolate );
		return ToLocal( ToLocal( v8::Script::Compile( Context, ToString( Script, Isolate ) ), Isolate )->Run( Context ), Isolate );
	}

	template <typename item> class sData_
	{
	private:
		v8::Local<item> Core_;
	protected:
		void Init( v8::Local<v8::Value> Value )
		{
			Core_ = v8::Local<item>::Cast( Value );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.Clear();
		}
		qCDTOR( sData_ );
		void Init( void )
		{
			Core_.Clear();
		}
		v8::Local<item> Core( void ) const
		{
			return Core_;
		}
	};

	template <typename item> class sValue_
	: public sData_<item>
	{
	public:
		qCDTOR( sValue_ );
		sValue_( v8::Local<v8::Value> Value )
		{
			sData_<item>::Init( Value );
		}
		using sData_<item>::Init;
		bso::sBool IsNull( void ) const
		{ 
			return Core()->IsNull();
		}
	};

	typedef sValue_<v8::Value> sValue;

	// Predeclaration.
	template <typename item> class sFunction_;

	template <typename item> class sObject_
	: public sValue_<item>
	{
	protected:
		v8::Local<v8::Value> Launch_(
			const char *Method,
			v8::Isolate *Isolate,
			int Argc,
			v8::Local<v8::Value> Argv[] ) const
		{
			v8::Local<v8::Function> Function = GetFunction( Core(), Method, Isolate );

			return ToLocal( Function->Call( GetContext( Isolate ), Core(), Argc, Argv ), Isolate );
		}
	public:
		qCDTOR( sObject_ );
		sObject_( sValue Value )
		{
			Init( Value.Core() );
		}
		sObject_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		using sValue_<item>::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsObject() )
				qRFwk();

			sValue_<item>::Init( Value );
		}
		v8::Local<v8::Value> Get(
			const char *Key,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate( Isolate );
			return ToLocal( Core()->Get( GetContext( Isolate ), v8q::ToString( Key, Isolate ) ), Isolate );
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
			sValue_<t> Value,
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

			Isolate = GetIsolate( Isolate );

			Set_( Isolate, Argv, 0, Arg, Args... );

			return Launch_( Method, Isolate, 1 + sizeof...( Args ), Argv );
		}
		template <typename ...args> v8::Local<v8::Value> Launch(
			const char *Method,
			const args &...Args ) const
		{
			return Launch( Method, (v8::Isolate *)NULL, Args... );
		}
		v8::Local<v8::Value> Launch(
			const char *Method,
			v8::Isolate *Isolate = NULL ) const
		{
			return Launch_( Method, GetIsolate( Isolate ), 0, NULL );
		}
		template <typename ...args> void Emit(
			const char *Event,
			v8::Isolate *Isolate,
			const args &...Args ) const
		{
			Launch("emit", Isolate, Event, Args... );
		}
		template <typename ...args> void Emit(
			const char *Event,
			const args &...Args ) const
		{
			return Emit( Event, (v8::Isolate *)NULL, Args... );
		}
		template <typename arg> void EmitError(
			const arg &Arg,
			v8::Isolate *Isolate = NULL )
		{
			return Emit( "error", Isolate, Arg );
		}
		void On(
			const char *Event,
			const class sFunction_<v8::Function> &Callback,
			v8::Isolate *Isolate = NULL ) const
		{
			Launch( "on", Isolate, Event, Callback );
		}
	};

	typedef sObject_<v8::Object> sObject;

	template <typename item> class sFunction_
	: public sObject_<item>
	{
	public:
		qCDTOR( sFunction_ );
		sFunction_( v8::FunctionCallback Callback )
		{
			Init( Callback );
		}
		sFunction_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		using sObject_<item>::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsFunction() )
				qRFwk();

			sObject_<item>::Init( Value );
		}
		void Init(
			v8::FunctionCallback Function,
			v8::Isolate *Isolate = NULL )
		{
			Init( v8::FunctionTemplate::New( GetIsolate( Isolate ), Function)->GetFunction() );
		}
		v8::Local<v8::Value> Launch(
			v8::Local<v8::Value> *Argv,
			int Argc )
		{
			return Core()->Call( Core(), Argc, Argv );
		}
		template <typename arg, typename ...args> v8::Local<v8::Value> Launch(
			v8::Isolate *Isolate,
			const arg &Arg,
			const args &...Args )
		{
			v8::Local<v8::Value> Argv[1+sizeof...( Args )];

			Set_( GetIsolate( Isolate ), Argv, 0, Arg, Args... );

			return Launch( Argv, 1 + sizeof...( Args ) );
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

	typedef sFunction_<v8::Function> sFunction;

	template <typename item> qTCLONE( sValue_<item>, sPrimitive_ );

	template <typename item> qTCLONE( sPrimitive_<item>, sName_ );

	template <typename item> class sString_
	: public sName_<item>
	{
	public:
		qCDTOR( sString_ );
		sString_(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		sString_(
			const str::dString &String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		sString_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		using sName_<item>::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsString() )
				qRFwk();

			sName_<item>::Init( Value );
		}
		void Init(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			sName_<item>::Init( ToString( String , Isolate ) );
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
		void Get( str::dString &String );
	};

	typedef sString_<v8::String> sString;

	template <typename item, typename type> class sExternal_
	: public sValue_<item>
	{
	public:
		qCDTOR( sExternal_ );
		sExternal_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sExternal_( sValue &Value )
		{
			Init( Value.Core() );
		}
		sExternal_( const type* External )
		{
			Init( External );
		}
		using sValue_<item>::Init;
		void Init(
			const type *External,
			v8::Isolate *Isolate = NULL )
		{
			sValue_<item>::Init( v8::External::New( GetIsolate( Isolate ), (void *)External ) );
		}
		type *Value( void ) const
		{
			return (type *)Core()->Value();
		}
	};

	template <typename type> class sExternal
	: public sExternal_<qCOVER2( v8::External, type )>
	{
	public:
		sExternal( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sExternal( sValue &Value )
		{
			Init( Value.Core() );
		}
		sExternal( const type* External )
		{
			Init( External );
		}
	};

	template <typename item> class sBoolean_
	: public sPrimitive_<item>
	{
	public:
		qCDTOR( sBoolean_ );
		sBoolean_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sBoolean_( sValue &Value )
		{
			Init( Value.Core() );
		}
		using sPrimitive_<item>::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsBoolean() )
				qRFwk();

			sPrimitive_<item>::Init( Value );
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

	typedef sBoolean_<v8::Boolean> sBoolean;

	template <typename item> class sNumber_
	: public sPrimitive_<item>
	{
	public:
		qCDTOR( sNumber_ );
		sNumber_( v8::Local<v8::Value> Value )
		{
			Init( Value );
		}
		sNumber_( sValue &Value )
		{
			Init( Value.Core() );
		}
		using sPrimitive_<item>::Init;
		void Init( v8::Local<v8::Value> Value )
		{
			if ( !Value->IsNumber() )
				qRFwk();

			sPrimitive_<item>::Init( Value );
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

	typedef sNumber_<v8::Number> sNumber;

	typedef v8::FunctionCallbackInfo<v8::Value> sFunctionInfos;

	namespace global {
		inline sObject &GetGlobal(
			sObject &Global,
			v8::Isolate *Isolate = NULL )
		{
			Global.Init( v8q::GetContext( Isolate )->Global() );

			return Global;
		}

		inline sObject &Get(
			const char *Name,
			sObject &Object,
			v8::Isolate *Isolate = NULL )
		{
			sObject Global;

			Isolate = GetIsolate( Isolate );

			Global.Init();

			Object.Init( GetGlobal( Global, Isolate ).Get( Name, Isolate ) );

			return Object;
		}
	}

	namespace console {
		inline sObject &GetConsole(
			sObject &Console,
			v8::Isolate *Isolate = NULL )
		{
			return global::Get( "console", Console, Isolate );
		}

		template <typename ...args> inline void Log(
			v8::Isolate *Isolate,
			const args &...Args )
		{
			sObject Console;

			Isolate = GetIsolate( Isolate );

			Console.Init();

			GetConsole( Console, Isolate ).Launch( "log", Isolate, Args... );
		}

		template <typename ...args> inline void Log( const args &...Args )
		{
			Log( (v8::Isolate *)NULL, Args... );
		}
	}

	namespace process {
		inline sObject &GetProcess(
			sObject &Process,
			v8::Isolate *Isolate = NULL )
		{
			return global::Get( "process", Process, Isolate );
		}

		template <typename ...args> inline void NextTick(
			const sFunction &Function,
			v8::Isolate *Isolate,
			const args &...Args )
		{
			sObject Process;

			Isolate = GetIsolate( Isolate );

			Process.Init();

			GetProcess( Process, Isolate ).Launch( "nextTick", Isolate, Function, Args... );
		}

		template <typename ...args> inline void NextTick(
			const sFunction &Function,
			const args &...Args )
		{
			NextTick( Function, (v8::Isolate *)NULL, Args... );
		}
	}
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const v8q::sString &String );


#endif
