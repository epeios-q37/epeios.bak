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

class Thread extends java.lang.Thread {
	private DOM dom;

	public Thread(DOM dom) {
		this.dom = dom;
	}

	public void run() {
		Event event = new Event();

		System.out.println("Connection detected...");

		for (;;) {
			String action = dom.getAction(event);

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
	}

	public void test() {
		System.out.println("Essai from void method!");
	}
}

class Blank {
	private static void displayBytecodeBuildTimestamp() throws Exception {
		System.out.println("Bytecode build : " + new java.util.Date(new java.io.File(Blank.class.getClassLoader()
				.getResource(Blank.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()));
	}

	public static void main(String[] args) throws Exception {
		System.out.println(Atlas.wrapperInfo());
		System.out.println(Atlas.componentInfo());
		displayBytecodeBuildTimestamp();
		System.out.println(Atlas.returnArgument("Text from JAVA file"));

		Atlas.launch("Connect", "blank", Atlas.Type.DEFAULT, args );

		for (;;) {
			java.lang.Thread thread = new Thread(new DOM());
			thread.start();
		}
	}
}