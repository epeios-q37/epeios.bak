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

import info.q37.xdhq.*;

class DOM {
	private Object core;
	static private String root;

	public static String returnArgument(String Text) {
		return (String) XDHq.wrap(0, Text);
	}

	public static void initialize(String arguments) {
		XDHq.wrap(7, arguments);
		root = "Root";
	}

	public DOM() {
		core = XDHq.wrap(8);
	}

	public void finalize() {
		XDHq.wrap(9, core);
	}

	public void set(Object object) {
		XDHq.wrap(10, core, object);
	}

	public void getAction(info.q37.xdhq.Event data) {
		XDHq.wrap(11, core, data);
	}

	public void setLayout(String id, Tree tree, String xslFilename) {
		XDHq.wrap(12, core, id, tree.core(), xslFilename);
	}
}
