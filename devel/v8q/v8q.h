/*
Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#ifndef V8Q_INC_
# define V8Q_INC_

# define V8Q_NAME		"V8Q"

# if defined( E_DEBUG ) && !defined( V8Q_NODBG )
#  define V8Q_DBG
# endif

# include "err.h"
# include "flx.h"
# include "str.h"
# include "txf.h"

// If 'TOL' was already included, we have to 'undefine' the 'system' macro it creates due to 'v8.h' using this function.
# ifdef TOL_SYSTEM_MACRO
#  undef system
# endif
// Note to developer : include 'h:\hg\NJSIncludeDirectories.props' in the '.vcxproj'.
# include <v8.h>
// Restoring the 'system' macro defined by 'TOL', if it was already included.
# ifdef TOL_SYSTEM_MACRO
#  define system TOL_SYSTEM_MACRO
# endif
// Put after above line due to redefinition of 'system(...)'.
# include "tol.h"

namespace v8q {
	void SetGlobalIsolate( v8::Isolate *Isolate );

	v8::Isolate *GetGlobalIsolate( void );

	inline v8::Isolate *GetIsolate_( v8::Isolate *Isolate )
	{
		if ( Isolate == NULL )
			return GetGlobalIsolate();
		else
			return Isolate;
	}

	inline v8::Local<v8::Context> GetContext_(v8::Isolate *Isolate)
	{
		return GetIsolate_( Isolate )->GetCurrentContext();
	}

	template <typename t> inline v8::Local<t> ToLocal(
		v8::MaybeLocal<t> V,
		v8::Isolate *Isolate = NULL )
	{
		if ( V.IsEmpty() )
			return v8::Null( GetIsolate_( Isolate ) );
		else
			return V.ToLocalChecked();
	}

	inline v8::Local<v8::Object> ToLocal(
		v8::MaybeLocal<v8::Object> Object,
		v8::Isolate *Isolate = NULL )
	{
		if ( Object.IsEmpty() )
			qRFwk();

		return Object.ToLocalChecked();
	}

	inline v8::Local<v8::String> ToLocal(
		v8::MaybeLocal<v8::String> String,
		v8::Isolate *Isolate = NULL )
	{
		if ( String.IsEmpty() )
			qRFwk();

		return String.ToLocalChecked();
	}

	inline v8::Local<v8::String> ToString(
		const char *String,
		v8::Isolate *Isolate = NULL )
	{
		return ToLocal( v8::String::NewFromUtf8( GetIsolate_( Isolate ), String, v8::NewStringType::kNormal ) );
	}

	inline v8::Local<v8::String> ToString(
		const str::dString &String,
		v8::Isolate *Isolate = NULL )
	{
		v8::Local<v8::String> Result;
	qRH
		qCBUFFERh Buffer;
	qRB
		Result = ToString( String.Convert( Buffer ) );
	qRR
	qRT
	qRE
		return Result;
	}

	inline v8::Local<v8::Array> ToArray(
		const str::dStrings &Strings,
		v8::Isolate *Isolate = NULL )
	{
		sdr::sRow Row = Strings.First();
		Isolate = GetIsolate_( Isolate );

		v8::Local<v8::Array> Array = v8::Array::New( Isolate, Strings.Amount() );

		while ( Row != qNIL ) {
			if ( !Array->Set( GetContext_( Isolate ), *Row, ToString( Strings( Row ), Isolate ) ).FromJust() )
				qRFwk();

			Row = Strings.Next( Row );
		}

		return Array;
	}

	template <typename arg> inline void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const arg &Arg )
	{
		argv[Position] = Arg.Core(Isolate);
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
		argv[Position] = ToString( Arg, Isolate );
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
		Set_( Isolate, argv, Position, String.Convert( Buffer ) );
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

		Set_( Isolate, argv, Position + 1, Args... );
	}

	template <typename t> t Expose( v8::Maybe<t> V )
	{
		if ( !V.IsJust() )
			qRFwk();

		return V.FromJust();
	}

	inline bso::sBool IsExternal( v8::Local<v8::Value> Value )
	{
		return Value->IsExternal();
	}

	inline bso::sBool IsExternal( v8::MaybeLocal<v8::Value> Value )
	{
		return IsExternal( Value.ToLocalChecked() );
	}

	inline v8::Local<v8::External> ToExternal( v8::Local<v8::Value> Value )
	{
		if ( !IsExternal( Value ) )
			qRFwk();

		return v8::Local<v8::External>::Cast( Value );
	}

	inline v8::Local<v8::External> ToExternal(
		v8::MaybeLocal<v8::Value> Value,
		v8::Isolate *Isolate = NULL )
	{
		return ToExternal( ToLocal( Value, Isolate ) );
	}

	inline v8::Local<v8::External> ToExternal(
		void *Value,
		v8::Isolate *Isolate = NULL )
	{
		Isolate = GetIsolate_( Isolate );

		return ToExternal( v8::External::New( Isolate, Value ), Isolate );
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
		Isolate = GetIsolate_( Isolate );
		return ToFunction( Object->Get( Context, ToString( Key, Isolate ) ), Isolate );
	}

	inline v8::Local<v8::Function> GetFunction(
		v8::Local<v8::Object> Object,
		const char *Key,
		v8::Isolate *Isolate = NULL )
	{
		return GetFunction( GetContext_( Isolate ), Object, Key, Isolate );
	}

	inline v8::Local<v8::Script> ToLocal(
		v8::MaybeLocal<v8::Script> Script,
		v8::Isolate *Isolate = NULL )
	{
		if ( Script.IsEmpty() )
			qRFwk();

		return Script.ToLocalChecked();
	}

	inline v8::Local<v8::Value> Execute(
		const char *Script,
		v8::Isolate *Isolate = NULL )
	{
		Isolate = GetIsolate_( Isolate );

		v8::Local<v8::Context> Context = GetContext_( Isolate );
		return ToLocal( ToLocal( v8::Script::Compile( Context, ToString( Script, Isolate ) ), Isolate )->Run( Context ), Isolate );
	}

	template <typename type> v8::Local<type> Cast_(
		v8::Local<v8::Value> Value,
		bso::sBool UndefinedForbidden,
		bool (v8::Value::*Function)(void) const )
	{
		if ( ((*Value)->*Function)() )
			return v8::Local<type>::Cast( Value );
		else if ( UndefinedForbidden )
			qRFwk();

		return v8::Local<type>();
	}

	template <typename item> class rPData_
	{
	private:
		v8::Persistent<item> Core_;
	protected:
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			bool (v8::Value::*Function)(void) const,
			v8::Isolate *Isolate = NULL )
		{
			Core_.Reset( GetIsolate_( Isolate ), Cast_<item>( Value, UndefinedForbidden, Function ) );
		}
		v8::Local<item> Get_( v8::Isolate *Isolate ) const
		{
			return Core_.Get( GetIsolate_( Isolate ) );

			// Above didn't exist in some version of Node.js, Below was the used.
			// This is the definition of v8::Persistent<>::Get()' in 'Node.js' >v4.
			return v8::Local<item>::New( Isolate, Core_ );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.Reset();
		}
		qCDTOR( rPData_ );
		void Init( void )
		{
			Core_.Reset();
		}
		v8::Local<item> Core(v8::Isolate *Isolate = NULL) const
		{
			if ( Core_.IsEmpty() )
				qRFwk();

			return Get_(GetIsolate_(Isolate));
		}
		bso::sBool IsEmpty( v8::Isolate *Isolate = NULL ) const
		{
			if ( Core_.IsEmpty() )
				return true;
			else {
				v8::Local<item> Item = Get_( GetIsolate_( Isolate ) );

				if ( Item->IsNull() )
					return true;
				else
					return Item->IsUndefined();
			}
		}
	};

	template <typename item> class sLData_
	{
	private:
		v8::Local<item> Core_;
	protected:
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			bool (v8::Value::*Function)(void) const,
			v8::Isolate *Isolate = NULL )
		{
			Core_ = Cast_<item>( Value, UndefinedForbidden, Function );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.Clear();
		}
		qCDTOR( sLData_ );
		void Init( void )
		{
			Core_.Clear();
		}
		v8::Local<item> Core(v8::Isolate *Isolate = NULL) const
		{
			return Core_;
		}
		bso::sBool IsEmpty(void) const
		{
			if ( Core_.IsEmpty() )
				return true;
			else if ( Core()->IsNull() )
				return true;
			else
				return Core()->IsUndefined();
		}
	};

	template <typename data> class xValue_
	: public data
	{
	public:
		using data::reset;
		using data::Core;
		using data::IsEmpty;
		qCDTOR( xValue_ );
		xValue_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			data::Init( Value, Isolate );
		}
		using data::Init;
	};

# ifdef T
#  define V8Q_T_BUFFER_T_ T
#  undef T
# endif

# define T( name, alias )\
	template <typename item> qTCLONE( x##alias##_<rP##name##_<item>>, rP##alias##_ );\
	template <typename item> qTCLONE( x##alias##_<sL##name##_<item>>, sL##alias##_ );\
	typedef rP##alias##_<v8::alias> rP##alias;\
	typedef sL##alias##_<v8::alias> sL##alias

	T( Data, Value );

/*
	template <typename item> qTCLONE( xValue_<rPersistentData_<item>>, rPersistentValue_ );
	template <typename item> qTCLONE( xValue_<sLocalData_<item>>, sLocalValue_ );

	typedef rPersistentValue_<v8::Value> rPersistentValue;
	typedef sLocalValue_<v8::Value> sLocalValue;
*/
	// Predeclaration.
	template <typename object> class xFunction_;

	template <typename value> class xObject_
	: public value
	{
	protected:
		v8::Local<v8::Value> Launch_(
			const char *Method,
			v8::Isolate *Isolate,
			int Argc,
			v8::Local<v8::Value> Argv[] ) const
		{
      v8::Local<v8::Function> Function = GetFunction(Core(Isolate), Method, Isolate);

			return ToLocal( Function->Call(GetContext_(Isolate), Core(Isolate), Argc, Argv), Isolate);
		}
	public:
		using value::reset;
		using value::Core;
		qCDTOR( xObject_ );
		template <typename data> xObject_(
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value.Core(), Isolate );
		}
		xObject_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		void Init( void )
		{
			value::Init();
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			bool (v8::Value::*Function)(void) const,
			v8::Isolate *Isolate = NULL )
		{
			value::Init( Value, UndefinedForbidden, Function, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, UndefinedForbidden, &v8::Value::IsObject, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		v8::Local<v8::Value> Get(
			const char *Key,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate_( Isolate );

			return ToLocal(Core(Isolate)->Get( GetContext_(Isolate), v8q::ToString(Key, Isolate) ), Isolate);
		}
		bso::sBool Set(
			const char *Key,
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL,
			qRPD )
		{
			Isolate = GetIsolate_( Isolate );

			if ( !Expose(Core(Isolate)->Set(GetContext_(Isolate), v8q::ToString(Key, Isolate), Value) ) ) {
				if ( qRPU )
					qRFwk();
				else
					return false;
			}

			return true;
		}
		template <typename data> bso::sBool Set(
			const char *Key,
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL,
			qRPD )
		{
		  Isolate = GetIsolate_(Isolate);

			return Set(Key, Value.Core(Isolate), Isolate, qRP);
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
			v8::Local<v8::Value> Argv[1 + sizeof...( Args )];

			Isolate = GetIsolate_( Isolate );

			Set_( Isolate, Argv, 0, Arg, Args... );

			return Launch_( Method, Isolate, 1 + sizeof...( Args ), Argv );
		}
		template <typename ...args> v8::Local<v8::Value> Launch(
			const char *Method,
			const args &...Args ) const
		{
			return Launch( Method, ( v8::Isolate * )NULL, Args... );
		}
		v8::Local<v8::Value> Launch(
			const char *Method,
			v8::Isolate *Isolate = NULL ) const
		{
			return Launch_( Method, GetIsolate_( Isolate ), 0, NULL );
		}
		template <typename ...args> void Emit(
			const char *Event,
			v8::Isolate *Isolate,
			const args &...Args ) const
		{
			Launch( "emit", Isolate, Event, Args... );
		}
		template <typename ...args> void Emit(
			const char *Event,
			const args &...Args ) const
		{
			return Emit( Event, ( v8::Isolate * )NULL, Args... );
		}
		template <typename arg> void EmitError(
			const arg &Arg,
			v8::Isolate *Isolate = NULL )
		{
			return Emit( "error", Isolate, Arg );
		}
		template <typename object> void On(
			const char *Event,
			const class xFunction_<object> &Callback,
			v8::Isolate *Isolate = NULL ) const
		{
			Launch( "on", Isolate, Event, Callback );
		}
	};

	T( Value, Object );

	template <typename object> class xFunction_
	: public object
	{
	public:
		using object::reset;
		using object::Core;
		qCDTOR( xFunction_ );
		xFunction_(
			v8::FunctionCallback Callback,
			v8::Isolate *Isolate = NULL )
		{
			Init( Callback, Isolate );
		}
		xFunction_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			object::Init( Value, UndefinedForbidden, &v8::Value::IsFunction, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			v8::FunctionCallback Function,
			v8::Isolate *Isolate = NULL )
		{
			v8::Local<v8::Value> LocalBuffer;

			if ( !v8::FunctionTemplate::New( GetIsolate_( Isolate ), Function )->GetFunction( GetContext_(Isolate)).ToLocal(&LocalBuffer) )
				qRFwk();

			Init( LocalBuffer );
		}
		void Launch(
			v8::Local<v8::Value> *Argv,
			int Argc,
			v8::Local<v8::Value> *Return,
			v8::Isolate *Isolate = NULL )
		{
		  Isolate = GetIsolate_(Isolate);

			if ( !Core(Isolate)->Call(GetContext_(Isolate), Core(Isolate), Argc, Argv ).ToLocal( Return))
				qRFwk();
		}
		template <typename arg, typename ...args> v8::Local<v8::Value> Launch(
			v8::Isolate *Isolate,
			const arg &Arg,
			const args &...Args )
		{
			v8::Local<v8::Value> Argv[1 + sizeof...( Args )];

			Set_( GetIsolate_( Isolate ), Argv, 0, Arg, Args... );

			return Launch( Argv, 1 + sizeof...( Args ) );
		}
		template <typename ...args> v8::Local<v8::Value> Launch( args &...Args )
		{
			return Launch( NULL, Args... );
		}
		v8::Local<v8::Value> Launch(void)
		{
			return Core()->Call(Core(), 0, NULL);
		}
	};

	T( Object, Function );

	template <typename value> qTCLONE( value, xPrimitive_ );

	T( Value, Primitive );

	template <typename primitive> qTCLONE( primitive, xName_ );

	T( Primitive, Name );

	template <typename name> class xString_
	: public name
	{
	public:
		using name::reset;
		using name::Core;
		qCDTOR( xString_ );
		xString_(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		xString_(
			const str::dString &String,
			v8::Isolate *Isolate = NULL )
		{
			Init( String, Isolate );
		}
		xString_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		void Init( void )
		{
			name::Init();
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			name::Init( Value, UndefinedForbidden, &v8::Value::IsString, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			const char *String,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate_( Isolate );

			Init( ToString( String, Isolate ), Isolate );
		}
		void Init(
			const str::dString &String,
			v8::Isolate *Isolate = NULL )
		{
		qRH;
			TOL_CBUFFER___ Buffer;
		qRB;
			Init( String.Convert( Buffer ), Isolate );
		qRR;
		qRT;
		qRE;
		}
		// NOT the number of char, but the size of the string in bytes, WITHOUT NULL terminating char.
		int Size( v8::Isolate *Isolate = NULL ) const
		{
		  Isolate = GetIsolate_(Isolate);

			return Core(Isolate)->Utf8Length(Isolate);
		}
		int Get(
			char *Buffer,
			v8::Isolate *Isolate = NULL) const
		{
		  Isolate = GetIsolate_(Isolate);

			return Core(Isolate)->WriteUtf8(Isolate, Buffer);
		}
		// Defined at bottom of this file.
		void Get( str::dString &String );
	};

	T( Name, String );

	template <typename primitive> class xBoolean_
	: public primitive
	{
	public:
		using primitive::reset;
		using primitive::Core;
		qCDTOR( xBoolean_ );
		xBoolean_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		template <typename data> xBoolean_(
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value.Core(), Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			primitive::Init( Value, UndefinedForbidden, &v8::Value::IsBoolean, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			bool Boolean,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate_( Isolate );
			Init( v8::Boolean::New( Isolate, Boolean ), Isolate );
		}
		bso::sBool operator *( void )
		{
			return Core()->Value();
		}
	};

	T( Primitive, Boolean );

	template <typename primitive> class xInt32_
	: public primitive
	{
	public:
		using primitive::reset;
		using primitive::Core;
		qCDTOR( xInt32_ );
		xInt32_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		template <typename data> xInt32_(
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value.Core(), Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			primitive::Init( Value, UndefinedForbidden, &v8::Value::IsUint32, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			uint32_t Uint32,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate_( Isolate );
			Init( v8::Uint32::New( Isolate, Uint32 ), Isolate );
		}
		bso::sBool operator *( void )
		{
			return Core()->Value();
		}
	};

	T( Primitive, Int32 );

	template <typename primitive> class xNumber_
	: public primitive
	{
	public:
		using primitive::reset;
		using primitive::Core;
		qCDTOR( xNumber_ );
		xNumber_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		template <typename data> xNumber_(
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value.Core(), Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			primitive::Init( Value, UndefinedForbidden, &v8::Value::IsNumber, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			double Number,
			v8::Isolate *Isolate = NULL )
		{
			Init( v8::Number::New( GetIsolate_( Isolate ), Number ) );
		}
		double operator *( void )
		{
			return Core()->Value();
		}
	};

	T( Primitive, Number );

# undef T

# ifdef V8Q_T_BUFFER_T_
#  define T V8Q_T_BUFFER_T_
#  undef V8Q_T_BUFFER_T_
# endif

	template <typename value, typename type> class xExternal_
	: public value {
	public:
		using value::reset;
		using value::Core;
		qCDTOR( xExternal_ );
		xExternal_(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, Isolate );
		}
		template <typename data> xExternal_(
			xValue_<data> &Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value.Core(), Isolate );
		}
		xExternal_(
			const type *External,
			v8::Isolate *Isolate = NULL )
		{
			Init( External, Isolate );
		}
		void Init( void )
		{
			value::Init();
		}
		void Init(
			v8::Local<v8::Value> Value,
			bso::sBool UndefinedForbidden,
			v8::Isolate *Isolate = NULL )
		{
			value::Init( Value, UndefinedForbidden, &v8::Value::IsExternal, Isolate );
		}
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate = NULL )
		{
			Init( Value, true, Isolate );
		}
		void Init(
			const type *External,
			v8::Isolate *Isolate = NULL )
		{
			Isolate = GetIsolate_( Isolate );
			Init( v8::External::New( GetIsolate_( Isolate ), (void *)External ), Isolate );
		}
		type *Value( void ) const
		{
			return (type *)Core()->Value();
		}
	};

	template <typename item, typename type> qTCLONE( qCOVER2( xExternal_<rPValue_<item>, type > ), rPExternal_ );
	template <typename item, typename type> qTCLONE( xExternal_<qCOVER2( sLValue_<item>, type )>, sLExternal_ );

	template <typename type> qTCLONE( rPExternal_<qCOVER2( v8::External, type )>, rPExternal );
	template <typename type> qTCLONE( sLExternal_<qCOVER2( v8::External, type )>, sLExternal );

	typedef v8::FunctionCallbackInfo<v8::Value> sFunctionInfos;

	namespace global {
		template <typename value> inline xObject_<value> &GetGlobal(
			xObject_<value> &Global,
			v8::Isolate *Isolate = NULL )
		{
			Global.Init( v8q::GetContext_( Isolate )->Global() );

			return Global;
		}

		template <typename value> inline xObject_<value> &Get(
			const char *Name,
			xObject_<value> &Object,
			v8::Isolate *Isolate = NULL )
		{
			sLObject Global;

			Isolate = GetIsolate_( Isolate );

			Global.Init();

			Object.Init( GetGlobal( Global, Isolate ).Get( Name, Isolate ) );

			return Object;
		}
	}

	namespace console {
		template <typename value> inline xObject_<value> &GetConsole(
			xObject_<value> &Console,
			v8::Isolate *Isolate = NULL )
		{
			return global::Get( "console", Console, Isolate );
		}

		template <typename ...args> inline void Log(
			v8::Isolate *Isolate,
			const args &...Args )
		{
			sLObject Console;

			Isolate = GetIsolate_( Isolate );

			Console.Init();

			GetConsole( Console, Isolate ).Launch( "log", Isolate, Args... );
		}

		template <typename ...args> inline void Log( const args &...Args )
		{
			Log( ( v8::Isolate * )NULL, Args... );
		}
	}

	namespace process {
		template <typename value> inline xObject_<value> &GetProcess(
			xObject_<value> &Process,
			v8::Isolate *Isolate = NULL )
		{
			return global::Get( "process", Process, Isolate );
		}

		template <typename object, typename ...args> inline void NextTick(
			const xFunction_<object> &Function,
			v8::Isolate *Isolate,
			const args &...Args )
		{
			sLObject Process;

			Isolate = GetIsolate_( Isolate );

			Process.Init();

			GetProcess( Process, Isolate ).Launch( "nextTick", Isolate, Function, Args... );
		}

		template <typename object, typename ...args> inline void NextTick(
			const xFunction_<object> &Function,
			const args &...Args )
		{
			NextTick( Function, ( v8::Isolate * )NULL, Args... );
		}
	}
}

template <typename name> txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const v8q::xString_<name> &String )
	{
qRH
	char *Buffer = NULL;
	int Size = 0;
qRB
	Buffer = (char *)malloc ( Size = String.Size() + 1 );

	if ( Buffer == NULL )
		qRAlc();

	if ( Size != String.Get( Buffer ) )
		qRFwk();

	Flow << Buffer;
qRR
qRT
	if ( Buffer != NULL )
		delete( Buffer );
qRE
	return Flow;
}

template <typename name> inline void v8q::xString_<name>::Get( str::dString &String )
{
qRH
	flx::rStringTOFlow Flow;
qRB
	Flow.Init( String );

	Flow << *this;
qRR
qRT
qRE
}

// Functions which should not be used internally.
namespace v8q {
	inline v8::Local<v8::Context> GetContext(v8::Isolate *Isolate = NULL)
	{
		return GetContext_(Isolate);
	}

	inline v8::Isolate *GetIsolate(v8::Isolate *Isolate = NULL)
	{
		return v8q::GetIsolate_(Isolate);
	}
}

#endif
