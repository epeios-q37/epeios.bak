/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

namespace jniq {
	void SetGlobalEnv( JNIEnv *Env );

	JNIEnv *GetGlobalEnv( void );

	// DEPRECATED, as useless in multithreading environment !
	inline JNIEnv *GetEnv_( JNIEnv *Env = NULL )
	{
		if ( Env == NULL )
			return GetGlobalEnv();
		else
			return Env;
	}

	const str::string_ &Convert(
		JNIEnv *Env,
		jstring JString,
		str::string_ &String);

	const char *Convert(
		JNIEnv *Env,
		jstring JString,
		qCBUFFERh &Buffer );

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
		jobject Object = Env->GetStaticObjectField( Class, GetStaticFieldID( Env, Class, Name, Signature ) );

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
			  || ( Signature[strlen( Signature )-1] != ';' ) )
			qRFwk();

		Env->SetObjectField( Object, GetFieldID( Env, Object, Name, Signature ), Value );
	}

	template <typename type> inline jsize GetLength(
		JNIEnv *Env,
		type Array )
	{
		jsize Length = 0;

		Length = Env->GetArrayLength( Array );

		if ( Length < 0 )
			qRFwk();	// Probably 'Array' is NOT an array (bad casting).

		return Length;
	}

	template <typename type> inline jobject GetElement(
		JNIEnv *Env,
		type Array,
		jsize Index )
	{
		return Env->GetObjectArrayElement( Array, Index );
	}

	template <typename type> inline void SetElement(
		JNIEnv *Env,
		type Array,
		jsize Index,
		jobject Object )
	{
		return Env->SetObjectArrayElement( Array, Index, Object );
	}

	// Don't know what happens if 'Array' isn't a 'jobjectArray' !
	inline jsize GetLength(
		JNIEnv *Env,
		jobject Array )
	{
		return GetLength( Env, (jobjectArray)Array );
	}

	inline jobject GetElement(
		JNIEnv *Env,
		jobject Array,
		jsize Index )
	{
		return GetElement( Env, (jobjectArray)Array, Index );
	}

	inline void SetElement(
		JNIEnv *Env,
		jobject Array,
		jsize Index,
		jobject Object )
	{
		return SetElement( Env, (jobjectArray)Array, Index, Object );
	}
}

#endif
