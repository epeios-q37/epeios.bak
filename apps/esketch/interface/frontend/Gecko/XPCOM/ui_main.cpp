/*
	'ui_main.cpp' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui_main.cpp,v 1.2 2013/06/18 09:36:03 csimon Exp $

#include "ui_main.h"

#include "ui.h"
#include "sktinf.h"
#include "trunk.h"

using namespace ui_main;
using namespace ui_base;
using trunk::trunk___;

using nsxpcm::event__;

#define DIGEST_TARGET	UI_MAIN_AFFIX

const char *ui_main::main__::XULFBSRefresh( xml::writer_ &Digest )
{
	_main__::Refresh( Digest );

	return DIGEST_TARGET;
}

void ui_main::main__::Init( trunk::trunk___ &Trunk )
{
	_main__::Init( Trunk );
}


void ui_main::Attach(
	trunk::trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	main__ &Target = Trunk.UI().Main;

	Target.Attach( Window );
}
