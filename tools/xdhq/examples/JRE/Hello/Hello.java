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
	public void handle( DOM dom, String action, String id )
	{
		String html = 
		"<div class=\"vcenter-out\">" +
		" <div class=\"vcenter-in\">" +
		"  <fieldset>" +
		"   <label>Name:</label>" +
		"   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
		"	 type=\"text\" data-xdh-onevent=\"input|Typing\"/>" +
		"   <button data-xdh-onevent=\"Clear\">Clear</button>" +
		"   <hr/>" +
		"   <h1>" +
		"    <span>Hello </span>" +
		"    <span style=\"font-style: italic;\" id=\"name\"></span>" +
		"    <span>!</span>" +
		"   </h1>" +
		"  </fieldset>" +
		" </div>" +
		"</div>";

		if ( "Connect".equals( action ) ) {
			dom.setLayout("", html);
			dom.focus( "input");
		} else if ("Typing".equals( action ) ) {
			dom.setContent("name", dom.getContent(id));
		} else if ( "Clear".equals( action ) ) {
			if ( dom.confirm( "Are you sure ?" ) )
				dom.setContents( new HashMap<String,String> ()
					{{ put( "input", ""); put( "name", ""); }} );
		} else {
			throw new RuntimeException( "Unknown action '" + action + "' !!!");
		}
	}
	public static void main(String[] args) throws Exception {
		String head = 
			"<title>\"Hello, World !\" example</title>" +
			"<style type=\"text/css\">" +
			" html, body { height: 100%; padding: 0; margin: 0; }" +
			" .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }" +
			" .vcenter-in { display: table-cell; vertical-align: middle; }" +
			"</style>";

		launch("Connect", head, "Hello", GUI.DEFAULT, args );

		for (;;)
			new Hello();
	}
}