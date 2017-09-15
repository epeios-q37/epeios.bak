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

// Java Runtime Environment

#ifndef JRE_INC_
# define JRE_INC_

# define JRE_NAME		"JRE"

# if defined( E_DEBUG ) && !defined( JRE_NODBG )
#  define JRE_DBG
# endif

# include "jrebse.h"

# include "err.h"
# include "fdr.h"

namespace jre {
	using namespace jrebse;

	typedef  java::lang::sObject sObject_;

	class sObject
	: public sObject_
	{
	public:
		void Set(
			const char *Name,
			const char *Signature,
			jobject Value,
			JNIEnv *Env = NULL )
		{
			jniq::SetObjectField( sObject_::Object(), Name, Signature, Value, Env );
		}
		jobject Get(
			const char *Name,
			const char *Signature,
			JNIEnv *Env = NULL )
		{
			return jniq::GetObjectField( sObject_::Object(), Name, Signature, Env );
		}
		void Init(
			jobject Object,
			const char *Class,
			JNIEnv *Env = NULL )
		{
			return sObject_::Init( Object, Signature, Env );
		}
	};

	typedef java::lang::sString sString_;

	class sString
	: public sString_
	{
	public:
		void reset( bso::sBool P = true )
		{
			sString_::reset( P );
		}
		qCDTOR( sString );
		sString(
			const char *Text,
			JNIEnv *Env = NULL )
		{
			Init( Text, Env );
		}
		sString(
			const str::dString &Text,
			JNIEnv *Env = NULL )
		{
			Init( Text, Env );
		}
		sString(
			jobject Object,
			JNIEnv *Env = NULL )
		{
			Init( Object, Env );
		}
		void Init(
			const char *Text,
			JNIEnv *Env = NULL )
		{
			Env = jniq::GetEnv( Env );

			sString_::Init( Env->NewStringUTF( Text ), Env );
		}
		void Init(
			const str::dString &Text,
			JNIEnv *Env = NULL )
		{
		qRH
			qCBUFFERr Buffer;
		qRB
			Init( Text.Convert( Buffer ), Env );
		qRR
		qRT
		qRE
		}
		void Init(
			jobject Object,
			JNIEnv *Env = NULL )
		{
			sString_::Init( Object, Env );
		}
		operator jstring( void ) const
		{
			return (jstring)Object();
		}
		jstring Concat(
			const char *Text,
			JNIEnv *Env = NULL )
		{
			Env = jniq::GetEnv( Env );

			return sString_::Concat( Env->NewStringUTF( Text ), Env );
		}
	};

	typedef fdr::rIDressedDriver rIDriver_;

	class rInputStreamIDriver
	: public rIDriver_
	{
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
		void Init(
			jobject Stream,
			JNIEnv *Env = NULL )
		{
			rIDriver_::Init( fdr::ts_Default );
			Stream_.Init( Stream, Env );
		}
	};
}

#endif
