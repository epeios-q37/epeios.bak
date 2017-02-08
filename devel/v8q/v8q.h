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
# include "txf.h"

# include <v8.h>
// Put after above line due to redefinition of 'System(...)'.
# include "tol.h"

namespace v8q {
	inline v8::Local<v8::String> ToString(
		v8::Isolate *Isolate,
		const char *String )
	{
		return v8::String::NewFromUtf8( Isolate, String );
	}

	template <typename arg> inline void Set_(
		v8::Isolate *,
		v8::Local<v8::Value> *argv,
		int Position,
		arg Arg )
	{
		argv[Position] = Arg.Core();
	}

	template <typename t> inline void Set_(
		v8::Isolate *,
		v8::Local<v8::Value> *argv,
		int Position,
		v8::Local<t> Arg )
	{
		argv[Position] = Arg;
	}

	inline void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		const char *Arg )
	{
		argv[Position] =ToString( Isolate, Arg );
	}

	template <typename arg, typename ...args> void Set_(
		v8::Isolate *Isolate,
		v8::Local<v8::Value> *argv,
		int Position,
		arg Arg,
		args... Args )
	{
		Set_( Isolate, argv, Position, Arg );

		Set_( Isolate, argv, Position+1, Args... );
	}

	template <typename t> inline v8::Local<t> ToLocal( v8::MaybeLocal<t> V )
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
		return ToFunction( Object->Get( Context, ToString( Isolate, Key ) ) );
	}

	inline v8::Local<v8::Function> GetFunction(
		v8::Isolate *Isolate,
		v8::Local<v8::Object> Object,
		const char *Key )
	{
		return GetFunction( Isolate, Isolate->GetCurrentContext(), Object, Key );
	}

	inline v8::Local<v8::Context> GetContext( v8::Isolate *Isolate )
	{
		return Isolate->GetCurrentContext();
	}

	inline v8::Local<v8::Value> Execute(
		v8::Isolate *Isolate,
		const char *Script )
	{
		v8::Local<v8::Context> Context = GetContext( Isolate );
		return ToLocal( ToLocal( v8::Script::Compile( Context, ToString( Isolate, Script) ) )->Run( Context ) );
	}

	template <typename t> class sCore_
	{
	protected:
		qRMV( v8::Isolate, I_, Isolate_ );
		v8::Local<t> Core_;
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			Core_ = v8::Local<t>::Cast( Value );
			Isolate_ = Isolate;
		}
	public:
		void reset( bso::sBool P = true )
		{
			Core_.Clear();
			Isolate_ = NULL;
		}
		qCDTOR( sCore_ );
		void Init( void )
		{
			Core_.Clear();
			Isolate_ = NULL;
		}
		v8::Local<t> Core( void ) const
		{
			return Core_;
		}
	};

	class sObject
	: public sCore_<v8::Object>
	{
	public:
		qCDTOR( sObject );
		using sCore_<v8::Object>::Init;
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			if ( !Value->IsObject() )
				qRGnr();

			sCore_<v8::Object>::Init( Value, Isolate );
		}
		template <typename arg, typename ...args> v8::Local<v8::Value> Launch(
			const char *Method,
			arg &Arg,
			args &...Args )
		{
			v8::Local<v8::Value> Argv[1+sizeof...( Args )];

			Set_( &I_(), Argv, 0, Arg, Args... );

			v8::Local<v8::Function> Function = GetFunction( &I_(), Core_, Method );

			return Function->Call( Core_, 1 + sizeof...( Args ), Argv );
		}
		v8::Local<v8::Value> Launch( const char *Method )
		{
			v8::Local<v8::Function> Function = GetFunction( &I_(), Core_, Method );

			return Function->Call( Core_, 0, NULL );
		}
		void On(
			const char *Event,
			const class sFunction &Callback )
		{
			Launch( "on", Event, Callback );
		}
	};

	class sFunction
	: public sCore_<v8::Function>
	{
	public:
		qCDTOR( sFunction );
		using sCore_<v8::Function>::Init;
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			if ( !Value->IsFunction() )
				qRGnr();

			sCore_<v8::Function>::Init( Value, Isolate );
		}
		void Init(
			v8::FunctionCallback Function,
			v8::Isolate *Isolate )
		{
			Init( v8::FunctionTemplate::New(Isolate, Function)->GetFunction(), Isolate );
		}
	};

	class sString
	: public sCore_<v8::String>
	{
	public:
		qCDTOR( sString );
		using sCore_<v8::String>::Init;
		void Init(
			v8::Local<v8::Value> Value,
			v8::Isolate *Isolate )
		{
			if ( !Value->IsString() )
				qRGnr();

			sCore_<v8::String>::Init( Value, Isolate );
		}
		// NOT the number of char, but the size of the string in bytes, WITHOUT NULL terminating char.
		int Size( void ) const
		{
			return Core_->Utf8Length();
		}
		const char *Get( char *Buffer ) const
		{
			Core_->WriteUtf8( Buffer );

			return Buffer;
		}
	};

	template <typename item> inline void Get(
		int Index,
		const v8::FunctionCallbackInfo<v8::Value> Infos,
		item &Item )
	{
		if ( Index >= Infos.Length()  )
			qRGnr();

		Item.Init( Infos[Index], Infos.GetIsolate() );
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
				qRGnr();

			v8q::Get( Index, A_(), Item );
		}
		template <typename ...items> inline void Get( items &...Items ) const
		{
			Get_( 1, A_(), Items... );
		}
		void This( sObject &This )
		{
			This.Init( A_().This(), GetIsolate() );
		}
		v8::Isolate *GetIsolate( void ) const
		{
			return A_().GetIsolate();
		}
	};

	typedef v8::FunctionCallbackInfo<v8::Value> sFunctionInfos;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const v8q::sString &String );


#endif
