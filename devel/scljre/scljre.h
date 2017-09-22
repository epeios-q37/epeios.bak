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

// Socle Java Runtime Environment

#ifndef SCLJRE_INC_
# define SCLJRE_INC_

# define SCLJRE_NAME		"SCLJRE"

# if defined( E_DEBUG ) && !defined( SCLJRE_NODBG )
#  define SCLJRE_DBG
# endif

# include "n4jre.h"
# include "scln4a.h"

#ifdef H
#  define SCLJRE_H_ H
#  undef H
# endif

namespace scljre {
	using namespace n4jre;

	extern n4jre::fNew_Object New_Object_;
	extern n4jre::fThrow Throw_;

	// Termination method.
	inline void Fill_(
		int Indice,
		sValue *Values )
	{}

	template <typename arg, typename ...args> inline void Fill_(
		int Indice,
		sValue *Values,
		arg &Arg,
		args&... Args )
	{
		Values[Indice].Init( Arg );
		Fill_( Indice + 1, Values, Args... );
	}

	template <typename ...args> cObject *New(
		const char *Class,
		const char *Signature,
		args&... Args )
	{
		sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.
		Values[sizeof...( args )].Type = t_Undefined;

		Fill_( 0, Values, Args... );

		return New_Object_( Class, Signature, sizeof...( args ), Values );
	}

	extern n4jre::fDelete Delete_;

	inline void Throw( const char *Message )
	{
		return Throw_( Message );
	}

	void Throw( const str::dString &Text );

	class rObject {
	private:
		qRMV( cObject, O_, Object_ );
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Object_ != NULL )
					Delete_( Object_ );
			}

			Object_ = NULL;
		}
		qCDTOR( rObject );
		void Init( n4jre::cObject *Object )
		{
			reset();

			Object_ = Object;
		}
		cObject *operator()( void )
		{
			return &O_();
		}
		void Set(
			const char *Name,
			const char *Signature,
			sJObject Value )
		{
			return O_().Set( Name, Signature, Value );
		}
		template <typename object> void Set(
			const char *Name,
			object &Object )
		{
			qRVct();
			Set( Name, NULL, Object );
		}
		template <typename object, typename ...args> void CallObjectMethod(
			const char *Method,
			const char *Signature,
			object &Object,
			args&... Args )
		{
			sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.
			sValue Return;

			Values[sizeof...( args )].Type = t_Undefined;

			Fill_( 0, Values, Args... );

			Return.Init( Object );

			O_().CallObjectMethod( Method, Signature, Return, sizeof...( args ), Values );
		}
		template <typename ...args> sJInt CallIntMethod(
			const char *Method,
			const char *Signature,
			args&... Args )
		{
			sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.
			Values[sizeof...( args )].Type = t_Undefined;

			Fill_( 0, Values, Args... );

			return O_().CallIntMethod( Method, Signature, sizeof...( args ), Values );
		}
		template <typename ...args> sJLong CallLongMethod(
			const char *Method,
			const char *Signature,
			args&... Args )
		{
			sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.

			Fill_( 0, Values, Args... );

			return O_().CallLongMethod( Method, Signature, sizeof...( args ), Values );
		}
	};

	class rCore_ {
	protected:
		rObject Object_;
	public:
		void reset( bso::sBool P = true )
		{
			Object_.reset( P );
		}
		qCDTOR( rCore_ );
		void Init( n4jre::cObject *Object )
		{
			Object_.Init( Object );
		}
		cObject *operator()( void )
		{
			return Object_.operator()();
		}
	};


# ifdef B
#  define SCKJRE_B_ B
# undef B
# endif

# ifdef A
#  define SCKJRE_A_ A
# undef A
# endif

// Before
# define B( name )\
	class r##name\
	: public rCore_\
	{\
	public:\
		using rCore_::Init

// After
# define A	}


	namespace java {
		namespace io {
			B( InputStream );
			sJInt Read( void )
			{
				return Object_.CallIntMethod( "read", "()I" );
			}
			sJInt Read(
				rJByteArray &b,
				sJInt off,
				sJInt len )
			{
				return Object_.CallIntMethod( "read", "([BII)I", b, off, len );
			}
			A;
		}

		namespace lang {
			B( Integer );
				void Init( sJInt Integer )
				{
					Init( New( "Ljava/lang/Integer;", "(I)V", Integer ) );
				}
				sJInt IntValue( void )
				{
					return Object_.CallIntMethod( "intValue", "()I" );
				}
			A;

			B( Long );
				void Init( sJLong Long )
				{
					Init( New( "Ljava/lang/Long;", "(J)V", Long ) );
				}
				sJLong LongValue( void )
				{
					return Object_.CallLongMethod( "longValue", "()J" );
				}
			A;

			B( String );
				void Init(
					rJByteArray &bytes,
					rJString &charsetName )
				{
					Init( New( "Ljava/lang/String;", "([BLjava/lang/String;)V", bytes, charsetName ) );
				}
				void GetBytes(
					rJString &charsetName,
					rJByteArray &Result )
				{
					return Object_.CallObjectMethod( "getBytes", "(Ljava/lang/String;)[B", Result, charsetName );
				}
			A;
		}
	}

# undef B
# undef A

# ifdef SCLJRE_B_
# define B SCLJRE_B_
# endif

# ifdef SCLJRE_A_
# define A SCLJRE_A_
# endif

	sJObject Null( void );
	sJObject Integer( sJInt Integer );
	sJObject Long( sJLong Long );
	sJObject String( const str::dString &UTF );

	typedef fdr::rIDressedDriver rIDriver_;

	class rInputStreamIDriver
	: public rIDriver_ {
	private:
		java::io::rInputStream Stream_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override;
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return Owner;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIDriver_::reset( P );
			tol::reset( P, Stream_ );
		}
		qCVDTOR( rInputStreamIDriver );
		void Init( cObject *Object )
		{
			rIDriver_::Init( fdr::ts_Default );
			Stream_.Init( Object );
		}
	};

	typedef scln4a::sCaller sCaller_;

	class sCaller
	: public sCaller_ {
	private:
		bso::sU8 Index_;
	public:
		void reset( bso::sBool P = true )
		{
			sCaller_::reset( P );
			Index_ = 0;
		}
		qCDTOR( sCaller );
		void Init( n4all::cCaller &Caller )
		{
			sCaller_::Init( Caller );

			Index_ = 0;
		}
		sJObject Get( void )
		{
			sJObject Object;

			C_().GetArgument( Index_++, n4jre::t_Undefined, &Object );

			return Object;
		}
		void Get( str::dString &String )
		{
		qRH
			scljre::java::lang::rString JString;
			rJString Charset;
			rJByteArray Result;
		qRB
			Charset.Init( sizeof( "UTF-8" ), "UTF-8", n4jre::hOriginal );
			JString.Init( Get() );

			Result.Init();
			JString.GetBytes( Charset, Result );

			String.Append( (bso::char__ *)Result.Core(), Result.Size() );
		qRR
		qRT
		qRE
		}
	};


	typedef sJObject ( fFunction )( sCaller &Caller );

	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	void SCLJREInfo( txf::sOFlow &Flow );	// To define by user.
	void SCLJRERegister( sRegistrar &Registrar );	// To define by user
}

# define SCLJRE_F( name ) scljre::sJObject name( scljre::sCaller &Caller )

# ifdef SCLJRE_H_
#  define H SCLJRE_H_
# endif

#endif
