/*
	'ui_ssn_frm.cpp' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_ssn_frm.cpp,v 1.1 2012/12/04 15:28:45 csimon Exp $

#include "ui_ssn_frm.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_ssn_frm;
using namespace ui_base;
using trunk::trunk___;

# define DIGEST_TARGET	UI_SSN_FRM_AFFIX

const char *ui_ssn_frm::session_form__::XULFBSRefresh( xml::writer_ &Digest )
{
	_session_form__::Refresh( Digest );

	return DIGEST_TARGET;
}

static void Attach_(
	widgets__ &Widgets,
	nsIDOMWindow *Window )
{
}

void ui_ssn_frm::session_form__::Init( trunk::trunk___ &Trunk )
{
	_session_form__::Init( Trunk );
}

void ui_ssn_frm::session_form__::Attach( nsIDOMWindow *Window )
{
	_session_form__::Attach( Window );

	Attach_( Widgets, Window );
}
