/*
	'ui_base.h' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_base.h,v 1.1 2012/12/04 15:28:45 csimon Exp $

#ifndef UI_BASE__INC
# define UI_BASE__INC

# include "xulfbs.h"

// Predeclarations
namespace trunk {
	class trunk___;
};

# define UI_DEF\
	namespace {\
		using namespace ui_base;\
	}

namespace ui_base {

	enum display__ {
		dHome,
		dSessionForm,
		dSessionView,
		d_amount,
		d_Undefined
	};

	typedef xulwdg::event_handler__<trunk::trunk___> _event_handler__;
	/*
	typedef xulfbs::window__<trunk::trunk___> _window__;
	typedef xulfbs::page__<trunk::trunk___> _page__;
	*/
	
	class event_handler__
	: public _event_handler__
	{
	protected:
		virtual void NSXPCMOnErr( const char *Message );
	};

	XULFBS_ALMOST_ALL_WIDGETS( trunk::trunk___ )

# define UI_BASE_EH( name )\
	class name\
	: public ui_base::event_handler__\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	};
};

#endif