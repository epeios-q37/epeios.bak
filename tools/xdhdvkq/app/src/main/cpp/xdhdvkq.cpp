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
#include <android/log.h>
#include <android/asset_manager_jni.h>

#include "tol.h"
#include "sclerror.h"
#include "sclmisc.h"

namespace {
    qCDEF( char *, NameLC_, "xdhdvkq" );
    qCDEF( char *, NameMC_, NameLC_ );
    qCDEF( char *, NameUC_, "XDHDVKQ" );
}

namespace log_ {
    inline  void Log(
            android_LogPriority Priority,
            const char *Tag,
            const char *Text )
    {
        if ( ( Text != NULL ) && ( *Text ) )
            __android_log_write( Priority, Tag, Text );
    }

    inline  void Log(
            android_LogPriority Priority,
            const char *Tag,
            const str::string_ &Text )
    {
    qRH
        TOL_CBUFFER___ Buffer;
    qRB
        Log( Priority, Tag, Text.Convert( Buffer ) );
    qRR
    qRT
    qRE
    }
}

namespace {
#define L(Type, Priority)\
    inline void Log##Type( const char *Text )\
    {\
        log_::Log( Priority, NameUC_, Text );\
    }\
    inline void Log##Type( const str::string_ &Text )\
    {\
        log_::Log( Priority, NameUC_, Text );\
    }

    L(I, ANDROID_LOG_INFO)\
    L(D, ANDROID_LOG_DEBUG)\
    L(W, ANDROID_LOG_WARN)\
    L(E, ANDROID_LOG_ERROR)\
    L(F, ANDROID_LOG_FATAL)\

#undef L
}

# define LOC	LogD( __LOC__ )

template<typename t> t T_( t T )
{
    if ( T == NULL )
        qRGnr();

    return T;
}

namespace asset_ {
    qCDEF( fdr::sSize, BufferSize_, 1000 );

    class sCommon {
    public:
        qPMV( JNIEnv, E, Env_ );
        qPMV( AAsset, A, Asset_  );
        void reset( bso::sBool P = true )
        {
            if ( P )
                if ( Asset_ != NULL )
                    AAsset_close( Asset_ );

            tol::reset( P, Env_, Asset_ );
        }
        qCDTOR( sCommon );
        void Init(
            JNIEnv *Env,
            AAsset *Asset )
        {
            Env_ = Env;
            Asset_ = Asset;
        }
    };

    typedef fdr::rIFlow rIFlowDriver_;

    class rIFlowDriver
    : public sCommon,
      public rIFlowDriver_
    {
    private:
        fdr::sSize JRead_(
            fdr::sSize Maximum,
            fdr::sByte *Buffer )
        {
            return AAsset_read( A(), Buffer, Maximum );
        }
    protected:
        virtual fdr::sSize FDRRead(
            fdr::sSize Maximum,
            fdr::sByte *Buffer ) override
        {
            return JRead_( Maximum, Buffer );
        }
        virtual void FDRDismiss( void ) override
        {
            // Nothing to do.
        }
        public:
        void reset( bso::bool__ P = true )
        {
            sCommon::reset( P );
            rIFlowDriver_::reset( P );
        }
        E_CVDTOR( rIFlowDriver )
        void Init(
            JNIEnv *Env,
            AAsset *Asset )
        {
            rIFlowDriver_::Init( fdr::ts_Default );

            sCommon::Init( Env, Asset );
        }
    };

    typedef flw::sDressedIFlow<> sIFlow_;

    class rIFlow
    : public sIFlow_
    {
    private:
        rIFlowDriver Driver_;
    public:
        void reset( bso::sBool P = true )
        {
            sIFlow_::reset( P );
            tol::reset( P, Driver_ );
        }
        qCDTOR( rIFlow );
        void Init(
                JNIEnv *Env,
                AAsset *Asset )
        {
            Driver_.Init( Env, Asset );
            sIFlow_::Init( Driver_ );
        }
    };
}

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
    jobject MainActivity,
    jobject assetManager )
{
qRFH
    sclmisc::sRack Rack;
    asset_::rIFlow LFlow, CFlow;
    xtf::sIFlow LXFlow, CXFlow;
    AAssetManager *AssetManager = NULL;
   str::wString Language;
qRFB
    AssetManager = AAssetManager_fromJava( Env, assetManager );

    Rack.Init( qRRor_, SCLError_, cio::GetSet( cio::t_Default ), Locale_);

    LFlow.Init( Env, AAssetManager_open( AssetManager, "xdhdvkq.xlcl", AASSET_MODE_STREAMING ) );
    LXFlow.Init( LFlow, utf::f_Default );

    CFlow.Init( Env, AAssetManager_open( AssetManager, "xdhdvkq.xcfg", AASSET_MODE_STREAMING ) );
    CXFlow.Init( CFlow, utf::f_Default );

    sclmisc::Initialize( Rack, LXFlow, "", CXFlow, "", "" );
    LogD( "TEST");

    Language.Init();
    sclmisc::MGetValue(sclrgstry::parameter::Language, Language );
    LogD( cpe::GetDescription() );
//    sclmisc::ReportAndAbort( "The abort message !!!" );
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
        jfieldID CallbackID = T_( Env->GetFieldID( MainActivityClass, "callback", "Linfo/q37/xdhdvkq/MainActivity$Callback;" ) );

        jobject Callback = T_( Env->GetObjectField( MainActivity, CallbackID ) );

        jfieldID WebViewID = T_( Env->GetFieldID( MainActivityClass, "webView", "Landroid/webkit/WebView;" ) );

        jobject WebView = T_( Env->GetObjectField( MainActivity, WebViewID ) );

        jclass WebViewClass = T_( Env->GetObjectClass( WebView ) );

        jmethodID EvaluateJavascriptID = T_( Env->GetMethodID( WebViewClass, "evaluateJavascript", "(Ljava/lang/String;Landroid/webkit/ValueCallback;)V") );

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



#if 0   // To display the compile/link command line for the below architecture.
# ifdef CPE_A_IA32
#  if 0   // 0 : link command line, 1 : compilation command line.
#   error
#  endif
# else
const char *sclmisc::SCLMISCTargetName = NameLC_;
# endif
#else
const char *sclmisc::SCLMISCTargetName = NameLC_;
#endif
const char *sclmisc::SCLMISCProductName = NameLC_;
const char *sclmisc::SCLMISCOrganizationName = "q37.info";

