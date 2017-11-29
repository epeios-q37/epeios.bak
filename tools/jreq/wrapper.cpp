/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'JREq'.

    'JREq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'JREq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'JREq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wrapper.h"

using namespace wrapper;

namespace {
	void Fill_(
		const n4jre::rJString &String,
		JNIEnv *Env,
		jvalue &Arg )
	{
		if ( String.Core() != 0 ) {
			Arg.l = Env->NewStringUTF( String.Core() );

			if ( Arg.l == NULL )
				qRAlc();
		}
	}

	void Fill_(
		const n4jre::rJByteArray &ByteArray,
		JNIEnv *Env,
		jvalue &Arg )
	{
		long Size = ByteArray.Size();

		Arg.l = Env->NewByteArray( ByteArray.Size() );

		if ( Arg.l == NULL )
			qRAlc();

		if ( Size > 0 )
			Env->SetByteArrayRegion( (jbyteArray)Arg.l, 0, ByteArray.Size(), ByteArray.Core() );
	}

	void Fill_(
		const n4jre::sValue &ArgV,
		JNIEnv *Env,
		jvalue &Arg )
	{
		switch ( ArgV.Type ) {
		case n4jre::tByte:
			Arg.b = ArgV.Byte;
			break;
		case n4jre::tInt:
			Arg.i = ArgV.Int;
			break;
		case n4jre::tLong:
			Arg.j = ArgV.Long;
			break;
		case n4jre::tString:
			Fill_( *ArgV.String, Env, Arg );
			break;
		case n4jre::tByteArray:
			Fill_( *ArgV.ByteArray, Env, Arg );
			break;
		default:
			qRGnr();
			break;
		}
	}

	void Fill_(
		int ArgC,
		const n4jre::sValue *ArgV,
		JNIEnv *Env,
		jvalue *Arg )
	{
		while ( ArgC-- )
			Fill_( ArgV[ArgC], Env, Arg[ArgC] );
	}
	jvalue *Fill_(
		int ArgC,
		const n4jre::sValue *ArgV,
		JNIEnv *Env )
	{
		jvalue *Args = NULL;
	qRH
		JNIEnv *Env = NULL;
	qRB
		JNIEnv *Env = jniq::GetEnv();

		if ( ArgC != 0 ) {
			Args = new jvalue[ArgC];

			if ( Args == NULL )
				qRAlc();

			Fill_( ArgC, ArgV, Env, Args );
		}
	qRR
	qRT
	qRE
		return Args;
	}

	void Retrieve_(
		jobject Arg,
		JNIEnv *Env,
		n4jre::rJString &String )
	{
		jsize Size = Env->GetStringUTFLength( (jstring)Arg );

		String.Init( Size + 1 );	// '+1' for the terminating NUL.

		if ( Size > 0 )
			Env->GetStringUTFRegion( (jstring)Arg, 0, Size, String.Core() );
	}

	void Retrieve_(
		jobject Arg,
		JNIEnv *Env,
		n4jre::rJByteArray &Array )
	{
		jsize Size = Env->GetArrayLength( (jbyteArray)Arg );

		Array.Init( Size );

		if ( Size > 0 )
			Env->GetByteArrayRegion( (jbyteArray)Arg, 0, Size, Array.Core() );
	}

	void RetrieveAndFree_(
		jvalue &Arg,
		JNIEnv *Env,
		n4jre::sValue &ArgV )
	{
		bso::sBool IsObject = true;

		switch ( ArgV.Type ) {
		case n4jre::tByte:
		case n4jre::tInt:
		case n4jre::tLong:
			IsObject = false;
			// Nothing to free.
			break;
		case n4jre::tString:
			Retrieve_( Arg.l, Env, *ArgV.String );
			break;
		case n4jre::tByteArray:
			Retrieve_( Arg.l, Env, *ArgV.ByteArray );
			break;
		default:
			qRGnr();
			break;
		}

		if ( IsObject )
			Env->DeleteLocalRef( Arg.l );
	}

	void RetrieveAndFree_(
		int ArgC,
		n4jre::sValue *ArgV,
		jvalue *Arg,
		JNIEnv *Env )
	{
		while ( ArgC-- )
			RetrieveAndFree_( Arg[ArgC], Env, ArgV[ArgC] );
	}
}

typedef n4all::cCaller cCaller_;

namespace {
	void GetString_(
		jobject Object,
		JNIEnv *Env,
		void *RawString )
	{
		str::dString &String = *( str::dString * )RawString;

		if ( !Env->IsInstanceOf( Object, Env->FindClass( "Ljava/lang/String;" ) ) )
			qRGnr();

		jniq::Convert( (jstring)Object, String, Env );
	}

	void SetString_(
		const void *RawString,
		JNIEnv *Env,
		jobject &Object )
	{
	qRH
		qCBUFFERr Buffer;
	qRB
		const str::dString &String = *( const str::dString * )RawString;

		Object = Env->NewStringUTF( String.Convert( Buffer ) );
	qRR
	qRT
	qRE
	}
}

namespace {
	typedef n4jre::cObject cObject_;

	class rObject_
	: public cObject_
	{
	private:
		jobject Object_;
		template <typename type, typename function> type CallMethod_(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV,
			function Function,
			JNIEnv *Env )
		{
			type Return;
		qRH
			jvalue *Args = NULL;
		qRB
			Args = Fill_( ArgC, ArgV, Env );

			Return = Function( Env, Object_, jniq::GetMethodID( Object_, Method, Signature, Env ), Args );
		qRR
		qRT
			if ( Args != NULL ) {
				RetrieveAndFree_( ArgC, ArgV, Args, Env );
				delete Args;
			}
		qRE
			return Return;
		}
		void Set_(
			jobject JObject,
			JNIEnv *Env,
			n4jre::sValue &Object )
		{
			switch ( Object.Type ) {
			case n4jre::tByte:
			case n4jre::tInt:
			case n4jre::tLong:
				qRGnr();	// This types are not objects.
				break;
			case n4jre::tString:
				qRVct();
				break;
			case n4jre::tByteArray:
				Retrieve_( JObject, Env, *Object.ByteArray );
				break;
			default:
				qRGnr();
				break;
			}
		}
	protected:
		virtual void N4JRESet(
			const char *Name,
			const char *Signature,
			cObject *Value ) override
		{
			rObject_ &Object = *(rObject_ *)Value;

			jniq::SetObjectField( Object_, Name, Signature, Object.Object_, jniq::GetEnv() );
		}
		virtual void N4JRECallObjectMethod(
			const char *Method,
			const char *Signature,
			n4jre::sValue &Object,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			JNIEnv *Env = jniq::GetEnv();

			jobject JObject = CallMethod_<jobject>( Method, Signature, ArgC, ArgV, Env->functions->CallObjectMethodA, Env );

			Set_( JObject, Env, Object );
		}
		virtual void N4JRECallVoidMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
		qRH
			JNIEnv *Env = NULL;
			jvalue *Args = NULL;
		qRB
			JNIEnv *Env = jniq::GetEnv();

			Args = Fill_( ArgC, ArgV, Env );

			Env->CallVoidMethodA( Object_, jniq::GetMethodID( Object_, Method, Signature, Env ), Args );
		qRR
		qRT
			if ( Args != NULL ) {
				RetrieveAndFree_( ArgC, ArgV, Args, Env );
				delete Args;
			}
		qRE
		}
		virtual n4jre::sJShort N4JRECallShortMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			JNIEnv *Env = jniq::GetEnv();

			return CallMethod_<jshort>( Method, Signature, ArgC, ArgV, Env->functions->CallShortMethodA, Env );
		}
		virtual n4jre::sJInt N4JRECallIntMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			JNIEnv *Env = jniq::GetEnv();

			return CallMethod_<jint>( Method, Signature, ArgC, ArgV, Env->functions->CallIntMethodA, Env );
		}
		virtual n4jre::sJLong N4JRECallLongMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			JNIEnv *Env = jniq::GetEnv();

			return CallMethod_<jlong>( Method, Signature, ArgC, ArgV, Env->functions->CallLongMethodA, Env );
		}

	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Object_ != NULL )
					jniq::GetEnv()->DeleteGlobalRef( Object_ );
			}

			Object_ = NULL;
		}
		qCVDTOR( rObject_ );
		void Init( jobject Object )
		{
			Object_ = Object;
		}
		jobject Object( void )
		{
			return Object_;
		}
	};

	class sCaller_
	: public cCaller_ {
	private:
		qRMV( jobjectArray, A_, Args_ );
		jobject ReturnedValue_;
		virtual void N4ALLGetArgument(
			bso::sU8 Index,
			n4all::sType Type,
			void *Value ) override
		{
		qRH
			JNIEnv *Env = NULL;
			rObject_ *Object = NULL;
		qRB
			Env = jniq::GetEnv();

			if ( Index >= Env->GetArrayLength( A_() ) )
				qRFwk();

			if ( Type != n4jre::t_Undefined )
				qRGnr();

			Object = ::new rObject_ ;

			if ( Object == NULL )
				qRAlc();

			Object->Init( Env->NewGlobalRef( Env->GetObjectArrayElement( A_(), Index ) ) );

			*(n4jre::sJObject *)Value = Object;
		qRR
		qRT
		qRE
		}
		virtual void N4ALLSetReturnValue(
			n4all::sType Type,
			const void *Value ) override
		{
			JNIEnv *Env = jniq::GetEnv();

			if ( Type != n4jre::t_Undefined )
				qRGnr();

			if ( Value != NULL ) {
				rObject_ *Object = (rObject_ *)Value;

				ReturnedValue_ = Object->Object();
			} else
				ReturnedValue_ = NULL;
		}
	public:
		void reset( bso::sBool = true )
		{
			Args_ = NULL;
			ReturnedValue_ = NULL;
		}
		qCDTOR( sCaller_ );
		void Init( jobjectArray &Args )
		{
			Args_ = &Args;
			ReturnedValue_ = NULL;
		}
		jobject GetReturnedValue( void )
		{
			return ReturnedValue_;
		}
	};
}

n4jre::cObject *wrapper::NewObject(
	const char *ClassName,
	const char *Signature,
	int ArgC,
	n4jre::sValue *ArgV )
{
	rObject_ *Object = NULL;
qRH
	jvalue *Args = NULL;
	JNIEnv *Env = NULL;
	jclass Class = NULL;
qRB
	Env = jniq::GetEnv();
	Class = jniq::FindClass( ClassName, Env );

	Args = Fill_( ArgC, ArgV, Env );

	Object = ::new rObject_;

	if ( Object == NULL )
		qRAlc();

	if ( ( ClassName == NULL ) || ( *ClassName == 0 ) )
		Object->Init( NULL );
	else
		Object->Init( Env->NewObjectA( Class, jniq::GetMethodID( Class, "<init>", Signature, Env ), Args ) );
qRR
	if ( Object != NULL )
		delete Object;
qRT
	if ( Args != NULL ) {
		RetrieveAndFree_( ArgC, ArgV, Args, Env );
		delete Args;
	}
qRE
	return Object;
}


jobject wrapper::Launch(
	jint Index,
	jobjectArray &Args )
{
	jobject Return = NULL;
qRH
	sCaller_ Caller;
qRB
	Caller.Init( Args );

	n4allw::Launch( Index, Caller );

//	n4allw::GetLauncher().Launch( n4allw::GetFunction( Index ), Caller );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.

	Return = Caller.GetReturnedValue();
qRR
qRT
qRE
	return Return;
}


