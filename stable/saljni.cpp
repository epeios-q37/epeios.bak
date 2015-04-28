/* begin of 'entete.txt' template file V1.1 */
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
