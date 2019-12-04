/*
	'ui_base.h' by Claude SIMON (http://q37.info/contact/).

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

#ifndef UI_BASE_INC_
# define UI_BASE_INC_

# include "tol.h"

# include "dvkbse.h"
# include "dvkfev.h"

# include "dalvik.h"

DVKBSE_EXPOSE_LOG_FUNCTIONS( "eSketch" )

#define LOC	DVKBSE_LOC

// #include "xulfbs.h"

// Predeclarations
namespace trunk {
	class trunk___;
};

namespace ui_base {

	enum panel__ {
		pHome,
		pSessionForm,
		pSessionView,
		p_amount,
		p_Undefined
	};

	template <typename trunk> class trunk_deposit__
	{
	private:
		trunk *_Trunk;
	public:
		void reset( bso::bool__ = true )
		{
			_Trunk = NULL;
		}
		E_CDTOR( trunk_deposit__ );
		void Init( trunk &Trunk )
		{
			_Trunk = &Trunk;
		}
		trunk &Trunk( void ) const
		{
			if ( _Trunk == NULL )
				qRGnr();

			return *_Trunk;
		}
	};

	template <typename trunk> class core__
	: public trunk_deposit__<trunk>
	{
	private:
		jint _Token;
	public:
		void reset( bso::bool__ P = true )
		{
			trunk_deposit__<trunk>::reset( P );
			_Token = 0;
		}
		E_CDTOR( core__ );
		void Init(
			jint Token,
			trunk &Trunk )
		{
			trunk_deposit__<trunk>::Init( Trunk );
			_Token = Token;
		}
		E_RODISCLOSE__( jint, Token );
	};

	template <typename trunk> class _layout__
	: public core__<trunk>
	{
	public:
		void Init(
			const char *Name,
			trunk &Trunk,
			JNIEnv *Env )
		{
			core__<trunk>::Init( dalvik::GetRlayoutToken( Name, Env ), Trunk );
		}
	};

	template <typename trunk> class widget__
	: public core__<trunk>
	{
	public:
		void Init(
			const char *Name,
			trunk &Trunk,
			JNIEnv *Env )
		{
			core__<trunk>::Init( dalvik::GetRidToken( Name, Env ), Trunk );
		}
	};

	template <typename trunk, typename event_handler> class event_handler__
	: public trunk_deposit__<trunk>,
	  public event_handler
	{
	public:
		void reset( bso::bool__ P = true )
		{
			trunk_deposit__<trunk>::reset( P );
			event_handler::reset( P );
		}
		E_CVDTOR( event_handler__ );
		void Init( trunk &Trunk )
		{
			trunk_deposit__<trunk>::Init( Trunk );
			event_handler::Init();
		}
	};

# define UI__BASE_W( name )\
	typedef widget__<trunk::trunk___> name

	UI__BASE_W( text_view__ );
	UI__BASE_W( button__ );
	UI__BASE_W( spinner__ );
	UI__BASE_W( edit_text__ );

	typedef _layout__<trunk::trunk___> layout__;



# define UI_BASE__EH( eh )\
	template <typename trunk> E_TTCLONE__( E_COVER2( ui_base::event_handler__<trunk, dvkfev::eh> ), _##eh )

	UI_BASE__EH( on_click_event_handler__ );
	UI_BASE__EH( on_item_selected_event_handler__ );

	typedef ui_base::_on_click_event_handler__<trunk::trunk___> on_click_event_handler__;
	typedef ui_base::_on_item_selected_event_handler__<trunk::trunk___> on_item_selected_event_handler__;
}

#endif