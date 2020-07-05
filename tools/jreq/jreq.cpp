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

#include "registry.h"
#include "wrapper.h"

#include "jrebse.h"
#include "n4jre.h"

#include "scla.h"
#include "sclm.h"

#include "epsmsc.h"
#include "iof.h"
#include "xpp.h"
#include "lcl.h"

SCLI_DEF( jreq, NAME_LC, NAME_MC );

#define MDEF( name ) qCDEF( char *, name, #name );

namespace {
	wrapper::rLauncher &GetLauncher_( jlong Long )
	{
		if ( Long == 0 )
			qRGnr();

		return *(wrapper::rLauncher *)Long;
	}
}

namespace {
	namespace{
		void GetInfo_(
			jint Version,
			str::dString &Info )
		{
		qRH
			flx::rStringOFlow BaseFlow;
			txf::sWFlow Flow;
		qRB
			BaseFlow.Init( Info );
			Flow.Init( BaseFlow );

			Flow << jreq::Info.Product() << " v" << VERSION << " - JNI v" << ( ( Version & 0xffff0000 ) >> 16 ) << '.' << ( Version & 0xffff )  << txf::nl
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
		qCBUFFERh Buffer;
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

extern "C" JNIEXPORT jstring JNICALL Java_info_q37_jreq_Wrapper_wrapperInfo(
	JNIEnv *Env,
	jclass )
{
	return GetInfo_( Env );
}

namespace {
	err::err___ Error_;
	scle::rError SCLError_;
	scll::rRack Locale_;
	sclm::sRack Rack_;

	void ERRFinal_( JNIEnv *Env )
	{
	qRH
		str::wString Message;
		err::buffer__ RBuffer;
		qCBUFFERh CBuffer;
	qRB
		Message.Init();

		if ( ERRType != err::t_Abort ) {
			Message.Append( err::Message( RBuffer ) );

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.
		} else if ( scle::IsErrorPending() )
			sclm::GetSCLBasePendingErrorTranslation( Message );

		Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message.Convert( CBuffer ) );
	qRR
		ERRRst();
	qRT
	qRE
	}
}

extern "C" JNIEXPORT jstring JNICALL Java_info_q37_jreq_Wrapper_componentInfo(
	JNIEnv *Env,
	jclass,
	jlong Launcher )
{
	jstring JString;
qRFH
	str::wString Info;
	qCBUFFERh Buffer;
qRFB
	Info.Init();

	if ( !GetLauncher_( Launcher ).GetInfo( Info ) )
		sclm::GetBaseTranslation( "NoRegisteredComponent", Info );

	JString = Env->NewStringUTF( Info.Convert( Buffer ) );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
	return JString;
}

extern "C" JNIEXPORT void JNICALL Java_info_q37_jreq_Wrapper_init(
	JNIEnv *Env,
	jclass,
	jstring RawLocation )
{
qRFH
	str::wString Location;
qRFB
	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	jniq::Convert( Env, RawLocation, Location);
	// TODO : Find a way to fill 'Location' with the path of the binary.

	cio::COut << ">>>>>>>>> " << Location << txf::nl << txf::commit;

	sclm::Initialize( Rack_, Location, jreq::Info );

	jniq::SetGlobalEnv( Env );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
}

namespace {
	n4jre::gShared Shared_;
}

n4jre::fMalloc n4jre::N4JREMalloc = malloc;
n4jre::fFree n4jre::N4JREFree = free;

namespace {
	inline void Delete_( n4jre::cObject *Object )
	{
		delete Object;
	}
	inline void Throw_(
		n4jre::sEnv *RawEnv,
		const char *Message )
	{
		JNIEnv *Env = (JNIEnv *)RawEnv;

		if ( Env->ExceptionOccurred() == NULL )
			Env->ThrowNew( Env->FindClass( "java/lang/Exception"), Message );

//		qRAbort();
	}
}


extern "C" JNIEXPORT jlong JNICALL Java_info_q37_jreq_Wrapper_register(
	JNIEnv *Env,
	jclass,
	jstring RawArguments )
{
	wrapper::rLauncher *Launcher = NULL;
qRFH;
	str::wString Arguments;
	str::wString ComponentFilename;
qRFB;
	Arguments.Init();
	jniq::Convert( Env, RawArguments, Arguments );

	scla::FillRegistry( Arguments, scla::faIsArgument, scla::uaReport );

	ComponentFilename.Init();
	sclm::MGetValue( registry::parameter::ComponentFilename, ComponentFilename );

	Shared_.NewObject = wrapper::NewObject;
	Shared_.NewObjectArray = wrapper::NewObjectArray;
	Shared_.Delete = Delete_;
	Shared_.Malloc = n4jre::N4JREMalloc;
	Shared_.Free = n4jre::N4JREFree;
	Shared_.Throw = Throw_;

	Launcher = new wrapper::rLauncher;

	if ( Launcher == NULL )
		qRAlc();

	Launcher->Init(ComponentFilename, dlbrry::nPrefixAndExt, Rack_, &Shared_, false);
qRFR;
	if ( Launcher != NULL )
		delete Launcher;
qRFT;
qRFE( ERRFinal_( Env ) )
	return (jlong)Launcher;
}

extern "C" JNIEXPORT jobject JNICALL Java_info_q37_jreq_Wrapper_call(
	JNIEnv *Env,
	jclass,
	jlong Launcher,
	jint Index,
	jobjectArray Args )
{
	jobject Return = NULL;
qRFH
qRFB
	Return = wrapper::Call( Env, GetLauncher_( Launcher ), Index, Args );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
	return Return;
}

qGCTOR( jreq )
{
	Error_.Init();
	SCLError_.Init();
	Locale_.Init();
};
