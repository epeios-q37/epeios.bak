/*
	'ui.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef UI_INC_
# define UI_INC_

// # include "xulfui.h"

# include "ui_base.h"
# include "ui_main.h"
# include "ui_ssn_frm.h"
// # include "ui_ssn_vew.h"


namespace ui {
	using namespace ui_base;

	class ui__
	{
	private:
		trunk::trunk___ *_Trunk;
		void _SwitchTo(
			panel__ Panel,
			JNIEnv *Env,
			jobject Activity );
	public:
		ui_main::main__ Main;
		ui_ssn_frm::session_form__ SessionForm;
		void reset( bso::bool__ P = true )
		{
			_Trunk = NULL;

			Main.reset( P );
		}
		E_CDTOR( ui__ );
		void Init(
			trunk::trunk___ &Trunk,
			JNIEnv *Env )
		{
			_Trunk = &Trunk;

			Main.Init( Trunk, Env );
			SessionForm.Init( Trunk, Env );
		}
		void Show(
			panel__ Panel,
			JNIEnv *Env,
			jobject Activity )
		{
			_SwitchTo( Panel, Env, Activity );
		}
	};
}

#endif