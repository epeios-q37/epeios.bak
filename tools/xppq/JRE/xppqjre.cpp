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
