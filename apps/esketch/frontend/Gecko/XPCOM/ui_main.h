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

// $Id: ui_main.h,v 1.3 2013/04/09 17:57:21 csimon Exp $

#ifndef UI_MAIN_INC_
# define UI_MAIN_INC_

# include "xulfmn.h"
# include "ui_base.h"

# define UI_MAIN_AFFIX	XULFMN_AFFIX

# define UI_MAIN_WINDOW_ID	"wdw" UI_MAIN_AFFIX

namespace ui_main {

	namespace {
		typedef xulfmn::main__<trunk::trunk___> _main__;
	}

	class widgets__
	{
	public:
		void reset( bso::bool__ P = true )
		{
		}
		E_CDTOR( widgets__ );
		void Init( void )
		{
		}
		void Attach( nsIDOMWindow *Window );
	};

	class main__
	: public _main__
	{
	protected:
		virtual const char *XULFBSRefresh( xml::writer_ &Digest );
	public:
		widgets__ Widgets;
		void reset( bso::bool__ P = true )
		{
			_main__::reset( P );
			Widgets.reset( P );
		}
		E_CDTOR( main__ );
		void Init( trunk::trunk___ &Trunk )
		{
			Widgets.Init();
			_main__::Init( Trunk );		}
		void Attach( nsIDOMWindow *Window )
		{
			Widgets.Attach( Window );
			_main__::Attach( Window );
		}
		};


}

#endif