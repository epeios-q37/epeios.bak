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

#include "jre.h"

#include "sclmisc.h"

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

# define NAME_MC			"JREQ"
# define NAME_LC			"jreq"
# define NAME_UC			"JREq"
# define WEBSITE_URL		"http://q37.info/"
# define AUTHOR_NAME		"Claude SIMON"
# define AUTHOR_CONTACT		"http://q37.info/contact/"
# define OWNER_NAME			"Claude SIMON"
# define OWNER_CONTACT		"http://q37.info/contact/"
# define COPYRIGHT			COPYRIGHT_YEARS " " OWNER_NAME " (" OWNER_CONTACT ")"

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
		jre::sString JString;
	qRH
		str::wString Info;
	qRB
		Info.Init();
		GetInfo_( Env->GetVersion(), Info );

		JString.Init( Info );
	qRR
	qRT
	qRE
		return JString;
	}
}

extern "C" JNIEXPORT jstring JNICALL Java_JREq_wrapperInfo(
	JNIEnv *Env,
	jclass)
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