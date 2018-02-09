/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.unjsq.*;

class Thread extends java.lang.Thread {
	private DOM dom;

	public Thread(DOM dom) {
		this.dom = dom;
	}

	public void run() {
		Event event = new Event();

		System.out.println("Connection detected...");

		for (;;) {
			switch (dom.getAction(event)) {
			case "Connect":
				dom.setLayout("", new Tree(), "Main.xsl");
				dom.addClass( "Input", "hidden");
				break;
			case "Submit":
				dom.setContent( "Pattern", dom.getContent( "Pattern" ).toUpperCase() );
				break;
			case "HideInput":
				dom.addClass( "Input", "hidden");
				break;
			case "ShowInput":
				dom.removeClass( "Input", "hidden");
				break;
			default:
				System.out.println( "No or unknown action !");
				System.exit( 1 );
				break;
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
		System.out.println(UnJSq.wrapperInfo());
		System.out.println(UnJSq.componentInfo());
		displayBytecodeBuildTimestamp();
		System.out.println(UnJSq.returnArgument("Text from JAVA file"));

		UnJSq.listen( "blank" );

		for (;;) {
			java.lang.Thread thread = new Thread(new DOM());
			thread.start();
		}
	}
}