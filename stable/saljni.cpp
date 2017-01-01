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
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.8 */
/* 
	See the 'saljni.h' file  for more details.
*/

#define SALJNI__COMPILATION

#include "saljni.h"

#ifdef SALJNI__LINE
#line 12
#endif

#include <jni.h>	// this comes with the JDK (www.javasoft.com)

flw__flow_ *SALJNIFlow = NULL;

void saljni__manager::Process( void )
{
	SALJNIFlow = &salcsm__manager::Process();
}

extern "C" {

JNIEXPORT void JNICALL Java_epeios_broker_Local__1Get
  (JNIEnv *, jobject, jint, jbyteArray);

JNIEXPORT void JNICALL Java_epeios_broker_Local__1Put
  (JNIEnv *, jobject, jbyteArray, jint);

JNIEXPORT void JNICALL Java_epeios_broker_Local__1Sync
  (JNIEnv *, jobject);
}


JNIEXPORT void JNICALL Java_epeios_broker_Local__1Get
  (JNIEnv *Env, jobject, jint Amount, jbyteArray Buffer)
{
	
	jbyte *T = Env->GetByteArrayElements( Buffer, NULL );

	SALJNIFlow->Get( Amount, T );

	Env->ReleaseByteArrayElements( Buffer, T, 0 );
}

JNIEXPORT void JNICALL Java_epeios_broker_Local__1Put
  (JNIEnv *Env, jobject, jbyteArray Buffer, jint Amount)
{
	jbyte *T = Env->GetByteArrayElements( Buffer, NULL );

	SALJNIFlow->Put( T, Amount );

	Env->ReleaseByteArrayElements( Buffer, T, JNI_ABORT );
}

JNIEXPORT void JNICALL Java_epeios_broker_Local__1Sync
  (JNIEnv *, jobject)
{
	SALJNIFlow->Synchronize();
}
