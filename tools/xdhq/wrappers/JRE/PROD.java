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

package info.q37.xdhq.dom;

import info.q37.xdhq.XDHq;
import info.q37.xdhq.dom.SHRD;

public class PROD extends SHRD {
	private Object core;

	public PROD() {
		core = XDHq.call(2);
	}

	public void finalize() {
		XDHq.call(3, core);
	}

	@Override public Object call(String command, Type type, String[] strings, String[][] xstrings )  {
		return XDHq.call( 5, core, command, type, strings, xstrings );
	}

	@Override public String getAction(Event event) {
		XDHq.call(4, core, event);

		return event.action;
	}

}
