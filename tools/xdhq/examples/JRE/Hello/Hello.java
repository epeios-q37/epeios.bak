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

/*
	This is the one-file version, which does not need the 'Head.html' and the 'Main.html' files.
*/

import info.q37.atlas.*;
import java.util.HashMap;

class Hello extends Atlas {
	private static String html = 
		"<div style=\"display: table; margin: 50px auto auto auto;\">" +
		" <fieldset>" +
		"   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
		"	         type=\"text\" data-xdh-onevent=\"Submit\"/>" +
		"   <div style=\"display: flex; justify-content: space-around; margin: 5px auto auto auto;\">" +
		"    <button data-xdh-onevent=\"Submit\">Submit</button>" +
		"    <button data-xdh-onevent=\"Clear\">Clear</button>" +
		"  </div>" +
		" </fieldset>" +
		"</div>";

	@Override
	public void handle(String action, String id )
	{
		switch( action) {
		case "":
			dom.setLayout("", html );
			break;
		case "Submit":
			dom.alert("Hello, " + dom.getContent("input").trim() + "!" );
			break;
		case "Clear":
			if ( dom.confirm( "Are you sure ?" ) )
				dom.setContent( "input", "");
			break;
		default:
			throw new RuntimeException( "Unknown action '" + action + "' !!!");
		}

		dom.focus( "input");
	}

	public static void main(String[] args) throws Exception {
		String head =
			"<title>\"Hello, World !\" example</title>" +
			"<link rel=\"icon\" type=\"image/png\" href=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==\" />";

		launch(() -> new Hello(), head, "Hello", GUI.DEFAULT, args );
	}
}