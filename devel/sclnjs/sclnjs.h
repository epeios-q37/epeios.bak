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
			if ( P ) {
				if ( Callback_ != NULL )
					delete Callback_;
			}

			Callback_ = NULL;
		}
		qCVDTOR( rCore_ );
		void Init( void )
		{
			reset();
			Callback_ = NULL;
		}
		void Assign( callback *Callback )
		{
			if ( HasAssignation() )
				qRFwk();

			Callback_ = Callback;
		}
		bso::sBool HasAssignation( void ) const
		{
			return Callback_ != NULL;
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
			return rCore_<callback>::C_().Get();
		}
		void Set( const type &Item )
		{
			rCore_<callback>::C_().Set( Item );
		}
	};

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

	typedef rBase_<str::dString, n4njs::cUString> rString;
	typedef rBase_<str::dStrings, n4njs::cUStrings> rStrings;
	typedef rBase_<n4njs::dCallbacks, n4njs::cUCallbacks> rCallbacks;
	typedef rObject_<n4njs::cUObject> rObject;
	typedef rObject_<n4njs::cUCallback> rCallback_;

	typedef n4njs::dArguments dArguments_;

	class dArguments
	: public dArguments_ {
	public:
		void TestAndAdd_(
			n4njs::eType Type,
			const void *Value )
		{
			n4njs::sArgument Argument;

			if ( Value == NULL )
				qRFwk();

			Argument.Init( Type, Value );

			dArguments_::Add( Argument );
		}
		template <typename callback> void TestAndAdd_(
			n4njs::eType Type,
			const rCore_<callback> &Base )
		{
			TestAndAdd_( Type, &Base.Callback() );
		}
		// Termination function for variadics.
		inline void TestAndAdd_( void )
		{}
		inline void TestAndAdd_( const int &Int )
		{
			TestAndAdd_( n4njs::tInt, &Int );
		}
		inline void TestAndAdd_( const str::dString &String )
		{
			TestAndAdd_( n4njs::tString, &String );
		}
		inline void TestAndAdd_( const str::wString &String )
		{
			TestAndAdd_( *String );
		}
		inline void TestAndAdd_( const str::dStrings &Strings )
		{
			TestAndAdd_( n4njs::tStrings, &Strings );
		}
		inline void TestAndAdd_( const str::wStrings &Strings )
		{
			TestAndAdd_( *Strings );
		}
		inline void TestAndAdd_( const rObject &Object )
		{
			TestAndAdd_( n4njs::tObject, Object );
		}
	public:
		inline void Add( void )
		{
			// Nothing to do; ease the use of variadics templates.
		}
		template <typename arg> inline void Add( const arg &Arg )
		{
			TestAndAdd_( Arg );
		}
		template <typename arg, typename ...args> void Add(
			const arg &Arg,
			const args &...Args )
		{
			TestAndAdd_( Arg );
			Add( Args... );
		}
		dArguments( s &S )
			: dArguments_( S )
		{}
	};

	qW( Arguments );

	typedef rCore_<n4njs::cUBuffer> rBuffer_;

	class rBuffer
	: public rBuffer_ {
	public:
		void ToString( str::dString &String )
		{
			return C_().ToString( String );
		}
	};

	typedef rObject_<n4njs::cURStream> rRStream_;

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

	// Launch with 'dArguments' as arguments.
	template <typename ret_item, typename ret> void Launch_(
		ret_item &ReturnItem,
		n4njs::eType ReturnType,
		n4njs::cUCallback &Callback,
		const dArguments &Arguments )
	{
	qRH;
		ret *Return = NULL;
	qRB;
		Return = (ret *)Callback.Launch( ReturnType, Arguments );

		ReturnItem.Assign( Return );
	qRR;
		if ( Return != NULL )
			delete Return;
	qRT;
	qRE;
	}

	// Launch with variadics as arguments.
	template <typename ret_item, typename ret, typename ...args> void Launch_(
		ret_item &ReturnItem,
		n4njs::eType ReturnType,
		n4njs::cUCallback &Callback,
		const args &...Args )
	{
	qRH;
		wArguments Arguments;
	qRB;
		Arguments.Init();
		Arguments.Add( Args... );

		Launch_<ret_item,ret>( ReturnItem, ReturnType, Callback, *Arguments );
	qRR;
	qRT;
	qRE;
	}


	void inline VoidLaunch(
		n4njs::cUCallback &Callback,
		const dArguments &Arguments )
	{
		Callback.Launch( n4njs::tVoid, Arguments );
	}

	template <typename ...args> void inline VoidLaunch(
		n4njs::cUCallback &Callback,
		const args &...Args )
	{
	qRH;
		wArguments Arguments;
	qRB;
		Arguments.Init();
		Arguments.Add( Args... );

		VoidLaunch( Callback, *Arguments );
	qRR;
	qRT;
	qRE;
	}

	inline void ObjectLaunch(
		rObject &Object,
		n4njs::cUCallback &Callback,
		const dArguments &Arguments )
	{
		Launch_<rObject, n4njs::cUObject>( Object, n4njs::tObject, Callback, Arguments );
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
		void VoidLaunch( const dArguments &Arguments )
		{
			return sclnjs::VoidLaunch( C_(), Arguments );
		}
		void VoidLaunch( const wArguments &Arguments )
		{
			return VoidLaunch( *Arguments );
		}
		template <typename ...args> void ObjectLaunch(
			rObject &Object,
			const args &...Args )
		{
			return sclnjs::ObjectLaunch( Object, C_(), Args... );
		}
		void ObjectLaunch(
			rObject &Object,
			const dArguments &Arguments )
		{
			return sclnjs::ObjectLaunch( Object, C_(), Arguments );
		}
		void ObjectLaunch(
			rObject &Object,
			const wArguments &Arguments )
		{
			return sclnjs::ObjectLaunch( Object, C_(), *Arguments );
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

	void SCLNJSInfo( txf::sWFlow &Flow );	// To define by user.
	void SCLNJSRegister( sRegistrar &Registrar );	// To define by user
}

// Required by g++ and Clang, (but not by VC++).
namespace scln4 {
	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rObject &Object );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rString &String );

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

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rStrings &Strings );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		sclnjs::rCallbacks &Callbacks );

	template <> void Get(
		int Index,
		cCaller_ &Caller,
		n4njs::cUCallback *&Callback );
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	sclnjs::rBuffer &Buffer );

# define SCLNJS_F( name ) void name( sclnjs::sCaller &Caller )

namespace scln4 {


}

#endif
