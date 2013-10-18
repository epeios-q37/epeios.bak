#include "jvabse.h"

#include "str.h"
#include "tol.h"

#include "scldalvik.h"
#include "dvkbse.h"
#include "dvkfev.h"
#include "sclrgstry.h"

#include "sktinf.h"

#include "trunk.h"

const char *dalvik::PackageName="org/zeusw/" SKTINF_LC_AFFIX;

using namespace jvabse;

// DVKBSE_EXPOSE_LOG_FUNCTIONS( "eSketch" )

// #define LOC	DVKBSE_LOC


void WriteInWidget_( 
	JNIEnv * Env,
	jobject Activity )
{
	jint Id = GetStaticIntField(  Env, dvkbse::GetRidClass( dalvik::PackageName, Env ), "message" );

	jobject Text = Env->CallNonvirtualObjectMethod(
		Activity,
		GetClass( Env, Activity ),
		jvabse::GetMethodID( Env, Activity, "findViewById", "(I)Landroid/view/View;" ),
		Id );

	if ( Text == NULL )
		ERRFwk();

	jvabse::GetMethodID( Env, Text, "setText", "(Ljava/lang/CharSequence;)V" );

	STR_BUFFER___ Buffer;
	str::string Value;

	Value.Init();

	if( !sclrgstry::GetRegistry().GetValue( rgstry::entry___( "Test" ), sclrgstry::GetRoot(), Value ) )
		ERRFwk();


	Env->CallVoidMethod(
		Text,
		jvabse::GetMethodID( Env, Text, "setText", "(Ljava/lang/CharSequence;)V" ),
		Env->NewStringUTF( Value.Convert( Buffer ) ) );
}

typedef dalvik::steering_callback___ _steering_callback___;

class steering_callback___
: public _steering_callback___
{
private:
	/*
	switch_event_handler___ _Switch;
	hide_event_handler___ _Hide;
	*/
	trunk::trunk___ _Trunk;
protected:
	void DALVIKOnCreate(
		JNIEnv *Env,
		jobject Activity,
		jobject Bundle )
	{
/*
		dalvik::InstallEventHandler( _Switch, "Main_NewProject", Activity, Env );
		dalvik::InstallEventHandler( _Hide, "Main_OpenProject", Activity, Env );
*/

		_Trunk.Init( Env, Activity );
	}
public:
	void reset( bso::bool__ P = true )
	{
		_steering_callback___::reset( P );
/*		_Switch.reset( P );
		_Hide.reset( P );
*/
		_Trunk.reset( P );
	}
	E_CDTOR( steering_callback___ )
	void Init( void )
	{
		_steering_callback___::Init();
/*		_Switch.Init();
		_Hide.Init();
		_Spinner.Init();
*/
	}
};

dalvik::steering_callback___ *scldalvik::CreateSteering( void )
{

	steering_callback___ *Steering = new steering_callback___;

	if ( Steering == NULL )
		ERRAlc();

	Steering->Init();

	return Steering;
}
