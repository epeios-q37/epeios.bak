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

// JNI OBJects

#ifndef JNIOBJ_INC_
# define JNIOBJ_INC_

# define JNIOBJ_NAME		"JNIOBJ"

# if defined( E_DEBUG ) && !defined( JNIOBJ_NODBG )
#  define JNIOBJ_DBG
# endif

# include "jniq.h"

# include "err.h"
# include "tol.h"

namespace jniobj {
	typedef jniq::sObject sObject_;

# ifdef CH
#  define JNIOBJ_CH_	CH
#  undef CH
#endif

# ifdef CF
#  define JNIOBJ_CF_	CF
#  undef CF
#endif


# define CH( name )\
	class s##name\
	: public sObject_\
	{\
	private:\
		static const char *Name_;\
	public:\
		void reset( bso::sBool P = true )\
		{\
			sObject_::reset( P );\
		}\
		qCDTOR( s##name );\
		s##name(\
			JNIEnv *Env,\
			jobject Object )\
		{\
			Init( Env, Object );\
		}\
		void Init(\
			JNIEnv *Env,\
			jobject Object )\
		{\
			return sObject_::Init( Env, Object, Name_, false );\
		}

# define CF	}

	namespace java {
		namespace io {
			CH( PrintStream )
				void Println(
					JNIEnv *Env,
					jobject Object ) const
				{
					return CallVoidMethod( Env, "println", "(Ljava/lang/Object;)V", Object );
				}
				void Println(
					JNIEnv *Env,
					const char *Text ) const
				{
					return Println(Env, Env->NewStringUTF( Text ) );
				}
				void Flush( JNIEnv *Env ) const
				{
					return CallVoidMethod( Env, "flush", "()V" );
				}
			CF;
			CH( InputStream )
				jint Read( JNIEnv *Env ) const
				{
					return CallIntMethod( Env, "read", "()I" );
				}
				jint Read(
					JNIEnv *Env,
					jbyteArray b,
					jint off,
					jint len ) const
				{
					return CallIntMethod( Env, "read", "([BII)I", b, off, len );
				}
			CF;
		}

		namespace lang {
			CH( Integer )
				sInteger(
					JNIEnv *Env,
					jint Value )
				{
					Init( Env, Value );
				}
				void Init(
					JNIEnv *Env,
					jint Value )
				{
					return sObject::Init( Env, Name_, "(I)V", Value );
				}
				jint IntValue( JNIEnv *Env ) const
				{
					return CallIntMethod(Env, "intValue", "()I");
				}
			CF;
			CH( Long )
				sLong(
					JNIEnv *Env,
					jlong Value )
				{
					Init( Env, Value );
				}
				void Init(
					JNIEnv *Env,
					jlong Value )
				{
					return sObject::Init( Env, Name_, "(J)V", Value );
				}
				jlong LongValue( JNIEnv *Env ) const
				{
					return CallLongMethod(Env, "longValue", "()J");
				}
			CF;
			CH( System )
				static io::sPrintStream Out( JNIEnv *Env )
				{
					return io::sPrintStream( Env, jniq::GetStaticObjectField( Env, Name_, "out", "Ljava/io/PrintStream;" ) );
				}
			CF;
		}
	}

# ifdef JNIOBJ_CH_
#  define CH JNIOBJ_CH_
#endif

# ifdef JNIOBJ_CF_
#  define CF JNIOBJ_CF_
#endif

}

#endif
