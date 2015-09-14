/*
	'ui_main.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui_main.h"

#include "dalvik.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_main;

void ui_main::project_type_event_handler__::DVKEVTOnItemSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View,
	bso::uint__ Id )
{
	switch ( Id ) {
	case 0:
		dalvik::Hide( Trunk().UI.Main.Widgets.PredefinedProjectList.Token(), Env, Activity );
		break;
	case 1:
		dalvik::Show( Trunk().UI.Main.Widgets.PredefinedProjectList.Token(), Env, Activity );
		break;
	default:
		qRGnr();
		break;
	}

}

void ui_main::project_type_event_handler__::DVKEVTOnNothingSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View )
{
}

void ui_main::predefined_project_list_event_handler__::DVKEVTOnItemSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View,
	bso::uint__ Id )
{
qRH
	str::string Log;
	bso::integer_buffer__ Buffer;
qRH
	Log.Init( "PredefinedProjectList : ");
	Log.Append( bso::Convert( Id, Buffer ) );

	LogD( Log );
qRR
qRT
qRE
}

void ui_main::predefined_project_list_event_handler__::DVKEVTOnNothingSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View )
{
}

void ui_main::apply_event_handler__::DVKEVTOnClick(
		JNIEnv * Env,
		jobject Activity,
		jobject View )
{
	Trunk().UI.Show( ui_base::pSessionForm, Env, Activity );
}

static void PopulatePredefinedProjectList_(
	JNIEnv *Env,
	jobject Activity )
{
	LOC
	jobject Spinner = dalvik::GetView( "Main_PredefinedProjectList", Activity, Env );
	LOC
	jclass ArrayListClass = Env->FindClass( "java/util/ArrayList" );
	LOC
	jobject ArrayList = Env->NewObject( ArrayListClass, jvabse::GetMethodID( Env, ArrayListClass, "<init>", "()V" ) );
	LOC
	Env->CallBooleanMethod( ArrayList, jvabse::GetMethodID( Env, ArrayList, "add", "(Ljava/lang/Object;)Z" ), Env->NewStringUTF( "toto" ) );
	LOC
	jclass ArrayAdapterClass = Env->FindClass( "android/widget/ArrayAdapter" );
	LOC
	jobject ArrayAdapter = Env->NewObject( ArrayAdapterClass,
										   jvabse::GetMethodID( Env,  ArrayAdapterClass, "<init>", "(Landroid/content/Context;ILjava/util/List;)V" ),
										   Activity, dvkbse::GetRlayoutToken( "simple_spinner_item", "android", Env ),
										   ArrayList );
	LOC
	Env->CallVoidMethod( ArrayAdapter, jvabse::GetMethodID( Env, ArrayAdapter, "setDropDownViewResource", "(I)V" ), dvkbse::GetRlayoutToken( "simple_spinner_dropdown_item", "android", Env ) );
	LOC
	Env->CallVoidMethod( Spinner, jvabse::GetMethodID( Env, Spinner, "setAdapter", "(Landroid/widget/Adapter;)V" ), ArrayAdapter );
	LOC
	Env->CallBooleanMethod( ArrayList, jvabse::GetMethodID( Env, ArrayList, "add", "(Ljava/lang/Object;)Z" ), Env->NewStringUTF( "titi" ) );
	LOC
}

void ui_main::main__::Show(
	JNIEnv *Env,
	jobject Activity )
{
	dvkbse::SetContentView( Token(), Activity, Env );

	dvkfev::Install( EventHandlers.ProjectType, Widgets.ProjectType.Token(), Activity, Env );
	dvkfev::Install( EventHandlers.PredefinedProjectList, Widgets.PredefinedProjectList.Token(), Activity, Env );
	dvkfev::Install( EventHandlers.Apply, dalvik::GetView( "Main_Apply", Activity, Env ), Activity, Env );

	PopulatePredefinedProjectList_( Env, Activity );
}
