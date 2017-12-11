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

// SoCLe Node.JS

#ifndef SCLNJS_INC_
# define SCLNJS_INC_

# define SCLNJS_NAME		"SCLNJS"

# if defined( E_DEBUG ) && !defined( SCLNJS_NODBG )
#  define SCLNJS_DBG
# endif

# include "n4njs.h"
# include "scln4a.h"
# include "uvqdcl.h"

// Wrapping to 'UVQ' library;
# define SCLNJSWork		UVQWork
# define SCLNJSAfter	UVQAfter

namespace sclnjs {
	using n4njs::cAsync;
	using uvq::eBehavior;

	qCDEF( eBehavior, bExitAndDelete, uvq::bExitAndDelete );
	qCDEF( eBehavior, bExitOnly, uvq::bExitOnly );
	qCDEF( eBehavior, bRelaunch, uvq::bRelaunch );

	inline void ERRFinal( void )
	{
		// TODO.
		qRVct();
	}

	template <typename callback> class rCore_
	{
	protected:
		qRMV( callback, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P )
				if ( Callback_ != NULL )
					delete Callback_;

			tol::reset( P, Callback_ );
		}
		qCVDTOR( rCore_ );
		void Init( void )
		{
			Callback_ = NULL;
		}
		void Assign( callback *Callback )
		{
			Callback_ = Callback;
		}
		callback &Callback( void ) const
		{
			return C_();
		}
	};

	template <typename type, typename callback> class rBase_
	: public rCore_<callback>
	{
	public:
		const type &Get( void ) const
		{
			return C_().Get();
		}
		void Set( const type &Item )
		{
			C_().Set( Item );
		}
	};

	inline void TestAndAdd_(
		n4njs::dArguments &Arguments,
		n4njs::eType Type,
		const void *Value )
	{
		n4njs::sArgument Argument;

		if ( Value == NULL )
			qRFwk();

		Argument.Init( Type, Value );

		Arguments.Add( Argument );
	}

	template <typename callback> void TestAndAdd_(
		n4njs::dArguments &Arguments,
		n4njs::eType Type,
		const rCore_<callback> &Base )
	{
		TestAndAdd_( Arguments, Type, &Base.Callback() );
	}

	template <typename callback> class rObject_
	: public rCore_<callback> {
	protected:
		using rCore_<callback>::C_;
	public:
		void Set(
			const char *Key,
			void *Value )
		{
			return C_().Set( Key, Value );
		}
		void *Get( const char *Key )
		{
			return C_().Get( Key );
		}
		void EmitError( const str::dString &Message )
		{
			return C_().EmitError( Message );
		}

	};

	// Termination function for variadics.
	inline void TestAndAdd_( n4njs::dArguments &Arguments )
	{}

	inline void TestAndAdd_(
		n4njs::dArguments &Arguments,
		const int &Int )
	{
		TestAndAdd_( Arguments, n4njs::tInt, &Int );
	}

	inline void TestAndAdd_(
		n4njs::dArguments &Arguments,
		const str::dString &String )
	{
		TestAndAdd_( Arguments, n4njs::tString, &String );
	}

	inline void TestAndAdd_(
		n4njs::dArguments &Arguments,
		const str::wString &String )
	{
		TestAndAdd_( Arguments, *String );
	}

	typedef rBase_<str::dString, n4njs::cUString> rString;
	typedef rBase_<str::dStrings, n4njs::cUStrings> rStrings;
	typedef rBase_<n4njs::dCallbacks, n4njs::cUCallbacks> rCallbacks;
	typedef rObject_<n4njs::cUObject> rObject;
	typedef rObject_<n4njs::cUCallback> rCallback_;

	inline void TestAndAdd_(
		n4njs::dArguments &Arguments,
		const rObject &Object )
	{
		TestAndAdd_( Arguments, n4njs::tObject, Object );
	}

	inline void Add_( n4njs::dArguments &Arguments )
	{
		// Nothing to do; ease the use of variadics templates.
	}

	template <typename arg> inline void Add_(
		n4njs::dArguments &Arguments,
		const arg &Arg )
	{
		TestAndAdd_( Arguments, Arg );
	}

	template <typename arg, typename ...args> void Add_(
		n4njs::dArguments &Arguments,
		const arg &Arg,
		const args &...Args )
	{
		TestAndAdd_( Arguments, Arg );
		Add_( Arguments, Args... );
	}

	typedef rCore_<n4njs::cUBuffer> rBuffer_;

	class rBuffer
	: public rBuffer_ {
	public:
		void ToString( str::dString &String )
		{
			return C_().ToString( String );
		}
	};

	typedef rCore_<n4njs::cURStream> rRStream_;

	class rRStream
	: public rRStream_ {
	public:
		bso::sBool Read( str::dString &Chunk )
		{
			return C_().Read( Chunk );
		}
		bso::sBool Push(
			void *Buffer,
			bso::sSize Size )
		{
			return C_().Push( Buffer, Size );
		}
		bso::sBool Push( const str::dString &Buffer );
		void End( void )
		{
			return C_().End();
		}
	};

	template <typename ret_item, typename ret, typename ...args> void Launch_(
		ret_item &ReturnItem,
		n4njs::eType ReturnType,
		n4njs::cUCallback &Callback,
		const args &...Args )
	{
	qRH;
		n4njs::wArguments Arguments;
		ret *Return = NULL;
	qRB;
		Arguments.Init();
		Add_( Arguments, Args... );

		Return = (ret *)Callback.Launch( ReturnType, Arguments );

		ReturnItem.Assign( Return );
	qRR;
		if ( Return != NULL )
			delete Return;
	qRT;
	qRE;
	}

	template <typename ...args> void inline VoidLaunch(
		n4njs::cUCallback &Callback,
		const args &...Args )
	{
	qRH;
		n4njs::wArguments Arguments;
	qRB;
		Arguments.Init();
		Add_( Arguments, Args... );

		Callback.Launch( n4njs::tVoid, Arguments );
	qRR;
	qRT;
	qRE;
	}

	template <typename ...args> inline void ObjectLaunch(
		rObject &Object,
		n4njs::cUCallback &Callback,
		const args &...Args )
	{
		Launch_<rObject, n4njs::cUObject>( Object, n4njs::tObject, Callback, Args... );
	}

	class rCallback
	: public rCallback_
	{
	public:
		template <typename ...args> void VoidLaunch( const args &...Args )
		{
			return sclnjs::VoidLaunch( C_(), Args... );
		}
		template <typename ...args> void ObjectLaunch(
			rObject &Object,
			const args &...Args )
		{
			return sclnjs::ObjectLaunch( Object, C_(), Args... );
		}
	};

	void Launch( cAsync &Async );

	typedef scln4a::sCaller sCaller_;

	class sCaller
	: public sCaller_
	{
	private:
		int Index_;
		// Termination method.
		inline void GetArgument_( int Index )
		{}
		template <typename item, typename ...items> inline void GetArgument_(
			int Index,
			item &Item,
			items &...Items )
		{
			scln4::Get( Index, C_(), Item );

			GetArgument_( Index + 1, Items... );
		}
	public:
		void reset( bso::sBool P = true )
		{
			sCaller_::reset( P );
		}
		qCDTOR( sCaller );
		void Init( n4njs::cCaller &Callback )
		{
			sCaller_::Init( Callback );
			Index_ = 0;
		}
		template <typename item> void GetArgument(
			bso::sUInt Index,
			item &Item ) const
		{
			GetArgument_( Index, Item );
		}
		template <typename ...items> inline void GetArgument( items &...Items )
		{
			GetArgument_( Index_, Items... );
			Index_ += sizeof...( Items );
		}
		void SetReturnValue( const str::dString &Value )
		{
			C_().SetReturnValue( n4njs::tString, &Value );
		}
		void SetReturnValue( const str::dStrings &Values )
		{
			C_().SetReturnValue( n4njs::tStrings, &Values );
		}
	};

	typedef void ( fFunction )( sCaller &Caller );

	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	void SCLNJSInfo( txf::sOFlow &Flow );	// To define by user.
	void SCLNJSRegister( sRegistrar &Registrar );	// To define by user
}

// Declaration of the handling of it own types.
// Extends same namespace declared in 'scln4a.h'.
namespace scln4 {
/*	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rObject &Object );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rRStream &Stream );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rBuffer &Buffer );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rCallback &Callback );
		*/
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	sclnjs::rBuffer &Buffer );

# define SCLNJS_F( name ) void name( sclnjs::sCaller &Caller )

#endif
