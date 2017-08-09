/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of JREq.

	JREq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	JREq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with JREq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "jreq.h"

#include "jrebse.h"
#include "n4a.h"

#include "sclmisc.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

#define MDEF( name ) qCDEF( char *, name, #name );

namespace messages_ {
	MDEF( NoRegisteredComponent );
}

namespace {
	namespace{
		void GetInfo_(
			jint Version,
			str::dString &Info )
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sOFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			Flow << sclmisc::SCLMISCProductName << " v" << VERSION << " - JNI v" << ( ( Version & 0xff00 ) >> 16 ) << '.' << ( Version & 0xff )  << txf::nl
				<< txf::pad << "Build : " __DATE__ " " __TIME__ " (" <<  cpe::GetDescription() << ')';
		qRR
		qRT
		qRE
		}
	}

	jstring GetInfo_( JNIEnv *Env )
	{
		jstring JString;
	qRH
		str::wString Info;
		qCBUFFERr Buffer;
	qRB
		Info.Init();
		GetInfo_( Env->GetVersion(), Info );

		JString = Env->NewStringUTF( Info.Convert( Buffer ) );
	qRR
	qRT
	qRE
		return JString;
	}
}

extern "C" JNIEXPORT jstring JNICALL Java_JREq_wrapperInfo(
	JNIEnv *Env,
	jclass )
{
	return GetInfo_( Env );
}

namespace {
	err::err___ Error_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;

	void ERRFinal_( JNIEnv *Env )
	{
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message );
	}
}

extern "C" JNIEXPORT jstring JNICALL Java_JREq_componentInfo(
	JNIEnv *Env,
	jclass )
{
	jstring String;
qRFH
	str::wString Message;
	qCBUFFERr Buffer;
qRFB
	Message.Init();

	sclmisc::GetBaseTranslation( messages_::NoRegisteredComponent, Message );

	String = Env->NewStringUTF( Message.Convert( Buffer ) );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
	return String;
}

extern "C" JNIEXPORT void JNICALL Java_JREq_register(
	JNIEnv *Env,
	jclass)
{
qRFH
	str::wString Location;
qRFB
//	sRegistrar Registrar;

	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	// TODO : Find a way to fill 'Location' with the path of the binary.

	sclmisc::Initialize( Rack_, Location, qRPU );

//	Registrar.Init();

//	Functions_.Init();
//	scljre::SCLJRERegister( Registrar );

	jniq::SetGlobalEnv( Env );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
}

extern "C" JNIEXPORT jobject JNICALL Java_JREq_wrapper(
	JNIEnv *Env,
	jclass,
	jint Index,
	jobjectArray Args )
{
//	return scljre::Launch_( Env, Index, Args );
	return NULL;
}


const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;


qGCTOR( jreq )
{
	Error_.Init();
	SCLError_.Init();
	Locale_.Init();
}