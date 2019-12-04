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

// $Id: ui.h,v 1.1 2012/12/04 15:28:45 csimon Exp $

#ifndef UI_INC_
# define UI_INC_

# include "xulfui.h"

# include "eh.h"
# include "ui_base.h"
# include "ui_main.h"
# include "ui_ssn_frm.h"
# include "ui_ssn_vew.h"


#define UI_USERDATA_ATTRIBUTE_NAME	"UserData"

namespace ui {
	using namespace ui_base;

	namespace {
		typedef xulfui::ui___ _ui___;
	}

	class ui___
	: public _ui___
	{
	private:
		trunk::trunk___ *_Trunk;
		display__ _Display;
		void _SetDisplay( display__ Display );
		void _Refresh( void );
	protected:
		virtual xulftk::trunk___ &XULFUITrunk( void ) const;
		void XULFUIRefresh( void )
		{
			_Refresh();
		}
	public:
		eh::event_handlers__ EventHandlers;
		ui_main::main__ Main;
		ui_ssn_frm::session_form__ SessionForm;
		ui_ssn_vew::session_view__ SessionView;
		void reset( bso::bool__ P = true )
		{
			EventHandlers.reset( P );
			Main.reset( P );
			SessionForm.reset( P );
			SessionView.reset( P );

			_Trunk = NULL;
			_Display = d_Undefined;
			_ui___::reset( P );
		}
		E_DTOR( ui___ );
		ui___( void )
		: _ui___( EventHandlers )
		{
			reset( false );
		}

		void Init( trunk::trunk___ &Trunk )
		{
			_Trunk = &Trunk;

			EventHandlers.Init( Trunk );
			Main.Init( Trunk );
			SessionForm.Init( Trunk );
			SessionView.Init( Trunk );
			_ui___::Init( Main, SessionForm, SessionView );
		}
		void Show( display__ Display )
		{
			_SetDisplay( Display );
			_Refresh();
		}
	};
}

#endif