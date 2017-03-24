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

namespace jniq {
	void SetGlobalEnv( JNIEnv *Env );

	JNIEnv *GetGlobalEnv( void );

	inline JNIEnv *GetEnv( JNIEnv *Env = NULL )
	{
		if ( Env == NULL )
			return GetGlobalEnv();
		else
			return Env;
	}

	const str::string_ &Convert(
		jstring JString,
		str::string_ &String,
		JNIEnv *Env = NULL );

	const char *Convert(
		jstring JString,
		qCBUFFERr &Buffer,
		JNIEnv *Env = NULL );

	inline jclass FindClass(
		const char *Name,
		JNIEnv *Env = NULL )
	{
		jclass Class = GetEnv( Env )->FindClass( Name );

		if ( Class == NULL )
			qRFwk();

		return Class;
	}

	inline jclass GetClass(
		jobject Object,
		JNIEnv *Env = NULL )
	{
		jclass Class = GetEnv( Env )->GetObjectClass( Object );

		if ( Class == NULL )
			qRLbr();

		return Class;
	}

	inline jmethodID GetMethodID(
		jclass Class,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		jmethodID MethodID = GetEnv( Env )->GetMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			qRLbr();

		return MethodID;
	}

	inline jmethodID GetMethodID(
		jobject Object,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return GetMethodID( GetClass( Object, Env ), Name, Signature, Env );
	}

	inline jmethodID GetStaticMethodID(
		jclass Class,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		jmethodID MethodID = GetEnv( Env )->GetStaticMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			qRLbr();

		return MethodID;
	}

	inline jmethodID GetStaticMethodID(
		jobject Object,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return GetStaticMethodID( GetClass( Object, Env ), Name, Signature, Env );
	}

	inline jfieldID GetFieldID(
		jclass Class,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		jfieldID FieldID = GetEnv( Env )->GetFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			qRLbr();

		return FieldID;
	}

	inline jfieldID GetFieldID(
		jobject Object,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );
		return GetFieldID( GetClass( Object, Env ), Name, Signature, Env );
	}

	inline jfieldID GetStaticFieldID(
		jclass Class,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		jfieldID FieldID = GetEnv( Env )->GetStaticFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			qRLbr();

		return FieldID;
	}

	inline jint GetIntField(
		jobject Object,
		const char *Name,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return Env->GetIntField( Object, GetFieldID( Object, Name, "I", Env ) );
	}

	inline jint GetStaticIntField(
		jclass Class,
		const char *Name,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return Env->GetStaticIntField( Class, GetStaticFieldID( Class, Name, "I", Env ) );
	}

	inline jlong GetLongField(
		jobject Object,
		const char *Name,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return Env->GetLongField( Object, GetFieldID( Object, Name, "J", Env ) );
	}

	inline jlong GetStaticLongField(
		jclass Class,
		const char *Name,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return Env->GetStaticLongField( Class, GetStaticFieldID( Class, Name, "J", Env ) );
	}

	inline jobject GetObjectField(
		jobject Object,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		jobject Field = Env->GetObjectField( Object, GetFieldID( Object, Name, Signature, Env ) );

		if ( Field == NULL )
			qRLbr();

		return Field;
	}

	inline jobject GetStaticObjectField(
		jclass Class,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env);

		jobject Object = Env->GetStaticObjectField( Class, GetStaticFieldID( Class, Name, Signature, Env ) );

		if ( Object == NULL )
			qRLbr();

		return Object;
	}

	inline jobject GetStaticObjectField(
		const char *ClassName,
		const char *Name,
		const char *Signature,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		return GetStaticObjectField( FindClass( ClassName, Env ), Name, Signature, Env );
	}

	inline void SetIntField(
		jobject Object,
		const char *Name,
		jint Value,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		Env->SetIntField( Object, GetFieldID( Object, Name, "J", Env ), Value );
	}

	inline void SetLongField(
		jobject Object,
		const char *Name,
		jlong Value,
		JNIEnv *Env = NULL )
	{
		Env = GetEnv( Env );

		Env->SetLongField( Object, GetFieldID( Object, Name, "J", Env ), Value );
	}

	inline void SetObjectField(
		jobject Object,
		const char *Name,
		const char *Signature,
		jobject Value,
		JNIEnv *Env = NULL )
	{
		if ( ( Signature == NULL )
			  || ( Signature[0] == 0 )
			  || ( Signature[strlen( Signature )-1] != ';' ) )
			qRFwk();

		Env = GetEnv( Env );

		Env->SetObjectField( Object, GetFieldID( Object, Name, Signature ), Value );
	}
}

#endif
