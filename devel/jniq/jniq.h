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

// From Java Native Interface

#ifndef JNIQ_INC_
# define JNIQ_INC_

# define JNIQ_NAME		"JNIQ"

# if defined( E_DEBUG ) && !defined( JNIQ_NODBG )
#  define JNIQ_DBG
# endif

// Note to developer : include 'h:\hg\JREIncludeDirectories.props' in the '.vcxproj'.
# include <jni.h>

# include "err.h"
# include "str.h"
# include "tol.h"

# ifdef H
#  #define JNIQ_H_ H
#  undef H
# endif


namespace jniq {
	const str::string_ &Convert(
		jstring JString,
		JNIEnv *Env,
		str::string_ &String );

	const char *Convert(
		jstring JString,
		JNIEnv *Env,
		qCBUFFERr &Buffer );

	inline jclass FindClass(
		JNIEnv *Env,
		const char *Name )
	{
		jclass Class = Env->FindClass( Name );

		if ( Class == NULL )
			qRFwk();

		return Class;
	}

	inline jclass GetClass(
		JNIEnv *Env,
		jobject Object )
	{
		jclass Class = Env->GetObjectClass( Object );

		if ( Class == NULL )
			qRLbr();

		return Class;
	}

	inline jmethodID GetMethodID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jmethodID MethodID = Env->GetMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			qRLbr();

		return MethodID;
	}

	inline jmethodID GetMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetMethodID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jmethodID GetStaticMethodID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jmethodID MethodID = Env->GetStaticMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			qRLbr();

		return MethodID;
	}

	inline jmethodID GetStaticMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetStaticMethodID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jfieldID GetFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jfieldID FieldID = Env->GetFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			qRLbr();

		return FieldID;
	}

	inline jfieldID GetFieldID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetFieldID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jfieldID GetStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jfieldID FieldID = Env->GetStaticFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			qRLbr();

		return FieldID;
	}

	inline jint GetIntField(
		JNIEnv *Env,
		jobject Object,
		const char *Name )
	{
		return Env->GetIntField( Object, GetFieldID( Env, Object, Name, "I" ) );
	}

	inline jint GetStaticIntField(
		JNIEnv *Env,
		jclass Class,
		const char *Name )
	{
		return Env->GetStaticIntField( Class, GetStaticFieldID( Env, Class, Name, "I" ) );
	}

	inline jlong GetLongField(
		JNIEnv *Env,
		jobject Object,
		const char *Name )
	{
		return Env->GetLongField( Object, GetFieldID( Env, Object, Name, "J" ) );
	}

	inline jlong GetStaticLongField(
		JNIEnv *Env,
		jclass Class,
		const char *Name )
	{
		return Env->GetStaticLongField( Class, GetStaticFieldID( Env, Class, Name, "J" ) );
	}

	inline jobject GetObjectField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		jobject Field = Env->GetObjectField( Object, GetFieldID( Env, Object, Name, Signature ) );

		if ( Field == NULL )
			qRLbr();

		return Field;
	}

	inline jobject GetStaticObjectField(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jobject Object = Env->GetStaticObjectField( Class, GetStaticFieldID( Env, Class, Name, Signature ) );;

		if ( Object == NULL )
			qRLbr();

		return Object;
	}

	inline jobject GetStaticObjectField(
		JNIEnv *Env,
		const char *ClassName,
		const char *Name,
		const char *Signature )
	{
		return GetStaticObjectField( Env, FindClass( Env, ClassName ), Name, Signature );
	}

	inline void SetIntField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		jint Value )
	{
		Env->SetIntField( Object, GetFieldID( Env, Object, Name, "J" ), Value );
	}

	inline void SetLongField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		jlong Value )
	{
		Env->SetLongField( Object, GetFieldID( Env, Object, Name, "J" ), Value );
	}

	inline void SetObjectField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature,
		jobject Value )
	{
		if ( ( Signature == NULL )
			  || ( Signature[0] == 0 )
			  ||( Signature[strlen( Signature )-1] != ';' ) )
			qRFwk();

		Env->SetObjectField( Object, GetFieldID( Env, Object, Name, Signature ), Value );
	}

	class sObject
	{
	private:
		qPMV( _jobject, O_, Object_ );
		JNIEnv *Env_;	// Jusrt for the destructor.
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) { 
				if ( Object_ != NULL )
					if ( Env_ != NULL  )
						Env_->DeleteGlobalRef( Object_ );
			}
				
			Object_ = NULL;
			Env_ = NULL;
		}
		qCDTOR( sObject );
		void Init(
			JNIEnv *Env,
			jobject Object,
			jclass Class,
			bso::sBool Take )
		{
			if ( !Env->IsInstanceOf( Object, Class ) )
				qRFwk();

			if ( Take )
				Object_ = Object;
			else {
				Object_ = Env->NewGlobalRef( Object );
				Env_ = Env;
			}
		}
		void Init(
			JNIEnv *Env,
			jobject Object,
			const char *Class,
			bso::sBool Take )
		{
			return Init( Env, Object, FindClass( Env, Class ), Take );
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
			Init( Env, Env->NewObject( Class, GetMethodID( Env, Class, "<init>", Signature ), Args... ), Class, true );
		}
		template <typename ...args> void Init(
			JNIEnv *Env,
			const char *ClassName,
			const char *Signature,
			args... Args )
		{
			return Init(Env, FindClass( Env, ClassName ), Signature, Args... );
		}
# define H( type, name )\
		template <typename ...args> type Call##name##Method(\
			JNIEnv *Env,\
			const char *Method,\
			const char *Signature,\
			args... Args ) const\
		{\
			return Env->Call##name##Method( O_(), GetMethodID( Env, O_(), Method, Signature ), Args... );\
		}
		H( void, Void );
		H( jint, Int );
		H( jlong, Long );
# undef H
	};
}

# ifdef JNIQ_H_
#  define H JNIQ_H_
# endif

#endif
