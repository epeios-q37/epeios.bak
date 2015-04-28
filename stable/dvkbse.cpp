/*
	'dvkbse' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dvkbse' header file ('dvkbse.h').
	Copyright (C) 20122004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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
