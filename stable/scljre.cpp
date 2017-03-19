/*
	Copyright (C) 1999-2017 Claude SIMON (http://q37.info/contact/).

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

#define SCLJRE_COMPILATION_

#include "scljre.h"

#include "sclerror.h"
#include "scllocale.h"
#include "sclmisc.h"

using namespace scljre;

namespace {
	bch::qBUNCHwl( sFunction_ ) Functions_;
}

void scljre::sRegistrar::Register( sFunction_ Function )
{
	Functions_.Append( Function );
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

void scljre::Register_( JNIEnv *Env )
{
qRFH
	str::wString Location;
qRFB
	sRegistrar Registrar;

	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	// TODO : Find a way to fill 'Location' with the path of the binary.

	sclmisc::Initialize( Rack_, Location, qRPU );

	Registrar.Init();

	Functions_.Init();
	scljre::SCLJRERegister( Registrar );
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
}

jobject scljre::Launch_(
	JNIEnv *Env,
	int Index,
	jobjectArray Args )
{
	jobject Result;
qRFH
	sArguments Arguments;
qRFB
	if ( !Functions_.Exists( Index ) )
		qRGnr();

	Arguments.Init( Args );

	Result = Functions_( Index )( Env, Arguments );

	if ( sclerror::IsErrorPending() )
		qRAbort();	// To force the handling of a pending error.
qRFR
qRFT
qRFE( ERRFinal_( Env ) )
	return Result;
}

qGCTOR( scljre )
{
	Error_.Init();
	SCLError_.Init();
	Locale_.Init();
}



