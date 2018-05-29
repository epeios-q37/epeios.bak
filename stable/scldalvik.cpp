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

#define SCLDALVIK__COMPILATION

#include "scldalvik.h"

#include "str.h"
#include "cio.h"

#include "jvastf.h"

#include "dvkbse.h"
#include "sclmisc.h"

#include "dvkbse.h"
#include "dvkfev.h"

using namespace scldalvik;

// Pour faciliter le dbogage.
#if 1
DVKBSE_EXPOSE_LOG_FUNCTIONS( "SCLDalvik" )
#define LOC	 DVKBSE_LOC	
#endif

STR_BUFFER___ TargetName_;

typedef fdr::oflow_driver___<> _oflow___;

class log_oflow_driver___
: public _oflow___
{
private:
	str::string _Buffer;
protected:
	virtual fdr::size__ FDRWrite(
		const fdr::datum__ *Buffer,
		fdr::size__ Maximum )
	{
		_Buffer.Append( (bso::char__ *)Buffer, Maximum );
		return Maximum;
	}
	virtual void FDRCommit( void )
	{
		if ( ::TargetName_ == NULL )
			qRFwk();

		dvkbse::Log( ANDROID_LOG_ERROR, ::TargetName_, _Buffer );
		_Buffer.Init();
	}
public:
	void reset( bso::bool__ P = true )
	{
		_oflow___::reset( P );
		_Buffer.reset( P );
	}
	E_CVDTOR( log_oflow_driver___ );
	void Init( void )
	{
		_oflow___::Init( fdr::ts_Default );
		_Buffer.Init();
	}
};

log_oflow_driver___ OFlowDriver;

static void InitializeCIO_( void )
{
	::OFlowDriver.Init();

	cio::COutDriver.Init( OFlowDriver, fdr::ts_Default );
	cio::CErrDriver.Init( OFlowDriver, fdr::ts_Default );
	cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

	cio::Initialize( cio::tUser );
}

/**/
static void DoDalvikRelatedStuff_(
		JNIEnv *Env,
		jobject Activity,
		jobject Bundle ) 
{
	Env->CallNonvirtualVoidMethod(
		Activity, Env->FindClass( "android/app/Activity" ),
		jvabse::GetMethodID( Env, Activity, "onCreate", "(Landroid/os/Bundle;)V" ),
		Bundle );
/*
	Env->CallVoidMethod(
		Activity,
		jvabse::GetMethodID( Env, Activity, "setContentView", "(I)V" ),
		jvabse::GetStaticIntField(  Env, dvkbse::GetRlayoutClass( dalvik::PackageName, Env ), "main" ) );
*/
}

static void InitializeFlow_(
	const char *Name,
	JNIEnv *Env,
	jobject Activity,
	jclass Raw,
	jvastf::jni_param__ &Param,
	jvastf::input_stream_iflow___ &IFlow )
{
	Param.Env = Env;
	Param.Object = dvkbse::GetRawResourceInputStream( Name, Raw, Env, Activity );

	IFlow.Init( Param );
}

static void Initialize_(
	const char *Target,
	JNIEnv *Env,
	jobject Activity )
{
qRH
	jvastf::input_stream_iflow___ Configuration, Locale;
	jclass Raw;
	jvastf::jni_param__ LocaleParam, ConfigurationParam;
qRB
	LOC
	InitializeCIO_();
	LOC
	Raw = dvkbse::GetRrawClass( dalvik::PackageName, Env );
	LOC
	InitializeFlow_( "locale", Env, Activity, Raw, LocaleParam, Locale );
	LOC
	InitializeFlow_( "configuration", Env, Activity, Raw, ConfigurationParam, Configuration );
	LOC
		sclmisc::Initialize( Locale, utf::f_Guess, Configuration, utf::f_Guess, Target, "", "" );
	LOC
qRR
qRT
qRE
}

static dalvik::steering_callback___ &GetSteering_(
	JNIEnv *Env,
	jobject Activity )
{
	dalvik::steering_callback___ &Steering = *(dalvik::steering_callback___ *)jvabse::GetLongField( Env, Activity, "steering" );

	if( &Steering == NULL )
		qRFwk();

	return Steering;
}

static void Main_( 
	JNIEnv *Env,
	jobject Activity,
	jobject Bundle )
{
	dalvik::steering_callback___ *Steering = CreateSteering();

	if ( Steering == NULL )
		qRFwk();
	jvabse::SetLongField( Env, Activity, "steering", (jlong)Steering );

	GetSteering_( Env, Activity ).OnCreate( Env, Activity, Bundle );
}

template <typename callback, typename method> static void HandleEvent_( 
	JNIEnv *Env,
	jobject Listener,
	jobject View,
	method Method,
	... )
{
	jobject Activity = jvabse::GetObjectField( Env, Listener, "activity", "Landroid/app/Activity;" );

	if ( Activity == NULL )
		qRFwk();

	callback &Callback = *(callback *)jvabse::GetLongField( Env, Listener, "callback" );

	if ( &Callback == NULL )
		qRFwk();

    va_list args;                                                       \
    va_start(args, Method );                                           \

	(Callback.*Method)( Env, Activity, View, args );
}

extern "C" {
	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosActivity_launch(
		JNIEnv *Env,
		jobject Activity,
		jstring TargetName,
		jobject Bundle ) 
	{
		LOC
		jvabse::Convert( TargetName, Env, ::TargetName_ );
		LOC
		DoDalvikRelatedStuff_( Env, Activity, Bundle );
		LOC
		Initialize_( ::TargetName_, Env, Activity );
		LOC
		Main_( Env, Activity, Bundle );
		LOC
	}

	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosOnClickListener_onClick(
		JNIEnv *Env,
		jobject Listener,
		jobject View)
	{
		HandleEvent_<dvkfev::on_click_event_handler__>( Env, Listener, View, &dvkfev::on_click_event_handler__::VAOnClick );
	}

	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosOnItemSelectedListener_onItemSelected(
		JNIEnv *Env,
		jobject Listener,
		jobject Parent,
		jobject View,
		jint Position,
		jlong Id )
	{
		HandleEvent_<dvkfev::on_item_selected_event_handler__>( Env, Listener, Parent, &dvkfev::on_item_selected_event_handler__::VAOnItemSelected, Id );
	}

	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosOnItemSelectedListener_onNothingSelected(
		JNIEnv *Env,
		jobject Listener,
		jobject Parent )
	{
		HandleEvent_<dvkfev::on_item_selected_event_handler__>( Env, Listener, Parent, &dvkfev::on_item_selected_event_handler__::VAOnNothingSelected );
	}
}

