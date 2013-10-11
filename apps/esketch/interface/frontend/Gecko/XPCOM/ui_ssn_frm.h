/*
	'ui_ssn_frm.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'Ssketch' is free software: you can redistribute it and/or modify
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

// $Id: ui_ssn_frm.h,v 1.1 2012/12/04 15:28:45 csimon Exp $

#ifndef UI_SSN_FRM__INC
# define UI_SSN_FRM__INC

# include "xulfsf.h"
# include "ui_base.h"

# define UI_SSN_FRM_AFFIX	XULFSF_AFFIX

# define UI_SSN_FRM_PAGE_ID	"pge" UI_SSN_FRM_AFFIX

namespace ui_ssn_frm {

	typedef xulfsf::session_form__ _session_form__;

	UI_DEF;

	class refresh_callback__
	: public _refresh_callback__
	{
	protected:
		virtual const char*XULFBSRefresh( xml::writer_ &Digest );
	};

	struct session_form__
	: public _session_form__,
	  public _page__
	{
	private:
		refresh_callback__ _Callback;
	protected:
		xulftk::trunk___ &XULFSFTrunk( void ) const;
	public:
		struct broadcasters__
		: public xulfsf::broadcasters__
		{} Broadcasters;
		struct widgets__
		: public xulfsf::widgets__
		{} Widgets;
		void reset( bso::bool__ P = true )
		{
			_session_form__::reset( P );
			_page__::reset( P );
			_Callback.reset( P );
		}
		E_VDTOR( session_form__ )
		session_form__( void )
		: _session_form__( Broadcasters, Widgets )
		{}
		void Init( trunk::trunk___ &Trunk );
		void Attach( nsIDOMWindow *Window )
		{
			_session_form__::Attach( Window );
			_page__::Attach( Window );
		}
		void Register( nsIDOMWindow *Window );
	};

}

#endif