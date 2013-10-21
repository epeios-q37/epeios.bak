/*
	'ui_ssn_frm.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui_ssn_frm.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_ssn_frm;

void ui_ssn_frm::backend_type_event_handler__::DVKEVTOnItemSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View,
	bso::uint__ Id )
{
	bso::integer_buffer__ Buffer;

	LogD( bso::Convert( Id, Buffer ) );

	switch ( Id ) {
	case 0:
		dalvik::Hide( Trunk().UI.SessionForm.Widget.PredefinedBackend.Token(), Env, Activity );
		dalvik::Hide( Trunk().UI.SessionForm.Widget.DaemonBackendLocation.Token(), Env, Activity );
		break;
	case 1:
		dalvik::Show( Trunk().UI.SessionForm.Widget.PredefinedBackend.Token(), Env, Activity );
		dalvik::Hide( Trunk().UI.SessionForm.Widget.DaemonBackendLocation.Token(), Env, Activity );
		break;
	case 2:
		dalvik::Hide( Trunk().UI.SessionForm.Widget.PredefinedBackend.Token(), Env, Activity );
		dalvik::Show( Trunk().UI.SessionForm.Widget.DaemonBackendLocation.Token(), Env, Activity );
		break;
	case 3:
		dalvik::Hide( Trunk().UI.SessionForm.Widget.PredefinedBackend.Token(), Env, Activity );
		dalvik::Hide( Trunk().UI.SessionForm.Widget.DaemonBackendLocation.Token(), Env, Activity );
		break;
	default:
		ERRFwk();
		break;
	}
}

void ui_ssn_frm::backend_type_event_handler__::DVKEVTOnNothingSelected(
	JNIEnv * Env,
	jobject Activity,
	jobject View )
{
	LogD( "Nothing selected !" );
}


void ui_ssn_frm::session_form__::Show(
	JNIEnv *Env,
	jobject Activity )
{
	LOC
	dvkbse::SetContentView( Token(), Activity, Env );
	LOC
	dvkfev::Install( EventHandler.BackendType, Widget.BackendType.Token(), Activity, Env );
	LOC
}
