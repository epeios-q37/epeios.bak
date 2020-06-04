/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.atlas.*;

class Blank extends Atlas {
	static private String readAsset_( String path )  {
		return readAsset( path, "Blank" );
	}

	@Override
	public void handle(String action, String id ) {
		if ( action.equals( "" ) ) {
			dom.setLayout("",readAsset_( "Main.html") );
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "Submit" ) ) {
			dom.setContent("Pattern", dom.getContent("Pattern").toUpperCase());
		} else if ( action.equals( "HideInput" ) ) {
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "ShowInput" ) ) {
			dom.removeClass("Input", "hidden");
			dom.focus("Pattern");
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}

	public static void main(String[] args) {
		String dir;

		launch(() -> new Blank(), readAsset_("Head.html"), "Blank", GUI.DEFAULT, args);
	}
}
