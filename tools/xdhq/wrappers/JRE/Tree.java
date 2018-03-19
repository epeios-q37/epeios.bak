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

public class Tree {
	private Object core;

	public Tree() {
		core = XDHq.call(1);
	}

	public void finalize() {
		XDHq.call(2, core);
	}

	public void pushTag(String name) {
		XDHq.call(3, core, name);
	}

	public void popTag() {
		XDHq.call(4, core);
	}

	public void putValue(String value) {
		XDHq.call(5, core, value);
	}

	public void putAttribute(String name, String value) {
		XDHq.call(6, core, name, value);
	}

	public void putAttribute(String name, boolean value) {
		putAttribute( name, String.valueOf( value ) );
	}

	public void putAttribute(String name, int value ) {
		putAttribute( name, String.valueOf( value ) );
	}

	Object core() {
		return core;
	}
}
