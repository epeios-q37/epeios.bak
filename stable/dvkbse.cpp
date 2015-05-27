/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define DVKBSE__COMPILATION

#include "dvkbse.h"

using namespace dvkbse;

#define PACKAGE_NAME_MAX_LENGTH	127

static inline const str::string_ &GetRClassName_(
	const char *PackageName,
	JNIEnv *Env,
	str::string_ &ClassName )
{
	ClassName.Append( PackageName );
	ClassName.Append( "/R" );

	return ClassName;
}

static inline const str::string_ &GetRidClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	str::string_ &ClassName )
{
	GetRClassName_( PackageName, Env, ClassName );
	ClassName.Append( "$id" );

	return ClassName;
}

static inline const char *GetRidClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string ClassName;
ERRBegin
	ClassName.Init();

	GetRidClassName_( PackageName, Env, ClassName );

	ClassName.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

jclass dvkbse::GetRidClass(
	const char *PackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	Class = Env->FindClass( GetRidClassName_( PackageName, Env, Buffer ) );

	if ( Class == NULL )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return Class;
}

jclass dvkbse::GetRidClass(
	const jstring JPackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ PackageName;
ERRBegin
	PackageName.Init();
	jvabse::Convert( JPackageName, Env, PackageName );

	Class = GetRidClass( PackageName, Env );
ERRErr
ERREnd
ERREpilog
	return Class;
}

static inline const str::string_ &GetRlayoutClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	str::string_ &ClassName )
{
	GetRClassName_( PackageName, Env, ClassName );
	ClassName.Append( "$layout" );

	return ClassName;
}

static inline const char *GetRlayoutClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string ClassName;
ERRBegin
	ClassName.Init();

	GetRlayoutClassName_( PackageName, Env, ClassName );

	ClassName.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

jclass dvkbse::GetRlayoutClass(
	const char *PackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	Class = Env->FindClass( GetRlayoutClassName_( PackageName, Env, Buffer ) );

	if ( Class == NULL )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return Class;
}

jclass dvkbse::GetRlayoutClass(
	jstring JPackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ PackageName;
ERRBegin
	PackageName.Init();
	jvabse::Convert( JPackageName, Env, PackageName );

	Class = GetRlayoutClass( PackageName, Env );
ERRErr
ERREnd
ERREpilog
	return Class;
}

static inline const str::string_ &GetRrawClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	str::string_ &ClassName )
{
	GetRClassName_( PackageName, Env, ClassName );

	ClassName.Append( "$raw" );

	return ClassName;
}

static inline const char *GetRrawClassName_( 
	const char *PackageName,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string ClassName;
ERRBegin
	ClassName.Init();

	GetRrawClassName_( PackageName, Env, ClassName );

	ClassName.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

jclass dvkbse::GetRrawClass(
	const char *PackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	Class = Env->FindClass( GetRrawClassName_( PackageName, Env, Buffer ) );

	if ( Class == NULL )
		ERRFwk();
ERRErr
ERREnd
ERREpilog
	return Class;
}

jclass dvkbse::GetRrawClass(
	jstring JPackageName,
	JNIEnv *Env )
{
	jclass Class = NULL;
ERRProlog
	STR_BUFFER___ PackageName;
ERRBegin
	PackageName.Init();
	jvabse::Convert( JPackageName, Env, PackageName );

	Class = GetRrawClass( PackageName, Env );
ERRErr
ERREnd
ERREpilog
	return Class;
}

jobject dvkbse::GetRawResourceInputStream(
	const char *Name,
	jclass Raw,
	JNIEnv *Env,
	jobject Activity )
{
	jobject Resources = Env->CallObjectMethod( Activity, jvabse::GetMethodID( Env, Activity, "getResources", "()Landroid/content/res/Resources;" ) );

	return Env->CallObjectMethod( Resources, jvabse::GetMethodID( Env, Resources, "openRawResource", "(I)Ljava/io/InputStream;" ), jvabse::GetStaticIntField(  Env, Raw, Name ) );
}

jobject dvkbse::GetRawResourceInputStream(
	const char *Name,
	const char *PackageName,
	JNIEnv *Env,
	jobject Activity )
{
	return GetRawResourceInputStream( Name, GetRrawClass( PackageName, Env ), Env, Activity );
}

jobject dvkbse::GetRawResourceInputStream(
	const char *Name,
	jstring JPackageName,
	JNIEnv *Env,
	jobject Activity )
{
	jobject Object = NULL;
ERRProlog
	STR_BUFFER___ PackageName;
ERRBegin
	PackageName.Init();
	jvabse::Convert( JPackageName, Env, PackageName );

	Object = GetRawResourceInputStream( Name, PackageName, Env, Activity );
ERRErr
ERREnd
ERREpilog
	return Object;
}
