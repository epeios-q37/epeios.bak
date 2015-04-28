/*
	'dvkfev' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'dvkfev' header file ('dvkfev.h').
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

#define DVKFEV__COMPILATION

#include "dvkfev.h"

#include "jvabse.h"

using namespace dvkfev;

void dvkfev::Install(
	on_click_event_handler__ &Handler,
	jobject View,
	jobject Activity,
	JNIEnv *Env )
{
	jclass ListenerClass = Env->FindClass( "org/zeusw/dalvik/EpeiosOnClickListener" );

	if ( ListenerClass == NULL )
		ERRFwk();

	jobject Listener = Env->NewObject(
			ListenerClass,
			jvabse::GetMethodID( Env, ListenerClass, "<init>", "(Landroid/app/Activity;J)V" ),
			Activity,
			(jlong)&Handler );
			
	Env->CallVoidMethod(
		View,
		jvabse::GetMethodID( Env, View, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V" ),
		Listener );
}

void dvkfev::Install(
	on_item_selected_event_handler__ &Handler,
	jobject View,
	jobject Activity,
	JNIEnv *Env )
{
	jclass ListenerClass = Env->FindClass( "org/zeusw/dalvik/EpeiosOnItemSelectedListener" );

	if ( ListenerClass == NULL )
		ERRFwk();

	jobject Listener = Env->NewObject(
			ListenerClass,
			jvabse::GetMethodID( Env, ListenerClass, "<init>", "(Landroid/app/Activity;J)V" ),
			Activity,
			(jlong)&Handler );
			
	Env->CallVoidMethod(
		View,
		jvabse::GetMethodID( Env, View, "setOnItemSelectedListener", "(Landroid/widget/AdapterView$OnItemSelectedListener;)V" ),
		Listener );
}
