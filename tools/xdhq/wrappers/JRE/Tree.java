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

import info.q37.xdhq.XDHq;

class Tree {
	private Object core;

	public Tree(String background) {
		core = XDHq.wrap(1, background);
	}

	public void finalize() {
		XDHq.wrap(2, core);
	}

	public void pushTag(String name) {
		XDHq.wrap(3, core, name);
	}

	public void popTag() {
		XDHq.wrap(4, core);
	}

	public void putValue(String value) {
		XDHq.wrap(5, core, value);
	}

	public void putAttribute(String name, String value) {
		XDHq.wrap(6, core, name, value);
	}

	Object core() {
		return core;
	}
}
