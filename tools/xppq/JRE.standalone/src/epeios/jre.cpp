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

#define JRE_COMPILATION_

#include "jre.h"

using namespace jre;

fdr::sSize jre::rInputStreamIDriver::FDRRead(
	fdr::sSize Maximum,
	fdr::sByte *Buffer )
{
qRH
	jbyteArray Array = NULL;
	JNIEnv *Env = NULL;
qRB
	if ( Maximum > 5 )
		Maximum = 5;

	Env = jniq::GetEnv( NULL );

	Array = Env->NewByteArray( (jint)Maximum );

	if ( Array == NULL )
		qRAlc();

	Maximum = Stream_.Read( Array, 0, (jint)Maximum, Env );

	if ( Maximum != -1 )
		Env->GetByteArrayRegion( Array, 0, (jint)Maximum, (jbyte *)Buffer );
	else
		Maximum = 0;
qRR
qRT
	// No need to 'delete' 'Array'.
qRE
	return Maximum;
}
