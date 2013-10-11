/*
	'ui_ssn_vew.cpp' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_ssn_vew.cpp,v 1.1 2012/12/04 15:28:45 csimon Exp $

#include "ui_ssn_vew.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_ssn_vew;
using namespace ui_base;

using nsxpcm::event__;

using trunk::trunk___;

#define DIGEST_TARGET	UI_SSN_VEW_AFFIX

const char*ui_ssn_vew::refresh_callback__::XULFBSRefresh( xml::writer_ &Digest )
{
	ui::PushDigestRootTag( DIGEST_TARGET, Digest );
	Trunk().UI().SessionView.GetDigest( Digest );
	Digest.PopTag();

	return DIGEST_TARGET;
}

xulftk::trunk___ &ui_ssn_vew::session_view__::XULFSVTrunk( void ) const
{
	return Trunk();
}


void ui_ssn_vew::session_view__::Init( trunk::trunk___ &Trunk )
{
	_session_view__::Init( Trunk );
	_Callback.Init( Trunk );
	_page__::Init( _Callback, Trunk );
}


#define R( name ) Widgets.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
}

void ui_ssn_vew::session_view__::Register( nsIDOMWindow *Window )
{
	_page__::Attach( Window );

	_session_view__::Register( Window );

	Register_( Trunk(), Trunk().UI().SessionView.Widgets, Window );
}
