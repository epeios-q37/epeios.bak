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

#include "tol.h"

extern "C"
void
Java_info_q37_xdhdvkq_MainActivity_00024Callback_onReceiveValue(
        JNIEnv* env,
        jobject Callback,
        jstring Value )
{
qRH
qRB
    const char *toto = env->GetStringUTFChars( Value, NULL );

    printf( "%s\n", toto );
qRR
qRT
qRE
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

        jobject Callback = Env->GetObjectField( MainActivity, CallbackID );

        jfieldID WebViewID = Env->GetFieldID( MainActivityClass, "webView", "Landroid/webkit/WebView;" );

        jobject WebView = Env->GetObjectField( MainActivity, WebViewID );

        jclass WebViewClass = Env->GetObjectClass( WebView );

        jmethodID EvaluateJavascriptID = Env->GetMethodID( WebViewClass, "evaluateJavascript", "(Ljava/lang/String;Landroid/webkit/ValueCallback;)V");

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
    const char *toto = Env->GetStringUTFChars( Digest, NULL );

    ExecuteJavascript_( Env, MainActivity, Digest );

    Env->ReleaseStringUTFChars( Digest, toto );
}

