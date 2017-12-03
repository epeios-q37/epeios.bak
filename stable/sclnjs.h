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

	template <typename callback> class rBase_
	{
	private:
		callback *Callback_;
	protected:
		qRM( callback, C_, Callback_ );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P )
				if ( Callback_ != NULL )
					delete Callback_;

			tol::reset( P, Callback_ );
		}
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

	typedef rBase_<n4njs::cUCallback> rCallback_;

	inline void TestAndAdd_(
		n4njs::dArguments_ &Arguments,
		n4njs::eType Type,
		void *Value )
	{
		n4njs::sArgument_ Argument;

		if ( Value == NULL )
			qRFwk();

		Argument.Init( Type, Value );

		Arguments.Add( Argument );
	}

	template <typename callback> class rCore_
	: public rBase_<callback> {
	protected:
		using rBase_<callback>::C_;
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

	typedef rCore_<n4njs::cUObject> rObject;

	inline void TestAndAdd_( n4njs::dArguments_ &Arguments )
	{}

	inline void TestAndAdd_(
		n4njs::dArguments_ &Arguments,
		const int &Int )
	{
		TestAndAdd_( Arguments, n4njs::tInt, new int( Int ) );
	}

	inline void TestAndAdd_(
		n4njs::dArguments_ &Arguments,
		const str::dString &String )
	{
		TestAndAdd_( Arguments, n4njs::tString, new str::wString( String ) );
	}

	inline void TestAndAdd_(
		n4njs::dArguments_ &Arguments,
		const str::wString &String )
	{
		TestAndAdd_( Arguments, *String );
	}

	inline void TestAndAdd_(
		n4njs::dArguments_ &Arguments,
		const rObject &Object )
	{
		n4njs::sArgument_ Argument;

		Argument.Init( n4njs::tObject, &Object.Callback() );

		Arguments.Add( Argument );
	}

	inline void Add_( n4njs::dArguments_ &Arguments )
	{
		// Nothing to do; ease the use of variadics templates.
	}

	template <typename arg> inline void Add_(
		n4njs::dArguments_ &Arguments,
		const arg &Arg )
	{
		TestAndAdd_( Arguments, Arg );
	}

	template <typename arg, typename ...args> void Add_(
		n4njs::dArguments_ &Arguments,
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

	class rCallback
	: public rCallback_
	{
	private:
		template <typename item, typename callback,typename ...args> void Launch_(
			item &Item,
			n4njs::eType ReturnType,
			const args &...Args )
		{
		qRH;
			n4njs::wArguments_ Arguments;
			callback *Callback = NULL;
		qRB;
			Arguments.Init();
			Add_( Arguments, Args... );

			Callback = (callback *)C_().Launch( ReturnType, Arguments );

			Item.Assign( Callback );
		qRR;
			if ( Callback != NULL )
				delete Callback;
		qRT;
		qRE;
		}
	public:
		/*
		void Launch( void )
		{
		qRH;
			n4njs::wArguments_ Arguments;
		qRB;
			Arguments.Init();

			C_().Launch( n4njs::tVoid, Arguments );
		qRR;
		qRT;
		qRE;
		}
		*/
		template <typename arg, typename ...args> void VoidLaunch(
			const arg &Arg,
			const args &...Args )
		{
		qRH;
			n4njs::wArguments_ Arguments;
		qRB;
			Arguments.Init();
			Add_( Arguments, Arg, Args... );

			C_().Launch( n4njs::tVoid, Arguments );
		qRR;
		qRT;
		qRE;
		}
		template <typename ...args> void ObjectLaunch(
			rObject &Object,
			const args &...Args )
		{
			Launch_<rObject, n4njs::cUObject>( Object, n4njs::tObject, Args... );
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
	};

	typedef void ( fFunction )( sCaller &Caller );

	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	void SCLNJSInfo( txf::sOFlow &Flow );	// To define by user.
	void SCLNJSRegister( sRegistrar &Registrar );	// To define by user
}

// Declaration of the handling of it own types.
// Extends same namespace declared in 'scln4a.h'.
namespace scln4 {
	template <> void Get(
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
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	sclnjs::rBuffer &Buffer );

# define SCLNJS_F( name ) void name( sclnjs::sCaller &Caller )

#endif
