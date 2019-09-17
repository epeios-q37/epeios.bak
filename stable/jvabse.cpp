/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

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

#define JVABSE_COMPILATION_

#include "jvabse.h"

using namespace jvabse;

const str::string_ &jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	str::string_ &String )
{
qRH
	const char *Buffer = NULL;
qRB
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

const char *jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
qRH
	str::string String;
qRB
	String.Init();

	Convert( JString, Env, String );

	String.Convert( Buffer );
qRR
qRT
qRE
	return Buffer;
}
