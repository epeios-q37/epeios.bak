/*
	'scldalvik' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scldalvik' header file ('scldalvik.h').
	Copyright (C) 20122004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id: scldalvik.cpp,v 1.6 2012/12/04 15:28:48 csimon Exp $

#define SCLDALVIK__COMPILATION

#include "scldalvik.h"

class scldalviktutor
: public ttr_tutor
{
public:
	scldalviktutor( void )
	: ttr_tutor( SCLDALVIK_NAME )
	{
#ifdef SCLDALVIK_DBG
		Version = SCLDALVIK_VERSION "\b\bD $";
#else
		Version = SCLDALVIK_VERSION;
#endif
		Owner = SCLDALVIK_OWNER;
		Date = "$Date: 2012/12/04 15:28:48 $";
	}
	virtual ~scldalviktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/
#include "str.h"
#include "cio.h"

#include "jvastf.h"

#include "dvkbse.h"
#include "sclmisc.h"

#include "dvkbse.h"
#include "dvkfev.h"

using namespace scldalvik;

// Pour faciliter le débogage.
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
			ERRFwk();

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
ERRProlog
	jvastf::input_stream_iflow___ Configuration, Locale;
	jclass Raw;
	jvastf::jni_param__ LocaleParam, ConfigurationParam;
ERRBegin
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
ERRErr
ERREnd
ERREpilog
}

static dalvik::steering_callback___ &GetSteering_(
	JNIEnv *Env,
	jobject Activity )
{
	dalvik::steering_callback___ &Steering = *(dalvik::steering_callback___ *)jvabse::GetLongField( Env, Activity, "steering" );

	if( &Steering == NULL )
		ERRFwk();

	return Steering;
}

static void Main_( 
	JNIEnv *Env,
	jobject Activity,
	jobject Bundle )
{
	dalvik::steering_callback___ *Steering = CreateSteering();

	if ( Steering == NULL )
		ERRFwk();
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
		ERRFwk();

	callback &Callback = *(callback *)jvabse::GetLongField( Env, Listener, "callback" );

	if ( &Callback == NULL )
		ERRFwk();

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

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scldalvikpersonnalization
: public scldalviktutor
{
public:
	scldalvikpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scldalvikpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scldalvikpersonnalization Tutor;

ttr_tutor &SCLDALVIKTutor = Tutor;
