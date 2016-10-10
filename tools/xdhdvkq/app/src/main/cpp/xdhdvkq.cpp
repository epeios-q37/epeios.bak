/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'xdhdvkq'.

    'xdhbrwq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xdhbrwq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'xdhbrwq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <jni.h>
#include <string>
#include <android/asset_manager.h>

#include "tol.h"
#include "sclerror.h"
#include "sclmisc.h"


namespace {
    err::err___ qRRor_;
    sclerror::rError SCLError_;
    scllocale::rRack Locale_;

    void ThrowError_(
            JNIEnv *Env,
            const char *Message )
    {
        jclass exClass;
        char *className = "java/lang/Error";

        exClass = Env->FindClass(className);

        Env->ThrowNew( exClass, Message);
    }

    void ThrowError_(
            JNIEnv *Env,
            const str::dString &Message )
    {
        bso::bool__ Exists = false;
    qRH
        TOL_CBUFFER___ Buffer;
    qRB
        ThrowError_( Env, Message.Convert( Buffer ) );
    qRR
    qRT
    qRE
    }

    void ThrowSCLBasePendingError_( JNIEnv *Env )
    {
    qRH
        str::string Translation;
    qRB
        Translation.Init();

        if ( ( sclmisc::GetSCLBasePendingErrorTranslation( Translation ) ) )
            sclerror::ResetPendingError();

        ThrowError_( Env, Translation );
    qRR
    qRT
    qRE
    }


    void ErrFinal_( JNIEnv *Env )
    {
    qRH
        str::string Message;
    qRB
        if ( ERRType != err::t_Abort ) {
            err::buffer__ Buffer;

            Message.Init( " { " );

            Message.Append( err::Message( Buffer ) );

            Message.Append( " }" );

            ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

            ThrowError_( Env, Message );

        } else if ( sclerror::IsErrorPending() )
            ThrowSCLBasePendingError_( Env );
        else
            ERRRst();
    qRR
    qRT
    qRE
    }
}

extern "C"
void Java_info_q37_xdhdvkq_MainActivity_initialize(
    JNIEnv* Env,
    jobject MainActivity )
{
qRFH
    sclmisc::sRack Rack;
    fnm::rName Dir;
qRFB
    Rack.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_);
    Dir.Init();
    AAssetManager_open( )
    sclmisc::Initialize( Rack, AAssetManager_openDir(mgr, "") );
    sclmisc::ReportAndAbort( "The abort message !!!" );

    MainActivity.
qRFR
qRFT
qRFE( ErrFinal_( Env ) )
}

extern "C"
void
Java_info_q37_xdhdvkq_MainActivity_00024Callback_onReceiveValue(
        JNIEnv* Env,
        jobject Callback,
        jstring Value )
{
qRFH
qRFB
    const char *toto = Env->GetStringUTFChars( Value, NULL );

    printf( "%s\n", toto );
qRFR
qRFT
qRFE( ErrFinal_( Env ) )
}

namespace {
    jobject GetMainActivity_(
        JNIEnv *Env,
        jobject Callback )
    {
        jclass CallbackClass = Env->GetObjectClass(( Callback ));

        jfieldID MainActivityID = Env->GetFieldID( CallbackClass, "mainActivity_", "Linfo/q37/xdhdvkq/MainActivity;" );

        jobject MainActivity = Env->GetObjectField( Callback, MainActivityID );

        return MainActivity;
    }

#define T( variable )\
        if ( variable == NULL )\
            qRGnr()

    void ExecuteJavascript_(
            JNIEnv *Env,
            jobject MainActivity,
            jstring Script )
    {
# if 1
        jclass MainActivityClass = Env->GetObjectClass( MainActivity );
# else
        jclass MainActivityClass = env->FindClass( "info/q37/xdhdvkq/MainActivity" );
#endif
        jfieldID CallbackID = Env->GetFieldID( MainActivityClass, "callback", "Linfo/q37/xdhdvkq/MainActivity$Callback;" );

        T( CallbackID );

        jobject Callback = Env->GetObjectField( MainActivity, CallbackID );

        T( Callback );

        jfieldID WebViewID = Env->GetFieldID( MainActivityClass, "webView", "Landroid/webkit/WebView;" );

        T( WebViewID );

        jobject WebView = Env->GetObjectField( MainActivity, WebViewID );

        T( WebView );

        jclass WebViewClass = Env->GetObjectClass( WebView );

        T( WebViewClass );

        jmethodID EvaluateJavascriptID = Env->GetMethodID( WebViewClass, "evaluateJavascript", "(Ljava/lang/String;Landroid/webkit/ValueCallback;)V");

        T( EvaluateJavascriptID );

        Env->CallNonvirtualVoidMethod( WebView, WebViewClass, EvaluateJavascriptID, Script, Callback);
    }
}

extern "C"
void
Java_info_q37_xdhdvkq_MainActivity_launchEvent(
        JNIEnv* Env,
        jobject MainActivity,
        jstring Digest )
{
qRFH
qRFB
    const char *toto = Env->GetStringUTFChars( Digest, NULL );

    ExecuteJavascript_( Env, MainActivity, Digest );

    Env->ReleaseStringUTFChars( Digest, toto );
qRFR
qRFT
qRFE( ErrFinal_( Env ) )
}

Q37_GCTOR( xdhdvkq )
{
    qRRor_.Init();
    SCLError_.Init();

    Locale_.Init();
}


const char *sclmisc::SCLMISCTargetName = "xdhdvkq";
const char *sclmisc::SCLMISCProductName = "xdhdvkq";
const char *sclmisc::SCLMISCOrganizationName = "q37.info";

