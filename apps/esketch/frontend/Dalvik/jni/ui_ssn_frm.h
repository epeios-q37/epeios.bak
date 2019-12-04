/*
	'ui_ssn_frm.h' by Claude SIMON (http://q37.info/contact/).

	This file is part of 'eSketch' software.

    'Ssketch' is free software: you can redistribute it and/or modify it
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

#ifndef UI_SSN_FRM_INC_
# define UI_SSN_FRM_INC_

# include "ui_base.h"

namespace ui_ssn_frm {

	typedef  ui_base::on_item_selected_event_handler__ _on_item_selected_event_handler__;

	using ui_base::layout__;

	class backend_type_event_handler__
	: public _on_item_selected_event_handler__
	{
	protected:
		virtual void DVKEVTOnItemSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View,
			bso::uint__ Id );
		virtual void DVKEVTOnNothingSelected(
			JNIEnv * Env,
			jobject Activity,
			jobject View );
	};

	struct session_form__
	: public layout__
	{
	public:
		struct widget__ {
			ui_base::spinner__ BackendType;
			ui_base::spinner__ PredefinedBackend;
			ui_base::edit_text__ DaemonBackendLocation;
		} Widget;
		struct event_handler__ {
			backend_type_event_handler__ BackendType;
		} EventHandler;
		void reset( bso::bool__ P = true )
		{
			layout__::reset( P );

			Widget.BackendType.reset( P );
			EventHandler.BackendType.reset( P );
		}
		E_CDTOR( session_form__ )
		void Init(
			trunk::trunk___ &Trunk,
			JNIEnv *Env )
		{
			layout__::Init( "session_form", Trunk, Env );

			Widget.BackendType.Init( "SessionForm_BackendType" ,Trunk, Env );
			Widget.PredefinedBackend.Init( "SessionForm_PredefinedBackend", Trunk, Env );
			Widget.DaemonBackendLocation.Init( "SessionForm_DaemonBackendLocation", Trunk, Env );

			EventHandler.BackendType.Init( Trunk );
		}
		void Show(
			JNIEnv *Env,
			jobject Activity );
	};


}

#endif