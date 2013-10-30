/*
	'jvabse.h' by Claude SIMON (http://zeusw.org/).

	'jvabse' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef JVABSE__INC
# define JVABSE__INC

# define JVABSE_NAME		"JVABSE"

# if defined( E_DEBUG ) && !defined( JVABSE_NODBG )
#  define JVABSE_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// JaVA BaSE

# include "err.h"
# include "flw.h"
# include "cpe.h"
# include "tol.h"
# include "str.h"

# ifndef E_JAVA
#  error "Use of JNI, but without defining the target as a Java component (define 'E_JAVA')."
# endif

# ifndef CPE_LIBRARY
#  error "A Java native component must be a library."
# endif

# include <jni.h>

# define ERRJProlog	ERRProlog
# define ERRJBegin	ERRBegin
# define ERRJErr	ERRErr
# define ERRJEnd	ERREnd
# define ERRJEpilog	ERRCommonEpilog ERRTestEpilog } else jvabse::ERRFinal( Env ); }

# define JVABSE_EXPORT	extern "C" JNIEXPORT

namespace jvabse {

	inline void ERRFinal( JNIEnv *Env )
	{
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message );
	}

	const str::string_ &Convert(
		jstring JString,
		JNIEnv *Env,
		str::string_ &String );

	const char *Convert(
		jstring JString,
		JNIEnv *Env,
		STR_BUFFER___ &Buffer );

	struct jni_param__ {
		JNIEnv *Env;
		jobject Object;
		void reset( bso::bool__ = true )
		{
			Env = NULL;
			Object = NULL;
		}
		E_CDTOR( jni_param__ )
	};

	inline jclass GetClass(
		JNIEnv *Env,
		jobject Object )
	{
		jclass Class = Env->GetObjectClass( Object );

		if ( Class == NULL )
			ERRLbr();

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
			ERRLbr();

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
			ERRLbr();

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
			ERRLbr();

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
			ERRLbr();

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
			ERRLbr();

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
			ERRLbr();

		return Object;
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
			ERRPrm();

		Env->SetObjectField( Object, GetFieldID( Env, Object, Name, Signature ), Value );
	}

}

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
