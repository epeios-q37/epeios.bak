/*
	'ui_main.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef UI_MAIN_INC_
# define UI_MAIN_INC_

# include "jni.h"
# include "dalvik.h"

// # include "xulfmn.h"
# include "ui_base.h"

namespace ui_main {

	typedef  ui_base::on_click_event_handler__ _on_click_event_handler__;
	typedef  ui_base::on_item_selected_event_handler__ _on_item_selected_event_handler__;

	using ui_base::layout__;

	class project_type_event_handler__
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

	class predefined_project_list_event_handler__
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


	class apply_event_handler__
	: public _on_click_event_handler__
	{
	protected:
		virtual void DVKEVTOnClick(
			JNIEnv * Env,
			jobject Activity,
			jobject View );
	};

	class event_handlers__
	{
	public:
		project_type_event_handler__ ProjectType;
		predefined_project_list_event_handler__ PredefinedProjectList;
		apply_event_handler__ Apply;
		void reset( bso::bool__ P = true )
		{
			ProjectType.reset( P );
			PredefinedProjectList.reset( P );
			Apply.reset( P );
		}
		E_CDTOR( event_handlers__ );
		void Init( trunk::trunk___ &Trunk )
		{
			ProjectType.Init( Trunk );
			PredefinedProjectList.Init( Trunk );
			Apply.Init( Trunk );
		}
	};

	class widgets__
	{
	public:
		ui_base::spinner__
			ProjectType,
			PredefinedProjectList;
		void reset( bso::bool__ P = true )
		{
			ProjectType.reset( P );
			PredefinedProjectList.reset( P );
		}
		E_CDTOR( widgets__ );
		void Init(
			trunk::trunk___ &Trunk,
			JNIEnv *Env )
		{
			ProjectType.Init( "Main_ProjectType", Trunk, Env );
			PredefinedProjectList.Init( "Main_PredefinedProjectList", Trunk, Env );
		}
	};

	class main__
	: public layout__
	{
	public:
		widgets__ Widgets;
		event_handlers__ EventHandlers;
		void reset( bso::bool__ P = true )
		{
			layout__::reset( P );

			Widgets.reset( P );
			EventHandlers.reset( P );
		}
		E_CDTOR( main__ );
		void Init(
			trunk::trunk___ &Trunk,
			JNIEnv *Env )
		{
			layout__::Init( "main", Trunk, Env );

			Widgets.Init( Trunk, Env );
			EventHandlers.Init( Trunk );

		}
		void Show(
			JNIEnv *Env,
			jobject Activity );
	};
}

#endif