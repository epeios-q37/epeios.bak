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

import info.q37.jreq.JRE;

public class XDH_PROD extends XDH_SHRD {
	static private long launcher;
	static {
		launcher = JRE.register( "xdhq" );
	}
	static public Object call(int index, Object... objects) {
		return JRE.call( launcher, index, objects );
	}
	static public void launch() {
		JRE.call( launcher, 1, "53752" );
	}
}