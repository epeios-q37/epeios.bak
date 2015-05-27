/*
	Copyright (C) 2000-2015 Claude SIMON (http://q37.info/contact/).

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

#define JVABSE__COMPILATION

#include "jvabse.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace jvabse;

const str::string_ &jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	str::string_ &String )
{
ERRProlog
	const char *Buffer = NULL;
ERRBegin
	if ( ( Buffer = Env->GetStringUTFChars( JString, NULL ) ) == NULL )
		ERRLbr();

	String.Append( Buffer );
ERRErr
ERREnd
	if ( Buffer != NULL )
		Env->ReleaseStringUTFChars( JString, Buffer );
ERREpilog
	return String;
}

const char *jvabse::Convert(
	jstring JString,
	JNIEnv *Env,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string String;
ERRBegin
	String.Init();

	Convert( JString, Env, String );

	String.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}
