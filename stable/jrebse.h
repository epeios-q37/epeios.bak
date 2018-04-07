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
		JNIEnv *Env_;
		qPMV( _jobject, O_, Object_ );
		virtual void reset_( bso::sBool P = true )
		{}
		virtual jobject Init_( jobject Object )
		{
			return Object;
		}
	public:
		void reset( bso::sBool P = true )
		{
			reset_( P );

			Env_ = NULL;
			Object_ = NULL;
		}
		qCDTOR( sObject_ );
		void Init(
			JNIEnv *Env,
			jobject Object,
			jclass Class )
		{
			Env_ = Env;

			if ( !Env_->IsInstanceOf( Object, Class ) )
				qRFwk();

			Object_ = Init_( Object );
		}
		void Init(
			JNIEnv *Env,
			jobject Object,
			const char *Class )
		{
			return Init( Env, Object, jniq::FindClass( Env, Class ) );
		}
		operator jobject( void )
		{
			return O_();
		}
		template <typename ...args> void Init(
			JNIEnv *Env,
			jclass Class,
			const char *Signature,
			args... Args )
		{
			return Init( Env, Env->NewObject( Class, jniq::GetMethodID( Env, Class, "<init>", Signature ), Args... ), Class );
		}
		template <typename ...args> void Init(
			JNIEnv *Env,
			const char *ClassName,
			const char *Signature,
			args... Args )
		{
			return Init( Env, jniq::FindClass( Env, ClassName ), Signature, Env, Args... );
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
			args... Args ) const\
		{\
			return Env_->Call##name##Method( Env_, O_(), jniq::GetMethodID( Env_, O_(), Method, Signature ), Args... );\
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
				Env_->DeleteGlobalRef( Object_ );
			}
		}
		virtual jobject Init_( jobject Object ) override
		{
			return Env_->NewGlobalRef( Object );
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
					JNIEnv *Env,
					jobject Object ) const
				{
					return object::CallVoidMethod( Env, "println", "(Ljava/lang/Object;)V", Object );
				}
				void Println(
					JNIEnv *Env,
					const char *Text ) const
				{
					return Println( Env, Env->NewStringUTF( Text ) );
				}
				void Flush( JNIEnv *Env ) const
				{
					return object::CallVoidMethod( Env, "flush", "()V" );
				}
			CF( PrintStream );
			CH( InputStream )
				jint Read( JNIEnv *Env ) const
				{
					return object::CallIntMethod( Env, "read", "()I" );
				}
				jint Read(
					JNIEnv *Env,
					jbyteArray b,
					jint off,
					jint len ) const
				{
					return object::CallIntMethod( Env, "read", "([BII)I", Env, b, off, len );
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
				static io::sPrintStream Out( JNIEnv *Env )
				{
					return io::sPrintStream( Env, jniq::GetStaticObjectField( Env, Name, "out", "Ljava/io/PrintStream;" ) );
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
