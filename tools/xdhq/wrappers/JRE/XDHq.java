/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

package info.q37.xdhq;

import info.q37.jreq.JREq;

public class XDHq extends info.q37.jreq.JREq {
	static private long launcher;
	static {
		launcher = JREq.register( "xdhq" );
	}
	static private Object wrap(int index, Object... objects) {
		return JREq.wrap( launcher, index, objects );
	}
	static public String componentInfo(){
		return JREq.componentInfo( launcher );
	}
	static public String returnArgument( String argument )
	{
		return (String)wrap( 0, argument );
	}
}

/*


class XDHqThread extends java.lang.Thread {
	private XDHq xdhq;

	public XDHqThread(XDHq xdhq) {
		this.xdhq = xdhq;
		xdhq.set(this);
	}

	private void setCasting(boolean enabled, String xslFilename) {
		JREqCastingTree tree = new JREqCastingTree();
		tree.pushTag("Test");

		if (enabled)
			tree.putAttribute("Enabled", "true");
		else
			tree.putAttribute("Enabled", "false");

		xdhq.setDocumentCasting(tree, xslFilename);
	}

	private void connect() {
		JREqLayoutTree tree = new JREqLayoutTree();

		xdhq.setDocumentLayout(tree, "../XSL/MainLayout.xsl");
		setCasting(false, "../XSL/MainCasting.xsl");

	}

	public void run() {
		XDHqData data = new XDHqData();

		System.out.println("Client started...");

		for (;;) {
			xdhq.getAction(data);
			System.out.println(data.action + " : " + data.id);

			switch (data.action) {
			case "":
			case "Connect":
				connect();
				break;
			case "ShowTestButton":
				setCasting(true, "../XSL/MainCasting.xsl");
				break;
			case "HideTestButton":
				setCasting(false, "../XSL/MainCasting.xsl");
				break;
			}
		}
	}

	public void test() {
		System.out.println("Essai from void method!");
	}
}

class XDHqTest {
	private static void displayBytecodeBuildTimestamp() throws Exception {
		System.out.println("Bytecode build : " + new java.util.Date(new java.io.File(XDHqTest.class.getClassLoader()
				.getResource(XDHqTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()));
	}

	public static void main(String[] args) throws Exception {
		System.out.println(XDHq.wrapperInfo());
		System.out.println(XDHq.componentInfo());
		displayBytecodeBuildTimestamp();
		System.out.println(XDHq.returnArgument("Text from JAVA file"));

		XDHq.initialize("12345");

		for (;;) {
			System.out.println("Avant");
			java.lang.Thread thread = new XDHqThread(new XDHq());
			thread.start();
			System.out.println("Apr�s");
		}
	}
}
*/