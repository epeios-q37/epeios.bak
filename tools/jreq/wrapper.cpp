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
		JNIEnv *Env,
		int ArgC,
		const n4jre::sValue *ArgV,
		jvalue *Arg )
	{
		while ( ArgC-- )
			Fill_( ArgV[ArgC], Env, Arg[ArgC] );
	}

	jvalue *Fill_(
		JNIEnv *Env,
		int ArgC,
		const n4jre::sValue *ArgV )
	{
		jvalue *Args = NULL;
	qRH
	qRB
		if ( ArgC != 0 ) {
			Args = new jvalue[ArgC];

			if ( Args == NULL )
				qRAlc();

			Fill_( Env, ArgC, ArgV, Args );
		}
	qRR
	qRT
	qRE
		return Args;
	}

	void Retrieve_(
		JNIEnv *Env,
		jobject Arg,
		n4jre::rJString &String )
	{
		jsize Size = Env->GetStringUTFLength( (jstring)Arg );

		String.Init( Size + 1 );	// '+1' for the terminating NUL.

		if ( Size > 0 )
			Env->GetStringUTFRegion( (jstring)Arg, 0, Size, String.Core() );
	}

	void Retrieve_(
		JNIEnv *Env,
		jobject Arg,
		n4jre::rJByteArray &Array )
	{
		jsize Size = Env->GetArrayLength( (jbyteArray)Arg );

		Array.Init( Size );

		if ( Size > 0 )
			Env->GetByteArrayRegion( (jbyteArray)Arg, 0, Size, Array.Core() );
	}

	void RetrieveAndFree_(
		JNIEnv *Env,
		jvalue &Arg,
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
			Retrieve_( Env, Arg.l, *ArgV.String );
			break;
		case n4jre::tByteArray:
			Retrieve_( Env, Arg.l, *ArgV.ByteArray );
			break;
		default:
			qRGnr();
			break;
		}

		if ( IsObject )
			Env->DeleteLocalRef( Arg.l );
	}

	void RetrieveAndFree_(
		JNIEnv *Env,
		int ArgC,
		n4jre::sValue *ArgV,
		jvalue *Arg )
	{
		while ( ArgC-- )
			RetrieveAndFree_( Env, Arg[ArgC], ArgV[ArgC] );
	}
}

typedef n4all::cCaller cCaller_;

namespace {
	void GetString_(
		JNIEnv *Env,
		jobject Object,
		void *RawString )
	{
		str::dString &String = *( str::dString * )RawString;

		if ( !Env->IsInstanceOf( Object, Env->FindClass( "Ljava/lang/String;" ) ) )
			qRGnr();

		jniq::Convert( Env, (jstring)Object, String );
	}

	void SetString_(
		JNIEnv *Env,
		const void *RawString,
		jobject &Object )
	{
	qRH
		qCBUFFERh Buffer;
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
		JNIEnv * Env_;
		jobject Object_;
		template <typename type, typename function> type CallMethod_(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV,
			function Function )
		{
			type Return;
		qRH
			jvalue *Args = NULL;
		qRB
			Args = Fill_( Env_, ArgC, ArgV );

			Return = Function( Env_, Object_, jniq::GetMethodID( Env_, Object_, Method, Signature ), Args );
		qRR
		qRT
			if ( Args != NULL ) {
				RetrieveAndFree_( Env_, ArgC, ArgV, Args );
				delete Args;
			}
		qRE
			return Return;
		}
		void Set_(
			jobject JObject,
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
				Retrieve_( Env_, JObject, *Object.ByteArray );
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

			jniq::SetObjectField( Env_, Object_, Name, Signature, Object.Object_ );
		}
		virtual void N4JRECallObjectMethod(
			const char *Method,
			const char *Signature,
			n4jre::sValue &Object,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			jobject JObject = CallMethod_<jobject>( Method, Signature, ArgC, ArgV, Env_->functions->CallObjectMethodA );

			Set_( JObject, Object );
		}
		virtual n4jre::sJSize N4JREGetLength( void ) override
		{
			return jniq::GetLength( Env_, Object_ );
		}
		virtual cObject *N4JREGetElement( n4jre::sJSize Index ) override
		{
			rObject_ *Object = NULL;
		qRH;
		qRB;
			if ( ( Object = new rObject_ ) == NULL  )
				qRAlc();

			Object->Init( Env_, jniq::GetElement( Env_, Object_, Index ) );
		qRR;
			if ( Object != NULL )
				delete Object;
		qRT;
		qRE;
			return Object;
		}
		virtual void N4JRESetElement(
			n4jre::sJSize Index,
			cObject *Object ) override
		{
			jniq::SetElement( Env_, Object_, Index, ((rObject_ *)Object)->Object_ );
		}
		virtual void N4JRECallVoidMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
		qRH
			jvalue *Args = NULL;
		qRB
			Args = Fill_( Env_, ArgC, ArgV );

			Env_->CallVoidMethodA( Object_, jniq::GetMethodID( Env_, Object_, Method, Signature ), Args );
		qRR
		qRT
			if ( Args != NULL ) {
				RetrieveAndFree_( Env_, ArgC, ArgV, Args );
				delete Args;
			}
		qRE
		}
		virtual n4jre::sJBoolean N4JRECallBooleanMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			return CallMethod_<jboolean>( Method, Signature, ArgC, ArgV, Env_->functions->CallBooleanMethod );
		}
		virtual n4jre::sJShort N4JRECallShortMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			return CallMethod_<jshort>( Method, Signature, ArgC, ArgV, Env_->functions->CallShortMethodA );
		}
		virtual n4jre::sJInt N4JRECallIntMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			return CallMethod_<jint>( Method, Signature, ArgC, ArgV, Env_->functions->CallIntMethodA );
		}
		virtual n4jre::sJLong N4JRECallLongMethod(
			const char *Method,
			const char *Signature,
			int ArgC,
			n4jre::sValue *ArgV ) override
		{
			return CallMethod_<jlong>( Method, Signature, ArgC, ArgV, Env_->functions->CallLongMethodA );
		}

	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Object_ != NULL )
					Env_->DeleteGlobalRef( Object_ );
			}

			Env_ = NULL;
			Object_ = NULL;
		}
		qCVDTOR( rObject_ );
		void Init(
			JNIEnv *Env,
			jobject Object )
		{
			Env_ = Env;
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
		JNIEnv * Env_;
		qRMV( jobjectArray, A_, Args_ );
		jobject ReturnedValue_;
		virtual void N4ALLGetArgument(
			bso::sU8 Index,
			n4all::sType Type,
			void *Value ) override
		{
		qRH
			rObject_ *Object = NULL;
		qRB
			if ( Index >= Env_->GetArrayLength( A_() ) )
				qRFwk();

			if ( Type != n4jre::t_Undefined )
				qRGnr();

			Object = ::new rObject_ ;

			if ( Object == NULL )
				qRAlc();

			Object->Init( Env_, Env_->NewGlobalRef( Env_->GetObjectArrayElement( A_(), Index ) ) );

			*(n4jre::sJObject *)Value = Object;
		qRR
		qRT
		qRE
		}
		virtual void N4ALLSetReturnValue(
			n4all::sType Type,
			const void *Value ) override
		{
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
		void Init(
			JNIEnv_ *Env,
			jobjectArray &Args )
		{
			Env_ = Env;
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
	n4jre::sEnv *RawEnv,
	const char *ClassName,
	const char *Signature,
	int ArgC,
	n4jre::sValue *ArgV )
{
	rObject_ *Object = NULL;
qRH
	jvalue *Args = NULL;
	jclass Class = NULL;
	JNIEnv *Env = NULL;
	qRB
		Env = (JNIEnv *)RawEnv;
	Class = jniq::FindClass( Env, ClassName );

	Args = Fill_( Env, ArgC, ArgV );

	Object = ::new rObject_;

	if ( Object == NULL )
		qRAlc();

	if ( ( ClassName == NULL ) || ( *ClassName == 0 ) )
		Object->Init( Env, NULL );
	else
		Object->Init( Env, Env->NewObjectA( Class, jniq::GetMethodID( Env, Class, "<init>", Signature ), Args ) );
qRR
	if ( Object != NULL )
		delete Object;
qRT
	if ( Args != NULL ) {
		RetrieveAndFree_( Env, ArgC, ArgV, Args );
		delete Args;
	}
qRE
	return Object;
}

n4jre::cObject *wrapper::NewObjectArray(
	n4jre::sEnv *RawEnv,
	n4jre::sJSize Length,
	const char *ClassName )
{
	rObject_ *Object = NULL;
qRH;
	jclass Class = NULL;
	JNIEnv *Env = NULL;
qRB
	Env = (JNIEnv *)RawEnv;
	Class = jniq::FindClass( Env, ClassName );

	Object = ::new rObject_;

	if ( Object == NULL )
		qRAlc();

	if ( (ClassName == NULL) || (*ClassName == 0) )
		qRGnr();

	Object->Init( Env, Env->NewObjectArray( Length, Class, NULL ) );
qRR
	if ( Object != NULL )
		delete Object;
qRT
qRE
	return Object;
}


jobject wrapper::Call(
	JNIEnv *Env,
	rLauncher &Launcher,
	jint Index,
	jobjectArray &Args )
{
	jobject Return = NULL;
qRH
	sCaller_ Caller;
qRB
	Caller.Init( Env, Args );

	Launcher.Call( (n4jre::sEnv*)Env, Index, Caller );

//	n4allw::GetLauncher().Launch( n4allw::GetFunction( Index ), Caller );

	if ( scle::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.

	Return = Caller.GetReturnedValue();
qRR
qRT
qRE
	return Return;
}


