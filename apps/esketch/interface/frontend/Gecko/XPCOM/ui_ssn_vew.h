/*
	'ui_ssn_frm.h' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_ssn_vew.h,v 1.1 2012/12/04 15:28:45 csimon Exp $

#ifndef UI_SSN_VEW__INC
# define UI_SSN_VEW__INC

# include "xulfsv.h"
# include "ui_base.h"

# define UI_SSN_VEW_AFFIX	XULFSV_AFFIX

# define UI_SSN_VEW_PAGE_ID	"pge" UI_SSN_VEW_AFFIX

namespace ui_ssn_vew {
	typedef xulfsv::session_view__ _session_view__;

	UI_DEF;

	class refresh_callback__
	: public _refresh_callback__
	{
	protected:
		virtual const char *XULFBSRefresh( xml::writer_ &Digest );
	};

	struct session_view__
	: public _session_view__,
	  public _page__
	{
	private:
		refresh_callback__ _Callback;
	protected:
		virtual xulftk::trunk___ &XULFSVTrunk( void ) const;
	public:
		struct widgets__
		: public xulfsv::widgets__
		{} Widgets;
		void reset( bso::bool__ P = true )
		{
			_Callback.reset( P );
			_session_view__::reset(  P);
			_page__::reset( P );
		}
		E_VDTOR( session_view__ )
		session_view__( void )
		: _session_view__( Widgets )
		{}
		void Init( trunk::trunk___ &Trunk );
		void Register( nsIDOMWindow *Window );
	};
}

#endif