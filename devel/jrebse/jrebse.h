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

// Java Runtime Environment BASe

#ifndef JREBSE_INC_
# define JREBSE_INC_

# define JREBSE_NAME		"JREBSE"

# if defined( E_DEBUG ) && !defined( JREBSE_NODBG )
#  define JREBSE_DBG
# endif

# include "err.h"
# include "jniq.h"

namespace jrebse {
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
			jobject Object,\
			JNIEnv *Env = NULL )\
		{\
			Init( Object );\
		}\
		void Init(\
			jobject Object,\
			JNIEnv *Env = NULL )\
		{\
			return sObject_::Init( Object, Name_, Env );\
		}

# define CF	}

	namespace java {
		namespace io {
			CH( PrintStream )
				void Println(
					jobject Object,
					JNIEnv *Env = NULL ) const
				{
					return CallVoidMethod( "println", "(Ljava/lang/Object;)V", Object, Env );
				}
				void Println(
					const char *Text,
					JNIEnv *Env = NULL ) const
				{
					Env = jniq::GetEnv( Env );

					return Println( Env->NewStringUTF( Text ), Env );
				}
				void Flush( JNIEnv *Env = NULL ) const
				{
					return CallVoidMethod( "flush", "()V", Env );
				}
			CF;
			CH( InputStream )
				jint Read( JNIEnv *Env = NULL ) const
				{
					return CallIntMethod( "read", "()I", Env );
				}
				jint Read(
					jbyteArray b,
					jint off,
					jint len,
					JNIEnv *Env = NULL ) const
				{
					return CallIntMethod( "read", "([BII)I", b, off, len, Env );
				}
			CF;
		}

		namespace lang {
			CH( Integer )
				sInteger(
					jint Value,
					JNIEnv *Env = NULL )
				{
					Init( Value, Env );
				}
				void Init(
					jint Value,
					JNIEnv *Env = NULL )
				{
					return sObject::Init( Name_, "(I)V", Value, Env );
				}
				jint IntValue( JNIEnv *Env = NULL ) const
				{
					return CallIntMethod( "intValue", "()I", Env);
				}
			CF;
			CH( Long )
				sLong(
					jlong Value,
					JNIEnv *Env = NULL )
				{
					Init( Value, Env );
				}
				void Init(
					jlong Value,
					JNIEnv *Env = NULL )
				{
					return sObject::Init( Name_, "(J)V", Value, Env );
				}
				jlong LongValue( JNIEnv *Env = NULL ) const
				{
					return CallLongMethod( "longValue", "()J", Env );
				}
			CF;
			CH( String )
			CF;
			CH( System )
				static io::sPrintStream Out( JNIEnv *Env = NULL )
				{
					Env = jniq::GetEnv( Env );

					return io::sPrintStream( jniq::GetStaticObjectField( Name_, "out", "Ljava/io/PrintStream;", Env ), Env );
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
