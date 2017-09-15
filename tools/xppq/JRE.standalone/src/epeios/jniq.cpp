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

#define JNIQ__COMPILATION

#include "jniq.h"

using namespace jniq;

namespace {
	JNIEnv *Env_ = NULL;
}

void jniq::SetGlobalEnv( JNIEnv *Env )
{
	if ( Env_ != NULL )
		qRFwk();

	Env_ = Env;
}

JNIEnv *jniq::GetGlobalEnv( void )
{
	if ( Env_ == NULL )
		qRFwk();

	return Env_;
}

const str::string_ &jniq::Convert(
	jstring JString,
	str::string_ &String,
	JNIEnv *Env )
{
qRH
	const char *Buffer = NULL;
qRB
	Env = GetEnv( Env );

	if ( ( Buffer = Env->GetStringUTFChars( JString, NULL ) ) == NULL )
		qRLbr();

	String.Append( Buffer );
qRR
qRT
	if ( Buffer != NULL )
		Env->ReleaseStringUTFChars( JString, Buffer );
qRE
	return String;
}

const char *jniq::Convert(
	jstring JString,
	qCBUFFERr &Buffer,
	JNIEnv *Env )
{
qRH
	str::string String;
qRB
	Env = GetEnv( Env );

	String.Init();

	Convert( JString, String, Env );

	String.Convert( Buffer );
qRR
qRT
qRE
	return Buffer;
}


