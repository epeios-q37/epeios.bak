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
import info.q37.xdhq.XDHq;

class Blank extends Atlas {
	public void handle( DOM dom, String action, String id ) {
		if ( action.equals( "Connect" ) ) {
			dom.setLayout("", new Tree(), "Main.xsl");
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "Submit" ) ) {
			dom.setContent("Pattern", dom.getContent("Pattern").toUpperCase());
		} else if ( action.equals( "HideInput" ) ) {
			dom.addClass("Input", "hidden");
		} else if ( action.equals( "ShowInput" ) ) {
			dom.removeClass("Input", "hidden");
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}

	public void test() {
		System.out.println("Essai from void method!");
	}

	private static void displayBytecodeBuildTimestamp() throws Exception {
		System.out.println("Bytecode build : " + new java.util.Date(new java.io.File(Blank.class.getClassLoader()
				.getResource(Blank.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()));
	}

	public static void main(String[] args) throws Exception {
		System.out.println(XDHq.wrapperInfo());
		System.out.println(XDHq.componentInfo());
		displayBytecodeBuildTimestamp();
		System.out.println(XDHq.returnArgument("Text from JAVA file"));

		launch("Connect", "blank", Atlas.Type.DEFAULT, args );

		for (;;) new Blank();
	}
}