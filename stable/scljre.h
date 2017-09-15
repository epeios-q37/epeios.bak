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

namespace scljre {
	using scln4a::sCaller;
	using namespace n4jre;

	class sObject {
	private:
		qRMV( cObject, O_, Object_ );
		void Set_(
			sValue_ &Value,
			jbyteArray Content )
		{
			Value.Init( tByteArray, Content );
		}
		void Set_(
			sValue_ &Value,
			jstring Content )
		{
			Value.Init( tString, Content );
		}
		void Set_(
			sValue_ &Value,
			jint Content )
		{
			Value.Init( tByteArray, &Content );
		}
		// Termination version of below method.
		void Fill_(
			int Indice,
			sValue_ *Values )
		{}
		template <typename arg, typename ...args> void Fill_(
			int Indice,
			sValue_ *Values,
			const arg &Arg,
			const args&... Args )
		{
			Set_( Values[Indice], Arg );
			Fill_( Indice + 1, Values, Args... );
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Object_ != NULL )
					delete Object_;
			}

			Object_ = NULL;
		}
		qCDTOR( sObject );
		void Init(
			const char *Name,
			n4jre::cObject *Object )
		{
			reset();

			Object_ = Object;
		}
		void Set(
			const char *Name,
			const char *Signature,
			jobject Value )
		{
			return O_().Set( Name, Signature, Value );
		}
		template <typename object> void Set(
			const char *Name,
			object &Object )
		{
			Set( Name, NULL, Object );
		}
		template <typename ...args> jint CallIntMethod(
			const char *Method,
			const char *Signature,
			const args&... Args )
		{
			sValue_ Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.

			Fill_( 0, Values, Args... );

			return O_().CallIntMethod( Method, Signature, Values );
		}
		template <typename ...args> jlong CallLongMethod(
			const char *Method,
			const char *Signature,
			const args&... Args )
		{
			sValue_ Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.

			Fill_( 0, Values, Args... );

			return O_().CallLongMethod( Method, Signature, Values );
		}
		template <typename ...args> jobject CallObjectMethod(
			const char *Method,
			const char *Signature,
			const args&... Args )
		{
			sValue_ Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.

			Fill_( 0, Values, Args... );

			return O_().CallObjectMethod( Method, Signature, Values );
		}
	};


	/*
	class sObject_ {
	private:
	qRMV( cObject, C_, Callback_ );
	# define H( type, name )\
	type Call##name##Method_(\
	const char *Method,\
	const char *Signature,\
	const sValue_ *Values )\
	{\
	return C_().Call##name##Method( Method, Signature, Values );\
	}
	H( void, Void );
	H( int, Int );
	H( long, Long );
	H( object, Object );
	# undef H
	public:
	void reset( bso::sBool = true )
	{
	Callback_ = NULL;
	}
	qCDTOR( sObjec_t );
	void Init( void )
	{
	Callback_ = NULL;
	}
	};
	*/

	namespace java {
		namespace io {
			class sInputStream {
			private:
				sObject Object_;
			public:
				void reset( bso::sBool P = true )
				{
					Object_.reset( P );
				}
				qCDTOR( sInputStream );
				void Init( n4jre::cObject *Object )
				{
					Object_.Init( NULL, Object );
				}
				jint Read( void )
				{
					return Object_.CallIntMethod( "read", "()I" );
				}
				jint Read(
					jbyteArray b,
					jint off,
					jint len )
				{
					return Object_.CallIntMethod( "read", "([BII)I", b, off, len );
				}
			};
		}
		namespace lang {
			class sInteger {
			private:
				sObject Object_;
			public:
				void reset( bso::sBool P = true )
				{
					Object_.reset( P );
				}
				qCDTOR( sInteger );
				void Init( n4jre::cObject *Object )
				{
					Object_.Init( NULL, Object );
				}
				jint IntValue( void )
				{
					return Object_.CallIntMethod( "intValue", "()I" );
				}
			};

			class sLong {
			private:
				sObject Object_;
			public:
				void reset( bso::sBool P = true )
				{
					Object_.reset( P );
				}
				qCDTOR( sLong );
				void Init( n4jre::cObject *Object )
				{
					Object_.Init( NULL, Object );
				}
				jlong LongValue( void )
				{
					return Object_.CallLongMethod( "longValue", "()J" );
				}
			};

			class sString {
			private:
				sObject Object_;
			public:
				void reset( bso::sBool P = true )
				{
					Object_.reset( P );
				}
				qCDTOR( sString );
				void Init( n4jre::cObject *Object )
				{
					Object_.Init( NULL, Object );
				}
				jlong LongValue( void )
				{
					return Object_.CallLongMethod( "longValue", "()J" );
				}
				jstring Concat( jstring String )
				{
					return (jstring)Object_.CallObjectMethod( "concat", "(Ljava/lang/String;)Ljava/lang/String;", String );
				}
			};
		}
	}

	typedef java::lang::sString sString_;

	class sString
	: public sString_ {
	public:
		void reset( bso::sBool P = true )
		{
			sString_::reset( P );
		}
		qCDTOR( sString );
		sString( const char *Text )
		{
			Init( Text );
		}
		sString( const str::dString &Text )
		{
			Init( Text );
		}
		sString( jobject *Object )
		{
			Init( Object );
		}
		void Init( const char *Text )
		{
			qRVct();
		}
		void Init( const str::dString &Text )
		{
		qRH
			qCBUFFERr Buffer;
		qRB
			Init( Text.Convert( Buffer ) );
		qRR
		qRT
		qRE
		}
		void Init( jobject *Object )
		{
			qRVct();
//			sString_::Init( Object );
		}
		operator jstring( void ) const
		{
			qRVct();
//			return (jstring)Object();
			return NULL;
		}
		jstring Concat( const char *Text )
		{
			qRVct();
			// return sString_::Concat( Env->NewStringUTF( Text ), Env );

			return NULL;
		}
	};

	typedef fdr::rIDressedDriver rIDriver_;

	class rInputStreamIDriver
	: public rIDriver_ {
	private:
		java::io::sInputStream Stream_;
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

	void Throw( const str::dString &Text );

	jobject Object( const char *Name );
	jobject Integer( jint Integer );
	jobject Long( jlong Long );
	jobject Null( void );

	using scln4a::sRegistrar;
	void SCLJREInfo( txf::sOFlow &Flow );	// To define by user.
	void SCLJRERegister( sRegistrar &Registrar );	// To define by user
}

#endif
