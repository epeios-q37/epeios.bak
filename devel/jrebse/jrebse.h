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

# ifdef H
#  #define JNIBSE_H_ H
#  undef H
# endif


namespace jrebse {
	class sObject_
	{
	protected:
		qPMV( _jobject, O_, Object_ );
		virtual void reset_( bso::sBool P = true )
		{}
		virtual jobject Init_(
			jobject Object,
			JNIEnv *Env )
		{
			return Object;
		}
	public:
		void reset( bso::sBool P = true )
		{
			reset_( P );

			Object_ = NULL;
		}
		qCDTOR( sObject_ );
		void Init(
			jobject Object,
			jclass Class,
			JNIEnv *Env = NULL )
		{
			Env = jniq::GetEnv( Env );

			if ( !Env->IsInstanceOf( Object, Class ) )
				qRFwk();

			Object_ = Init_( Object, Env );
		}
		void Init(
			jobject Object,
			const char *Class,
			JNIEnv *Env = NULL )
		{
			Env = jniq::GetEnv( Env );

			return Init( Object, jniq::FindClass( Class, Env ), Env );
		}
		operator jobject( void )
		{
			return O_();
		}
		template <typename ...args> void Init(
			jclass Class,
			const char *Signature,
			JNIEnv *Env,
			args... Args )
		{
			Env = jniq::GetEnv( Env );

			return Init( Env->NewObject( Class, jniq::GetMethodID( Class, "<init>", Signature, Env ), Args... ), Class, Env );
		}
		template <typename ...args> void Init(
			jclass Class,
			const char *Signature,
			args... Args )
		{
			return Init( Class, Signature, NULL, Args... );
		}
		template <typename ...args> void Init(
			const char *ClassName,
			const char *Signature,
			JNIEnv *Env,
			args... Args )
		{
			Env = jniq::GetEnv( Env );

			return Init( jniq::FindClass( ClassName, Env ), Signature, Env, Args... );
		}
		template <typename ...args> void Init(
			const char *ClassName,
			const char *Signature,
			args... Args )
		{
			return Init(  ClassName, Signature, (JNIEnv *)NULL, Args... );
		}
		jobject Object( void ) const
		{
			return O_();
		}
# define H( type, name )\
		template <typename ...args> type Call##name##Method(\
			const char *Method,\
			const char *Signature,\
			JNIEnv *Env,\
			args... Args ) const\
		{\
			Env = jniq::GetEnv( Env );\
\
			return Env->Call##name##Method( O_(), jniq::GetMethodID( O_(), Method, Signature, Env ), Args... );\
		}
		H( void, Void );
		H( jint, Int );
		H( jlong, Long );
		H( jobject, Object );
# undef H
	};

	class rObject_
	: public sObject_
	{
	protected:
		virtual void reset_( bso::sBool P ) override
		{
			if ( P ) { 
				jniq::GetEnv( NULL )->DeleteGlobalRef( Object_ );
			}
		}
		virtual jobject Init_(
			jobject Object,
			JNIEnv_ *Env ) override
		{
			return jniq::GetEnv( Env )->NewGlobalRef( Object );
		}
	};

# ifdef CH
#  define JREBSE_CH_	CH
#  undef CH
#endif

# ifdef CF
#  define JREBSE_CF_	CF
#  undef CF
#endif


# define CH( name )\
	class name\
	{\
	public:\
		static const char *Name;\
		static const char *Signature;\
	};\
	\
	template <typename object> class name##_\
	: public object,\
	  public name\
	{\
	public:\
		void reset( bso::sBool P = true )\
		{\
			object::reset( P );\
		}\
		qCDTOR( name##_ );\
		name##_(\
			jobject Object,\
			JNIEnv *Env = NULL )\
		{\
			Init( Object );\
		}\
		void Init(\
			jobject Object,\
			JNIEnv *Env = NULL )\
		{\
			return object::Init( Object, Signature, Env );\
		}

# define CF( name )\
	};\
	typedef name##_<jrebse::sObject_> s##name;\
	typedef name##_<jrebse::rObject_> r##name;

	namespace java {
		namespace io {
			CH( PrintStream )
				void Println(
					jobject Object,
					JNIEnv *Env = NULL ) const
				{
					return object::CallVoidMethod( "println", "(Ljava/lang/Object;)V", Env, Object );
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
					return object::CallVoidMethod( "flush", "()V", Env );
				}
			CF( PrintStream );
			CH( InputStream )
				jint Read( JNIEnv *Env = NULL ) const
				{
					return object::CallIntMethod( "read", "()I", Env );
				}
				jint Read(
					jbyteArray b,
					jint off,
					jint len,
					JNIEnv *Env = NULL ) const
				{
					return object::CallIntMethod( "read", "([BII)I", Env, b, off, len, Env );
				}
			CF( InputStream );
		}

		namespace lang {
			CH( Integer )
				Integer_(
					jint Value,
					JNIEnv *Env = NULL )
				{
					Init( Value, Env );
				}
				void Init(
					jint Value,
					JNIEnv *Env = NULL )
				{
					return object::Init( Name, "(I)V", Value, Env );
				}
				jint IntValue( JNIEnv *Env = NULL ) const
				{
					return object::CallIntMethod( "intValue", "()I", Env);
				}
			CF( Integer );
			CH( Long )
				Long_(
					jlong Value,
					JNIEnv *Env = NULL )
				{
					Init( Value, Env );
				}
				void Init(
					jlong Value,
					JNIEnv *Env = NULL )
				{
					return object::Init( Name, "(J)V", Value, Env );
				}
				jlong LongValue( JNIEnv *Env = NULL ) const
				{
					return object::CallLongMethod( "longValue", "()J", Env );
				}
			CF( Long );
			CH( Object )
			CF( Object );
			CH( String )
				jstring Concat(
					jstring String,
					JNIEnv *Env = NULL )
				{
					return (jstring)object::CallObjectMethod( "concat", "(Ljava/lang/String;)Ljava/lang/String;", Env, String );
				}
			CF( String );
			CH( System )
				static io::sPrintStream Out( JNIEnv *Env = NULL )
				{
					Env = jniq::GetEnv( Env );

					return io::sPrintStream( jniq::GetStaticObjectField( Name, "out", "Ljava/io/PrintStream;", Env ), Env );
				}
			CF( System );
		}
	}

# ifdef JREBSE_CH_
#  define CH JREBSE_CH_
#endif

# ifdef JREBSE_CF_
#  define CF JREBSE_CF_
#endif

# ifdef JNINSE_H_
#  define H JNIBSE_H_
# endif



}

#endif
