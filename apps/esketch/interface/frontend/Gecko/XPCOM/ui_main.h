/*
	'ui_main.h' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_main.h,v 1.3 2013/04/09 17:57:21 csimon Exp $

#ifndef UI_MAIN__INC
# define UI_MAIN__INC

# include "xulfmn.h"
# include "ui_base.h"

# define UI_MAIN_AFFIX	XULFMN_AFFIX

# define UI_MAIN_WINDOW_ID	"wdw" UI_MAIN_AFFIX

namespace ui_main {

	typedef xulfmn::widgets__ _widgets__;

	UI_DEF;

	class widgets__
	: public _widgets__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_widgets__::reset( P );
		}
		E_CDTOR( widgets__ );
		void Init( void )
		{
			_widgets__::Init();
		}
	};

	class refresh_callback__
	: public _refresh_callback__
	{
	protected:
		const char *XULFBSRefresh( xml::writer_ &Digest );
	};
	
	typedef xulfmn::main__ _main__;

	class main__
	: public _main__,
	  public window__
	{
	private:
		refresh_callback__ _Callback;
	protected:
		virtual xulftk::trunk___ &XULFMNTrunk( void ) const;
	public:
		nsIDOMElement *Broadcasters;
		widgets__ Widgets;
		void reset( bso::bool__ P = true )
		{
			_main__::reset( P );
			window__::reset( P );
			_Callback.reset( P );

			Widgets.reset( P );
		}
		main__( void )
		: _main__( Widgets )
		{
			reset( false );
		}
		E_DTOR( main__ )
		void Init( trunk::trunk___ &Trunk );
		void Register( nsIDOMWindow *Window );
	};
/*
	void RegisterMainUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
*/
}

#endif