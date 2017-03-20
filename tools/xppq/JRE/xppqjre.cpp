/*
	'jexpp' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'jexpp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'jexpp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'jexpp'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "jre.h"

#include "sclmisc.h"
#include "scljre.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

# define NAME_MC			"XPPqJRE"
# define NAME_LC			"xppqjre"
# define NAME_UC			"XPPQJRE"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"	

/*
static void Print_(
	JNIEnv *Env,
	const char *Text )
{
	jclass System = Env->FindClass( "java/lang/System" );

	jobject Out = jniq::GetStaticObjectField( Env, System, "out", "Ljava/io/PrintStream;" );

	jcharArray Array = Env->NewCharArray( 1000 );
	

	Env->CallNonvirtualVoidMethod( Out, jniq::GetClass( Env, Out ), jniq::GetMethodID( Env, Out, "println", "(Ljava/lang/String;)V" ), Env->NewStringUTF( Text ) );

	Env->CallNonvirtualVoidMethod( Out, jniq::GetClass( Env, Out ), jniq::GetMethodID( Env, Out, "flush", "()V" ) );
}
*/

static void Print_(
	JNIEnv *Env,
	const char *Text )
{
	jniobj::java::lang::sSystem::Out( Env ).Println( Env, Text );
	jniobj::java::lang::sSystem::Out( Env ).Flush( Env );
}


static void Print_(
	JNIEnv *Env,
	jobject Object )
{
	jniobj::java::lang::sSystem::Out( Env ).Println( Env, Object );
	jniobj::java::lang::sSystem::Out( Env ).Flush( Env );
}


static void PrintNew_(
	JNIEnv *Env,
	jobject Object )
{
	jniobj::java::io::sPrintStream Out;

	jclass System = Env->FindClass( "java/lang/System" );

	Out.Init( Env, jniq::GetStaticObjectField( Env, System, "out", "Ljava/io/PrintStream;" ) );

	Out.Println(Env, Object );
	Out.CallVoidMethod(Env, "flush", "()V", Object );
}

/*

#define BUFFER_SIZE	JVASTF_BUFFER_SIZE

struct data___ {
	xpp::preprocessing_iflow___ *XPPFlow;
	jvabse::jni_param__ Param;
	jvastf::input_stream_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ XPP;
	~data___( void )
	{
		XPPFlow = NULL;
		XPP.reset();
		XFlow.reset();
		Flow.reset();
	}
};

static flw::iflow__ *GetFlow_(
	JNIEnv *Env,
	jobject Object )
{
	flw::iflow__ *Flow = NULL;
ERRProlog
	xpp::context___ Context;
	lcl::locale Locale;
	lcl::meaning ErrorMeaning;
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	data___ &Data = *(data___ *)GetLongField( Env, Object, "core" );

	Data.Param.Env = Env;
	Data.Param.Object = GetObjectField( Env, Object, "in", "Ljava/io/InputStream;" );

	if ( !Data.XPPFlow->EndOfFlow() ) {
		Flow = Data.XPPFlow;
	} else 
		switch ( Data.XPPFlow->GetContext( Context ).Status )
		{
		case xml::sOK:
			break;
		default:
			Locale.Init();

			ErrorMeaning.Init();
			xpp::GetMeaning( Context, ErrorMeaning );

			Translation.Init();
			Locale.GetTranslation( ErrorMeaning, "", Translation );

			Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Translation.Convert( Buffer ) );
			break;
		}
ERRErr
ERREnd
ERREpilog
	return Flow;
}

EXPORT jint JNICALL Java_org_zeusw_XPPInputStream_available(
	JNIEnv *Env,
	jobject Object )
{
	bso::size__ Amount = 0;
ERRJProlog
	flw::iflow__ *Flow = NULL;
ERRJBegin
	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL )
		Amount = Flow->IsCacheEmpty( Amount );

	if ( Amount < 0 )
		ERRFwk();
	else if ( Amount > ULONG_MAX )
		Amount = ULONG_MAX;

ERRJErr
ERRJEnd
ERRJEpilog
	return (jint)Amount;
}

EXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read__(
	JNIEnv *Env,
	jobject Object )
{
	jint C = -1;
ERRJProlog
	flw::iflow__ *Flow = NULL;
ERRJBegin
	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL )
		C = Flow->Get();
ERRJErr
ERRJEnd
ERRJEpilog
	return C;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	jsize Amount = len - off;
ERRJProlog
	flw::iflow__ *Flow = NULL;
	fdr::datum__ Buffer[BUFFER_SIZE];
ERRJBegin
	if ( Amount > BUFFER_SIZE )
		Amount = BUFFER_SIZE;

	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL ) {
		Amount = (jsize)Flow->ReadUpTo( Amount, Buffer );

		if ( Amount == 0 )
			Amount = -1;
		else
			Env->SetByteArrayRegion( b, off, Amount, (const jbyte *)Buffer );
	} else
		Amount = -1;
ERRJErr
ERRJEnd
ERRJEpilog
	return Amount;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b, 0, Env->GetArrayLength( b ) );
}

EXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3B(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b );
}

EXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3BII(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	return Read( Env, Object, b, off, len );
}



EXPORT void JNICALL Java_org_zeusw_XPPInputStream_constructor(
  JNIEnv *Env,
  jobject Object )
{
ERRJProlog
	data___ *Data = NULL;
	xpp::criterions___ C;
	str::string D, K;
ERRJBegin
	Data = new data___;

	if ( Data == NULL )
		ERRAlc();

	Data->Param.Env = Env;

	Data->Param.Object = GetObjectField( Env, Object, "in", "Ljava/io/InputStream;" );

	Data->Flow.Init( Data->Param );

	Data->XFlow.Init( Data->Flow, utf::f_Guess );

#if 1
	D.Init( ".." );
#else
	D.Init();
	Print_( Env, __LOC__ );
	D.Append( '.' );
	Print_( Env, __LOC__ );
	D.Append( '.' );

#endif
	K.Init();

	C.Init( D, K, K );

	Data->XPP.Init( Data->XFlow, C );

	Data->XPPFlow = &Data->XPP;

	SetLongField( Env, Object, "core", (jlong)Data );
ERRJErr
ERRJEnd
ERRJEpilog
}


EXPORT void JNICALL Java_org_zeusw_XPPInputStream_destructor(
	JNIEnv *Env,
	jobject Object )
{
ERRJProlog
ERRJBegin
	delete (data___ *)GetLongField( Env, Object, "core" );
ERRJErr
ERRJEnd
ERRJEpilog
}
*/

SCLJRE_DEF( XPPQ );

namespace {
	class rProcessor
	{
	private:
		jre::rInputStreamIDriver Input_;
		flw::sDressedIFlow<> Flow_;
		xtf::extended_text_iflow__ XFlow_;
		xpp::preprocessing_iflow___ PFlow_;
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Input_, Flow_, XFlow_, PFlow_ );
		}
		void Init(
			JNIEnv *Env,
			jobject InputStream )
		{
			Input_.Init( Env, InputStream );
			Flow_.Init( Input_ );
			XFlow_.Init( Flow_, utf::f_Default );
			PFlow_.Init(XFlow_, xpp::criterions___( str::wString() ) );
		}
		fdr::sByte Get( void )
		{
			return PFlow_.Get();
		}
		bso::sBool IsEOF( void )
		{
			return PFlow_.EndOfFlow();
		}
	};

	jobject New_(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		rProcessor *Processor = new rProcessor;

		if ( Processor == NULL )
			qRAlc();

		Processor->Init( Env, Args.Get( Env ) );

		return jniobj::java::lang::sLong( Env, (jlong)Processor );
	}

	jobject Delete_(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		delete (rProcessor *)jniobj::java::lang::sLong(Env, Args.Get( Env ) ).LongValue( Env );

		return NULL;
	}

	jobject Read_(
		JNIEnv *Env,
		const scljre::sArguments &Args )
	{
		rProcessor &Processor = *(rProcessor *)jniobj::java::lang::sLong(Env, Args.Get( Env ) ).LongValue( Env );

		if ( Processor.IsEOF() )
			return jniobj::java::lang::sInteger( Env, (jint)-1 );
		else
			return jniobj::java::lang::sInteger( Env, Processor.Get() );
	}
}

void scljre::SCLJRERegister( sRegistrar &Registrar )
{
	Registrar.Register( New_,  Delete_,  Read_ );
}


const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;
