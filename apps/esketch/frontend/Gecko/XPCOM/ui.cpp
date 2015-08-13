/*
	'ui.cpp' by Claude SIMON (http://zeusw.org/).

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

// $Id: ui.cpp,v 1.4 2013/04/09 17:57:21 csimon Exp $

#include "ui.h"
#include "trunk.h"

using namespace ui;

xulftk::trunk___ &ui::ui___::XULFUITrunk( void ) const
{
	if ( _Trunk == NULL )
		qRGnr();

	return *_Trunk;
}

static void _ShowPanel(
	display__ Display,
	ui___ &UI )
{
	nsIDOMNode *MainPanel = NULL;

	switch ( Display ) {
	case dHome:
		MainPanel = UI.Main.vewHome();
		break;
	case dSessionForm:
		MainPanel = UI.Main.vewSessionForm();
		break;
	case dSessionView:
		MainPanel = UI.Main.vewSessionView();
		break;
	default:
		ERRPrm();
		break;
	}

	UI.Main.dckMain().SetSelectedItem( MainPanel );

}

void ui::ui___::_SetDisplay( display__ Display )
{
	_Display = Display;
}

void ui::ui___::_Refresh( void )
{
	Main.Refresh();

	switch( _Display ) {
	case dHome:
		break;
	case dSessionForm:
		SessionForm.Refresh();
		break;
	case dSessionView:
		SessionView.Refresh();
		break;
	default:
		qRGnr();
		break;
	}

	_ShowPanel( _Display, *this );
}


